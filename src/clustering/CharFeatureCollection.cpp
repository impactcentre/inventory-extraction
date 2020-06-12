/*******************************************************************************
 * 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#include "CharFeatureCollection.hpp"

#include "core/PCA.hpp"

namespace impact_tr4
{

CharFeatureCollection::CharFeatureCollection()
{
	mDataMatrix.clear();
	mImageCharFeatureVec.clear();
}

// copy constructor:
//CharFeatureCollection::CharFeatureCollection(const CharFeatureCollection& source)
//{
//	mDataMatrix = source.mDataMatrix;
//	mImageCharFeatureVec.resize(source.mImageCharFeatureVec.size());
//	for (int i=0; i<mImageCharFeatureVec.size(); ++i) {
//		mImageCharFeatureVec[i] = new ImageCharFeatureI(source.mImageCharFeatureVec[i]);
//	}
//
//	std::vector<ImageCharFeatureI*> mImageCharFeatureVec;
//}

/*virtual*/ CharFeatureCollection::~CharFeatureCollection()
{
	deleteFeatures();
	mDataMatrix.clear();
	mImageCharFeatureVec.clear();
}



//void CharFeatureCollection::setImageCharFeatureVec(const std::vector<ImageCharFeatureI*> &imageCharFeatureVec)
//{
//	deleteFeatures();
//	mImageCharFeatureVec = imageCharFeatureVec;
//	return;
//}

void CharFeatureCollection::deleteFeatures()
{
	for (int i=0; i<mImageCharFeatureVec.size(); ++i) {
		delete mImageCharFeatureVec[i];
		mImageCharFeatureVec[i] = NULL;
	}
	mImageCharFeatureVec.clear();
	return;
}

void CharFeatureCollection::addFeature(ImageCharFeatureI* pFeature)
{
	mImageCharFeatureVec.push_back(pFeature);
	return;
}

void CharFeatureCollection::deleteFeature(const int i)
{
//	delete mImageCharFeatureVec[mImageCharFeatureVec.begin()+i]; // delete feature from memory
	delete mImageCharFeatureVec[i]; // delete feature from memory
	mImageCharFeatureVec.erase(mImageCharFeatureVec.begin()+i); // erase corresponding pointer from vector

	return;
}

void CharFeatureCollection::calculateOutputDataRow(const GrayImage<> &image, const int rowIndex, ublas::matrix<float> &dataMatrix) const
{
	int currColIndex = 0;
	for (int j=0; j<mImageCharFeatureVec.size(); ++j) {
		mImageCharFeatureVec[j]->calculateOutputVector(image, rowIndex, currColIndex, dataMatrix);
//			std::cout << "successfully computed an output vector!" << std::endl;
		currColIndex += mImageCharFeatureVec[j]->vectorSize();
	} // end for all image chars j

	return;
} // end calculateOutputDataRow(...)

void CharFeatureCollection::calculateOutputDataMatrix(const std::vector<GrayImage<> *> &images)
{
	// check if there are features:
	if (mImageCharFeatureVec.empty()) {
		std::cout << "Exception in CharFeatureCollection::calculateOutputDataMatrix(const std::vector<GrayImage<> > &images): no feature vector list!" << std::endl;
		throw NoDataException("No features specified for feature vector calculation!");
	}

	// calc. size of one column:
	int nCols = 0;
	for (int i=0; i<mImageCharFeatureVec.size(); ++i) {
		nCols += mImageCharFeatureVec[i]->vectorSize();
	}
	// resize output matrix:
	const int nRows = images.size();
	std::cout << "Matrix dimensions are: " << nRows << " x " << nCols << std::endl;
	mDataMatrix.resize(nRows, nCols);
	#pragma omp parallel for
	for (int i=0; i<nRows; ++i) {
		this->calculateOutputDataRow(*images[i], i, mDataMatrix);
		std::cout << "1Computed feature " << i+1 << " of " << nRows << std::endl;
	} // end for all rows i

	return;
}

void CharFeatureCollection::calculateOutputDataMatrix(std::vector<ImageChar*> &imageChars)
{
	// check if there are features:
	if (mImageCharFeatureVec.empty()) {
		std::cout << "Exception in CharFeatureCollection::calculateOutputDataMatrix(std::vector<ImageChar> &imageChars): no feature vector list!" << std::endl;
		throw NoDataException("No features specified for feature vector calculation!");
	}

	// calc. size of one column:
	int nCols = 0;
	for (int i=0; i<mImageCharFeatureVec.size(); ++i) {
		nCols += mImageCharFeatureVec[i]->vectorSize();
	}
	// resize output matrix:
	const int nRows = imageChars.size();
	std::cout << "2 matrix dimensions are: " << nRows << " x " << nCols << std::endl;
	mDataMatrix.resize(nRows, nCols);
#if 1 // take preprocessed image from pointer in ImageChar
	#pragma omp parallel for
	for (int i=0; i<nRows; ++i) {
#if 0
		if (imageChars[i]->mPreprocessingResults.isEmpty()) {
			imageChars[i]->mPreprocessingResults
		}

#else
//		GrayImage<> *pPreprImage = imageChars[i]->pPreprImage;
		GrayImage<> *pPreprImage = imageChars[i]->mPreprocessingResults.mpProcessedImage;
		if (!pPreprImage) {
			std::cerr << "Preprocessing image not available while computing features!" << std::endl;
			throw NoDataException("Preprocessing image not available while computing features!");
		}
#endif
		this->calculateOutputDataRow(*pPreprImage, i, mDataMatrix);
		std::cout << "2Computed feature " << i+1 << " of " << nRows << std::endl;
		// store reference to feature vector in ImageChar of mpImageCharsVec:
		imageChars[i]->setFeatureData(&mDataMatrix, i);
	} // end for all rows i
#else // extract bounding box image from ImageChar
	for (int i=0; i<nRows; ++i) {
		GrayImage<> *pImage = imageChars[i]->pImage;
		BoundingBox bbox = imageChars[i]->bBox;
		pImage->setRoi(bbox);
		GrayImage<> charImage;
		pImage->computeRoiImage(charImage);
		pImage->releaseRoi();
//		std::cout << "current row: " << i << std::endl;
		this->calculateOutputDataRow(charImage, i, mDataMatrix);
		// store reference to feature vector in ImageChar of mpImageCharsVec:
		imageChars[i]->setFeatureData(&mDataMatrix, i);
	} // end for all rows i
#endif

	return;
} // end calculateOutputDataMatrix

void CharFeatureCollection::reduceMatrixDimensionsThreshold(const float thresh)
{
	PCA<float> pca(mDataMatrix);
	pca.computePCA();
	// reduce dimension:
	const int index = pca.dimReductionIndex(thresh);
	std::cout << "thresh = " << thresh << ", index = " << index << std::endl;
	ublas::matrix<float> reducedDataMatrix;
	pca.project(index+1, reducedDataMatrix);
	std::cout << "dimension before reduction: "
		<< mDataMatrix.size2() << std::endl;
	mDataMatrix = reducedDataMatrix;
	std::cout << "dimension after reduction: "
		<< mDataMatrix.size2() << std::endl;

	return;
} // end reduceMatrixDimensionsThreshold(...)

void CharFeatureCollection::reduceMatrixDimensionsNewDim(const int newDim)
{
	PCA<float> pca(mDataMatrix);
	pca.computePCA();
	// reduce dimension:
//	const int index = pca.dimReductionIndex(thresh);
//	std::cout << "thresh = " << thresh << ", index = " << index << std::endl;
	ublas::matrix<float> reducedDataMatrix;
	pca.project(newDim, reducedDataMatrix);
	std::cout << "dimension before reduction: "
		<< mDataMatrix.size2() << std::endl;
	mDataMatrix = reducedDataMatrix;
	std::cout << "dimension after reduction: "
		<< mDataMatrix.size2() << std::endl;

	return;
} // end reduceMatrixDimensions(...)

} // end of namespace impact_tr4

