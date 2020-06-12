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
#include "CharCluster.hpp"

// OpenCV includes:
#include <cv.h>
#include <highgui.h>

#include "clustering/Document.hpp"

namespace impact_tr4 {

//float CharCluster::sfpImageCharDistance;
//(const ImageChar& imc1, const ImageChar& imc2);

///*static*/ float (*sfpImageCharDistance)(const ImageChar& imc1, const ImageChar& imc2) = &imageCharDistEucl;

///*static*/ CharCluster::FEATURE_DIST_FUNCTION_TYPE CharCluster::sfpImageCharDistance = &imageCharDistEucl;

///*static*/ float (*sfpImageCharDistance)(const ImageChar& imc1, const ImageChar& imc2) = &imageCharDistEucl;

//sfpImageCharDistance = &imageCharDistEucl;

//void removeChar(std::list<ImageChar*>::iterator& it);
//void removeChars(const std::vector<std::list<ImageChar*>::iterator>& iterators);

/*static*/ const int CharCluster::defaultPrototypeWidth = 60;
/*static*/ const int CharCluster::defaultPrototypeHeight = 60;

void CharCluster::setUnicodeLabel(const std::string &hexUnicodeStr)
{
	sscanf(hexUnicodeStr.c_str(), "%x", &mUnicodeLabel);

	return;
}

std::string CharCluster::unicodeLabelString() const
{
	char buffer[50];
	sprintf(buffer, "%x", mUnicodeLabel);

	return std::string(buffer);
}

void CharCluster::recomputePrototypeImage(const int width/*=defaultPrototypeWidth*/, const int height/*=defaultPrototypeHeight*/)
{
	if (this->empty()) {
		mPrototypeImage.init(width, height); mPrototypeImage.fill(255);
		mPrototypeImageThresh.init(width, height); mPrototypeImage.fill(0);
		mIsPrototypeUpToDate = true;
		return;
	}
//	std::cout << "computing prototype image" << std::endl;

	ImageCharIt it = mCharList.begin();
	GrayImage<int32> sumImage(width, height); sumImage.setZero();
	GrayImage<int32> tempImage(width, height); tempImage.setZero();

	const int maxCount = 30; // max. nr of glyphs for which prototype image is computed
	int count = 0;
	while (it != mCharList.end()) {
		GrayImage<> *pImage = (*it)->pImage;
		BoundingBox bbox = (*it)->bBox;
//		std::cout << bbox << std::endl;
		pImage->setRoi(bbox);
		GrayImage<> glyphImage(width, height);
		pImage->computeRoiImage(glyphImage);
		pImage->releaseRoi();
		glyphImage.resizeRelativeToCenterOfMass(width, height, 255);
		ImageOperations::convert<uint8, int32>(glyphImage, tempImage);
		ImageOperations::add<int32>(sumImage, tempImage, sumImage);
		++it;
		count++;
		if (count >= maxCount)
			break;
	} // end while
	// divide image by nr of elements in cluster and convert back to uint8:
	double scaleFactor = 1.0f;
	if (count != 0)
		scaleFactor = 1.0f / (double)(count);
	mPrototypeImage.init(width, height);
	mPrototypeImage.setZero();
	ImageOperations::convert<int32, uint8>(sumImage, mPrototypeImage, scaleFactor);
	mPrototypeImageThresh = mPrototypeImage;
	mPrototypeImageThresh.binarize(200);
	mPrototypeImageThresh.invert();
	mIsPrototypeUpToDate = true;

	return;
} // end recomputePrototypeImage(width, height)

void CharCluster::addChar(ImageChar* imChar)
{
	mCharList.push_back(imChar);
	mIsPrototypeUpToDate = false;
}

void CharCluster::removeChar(ImageCharIt& it)
{
	mCharList.erase(it);
	mIsPrototypeUpToDate = false;
	return;
}

void CharCluster::removeChars(std::vector<ImageCharIt>& iterators)
{
	for (int i=0; i<iterators.size(); ++i) {
		removeChar(iterators[i]);
	}
	return;
}

void CharCluster::clear()
{
	mCharList.clear();

//	for (ImageCharIt it=begin(); it!=end(); ++it) {
//		this->removeChar(it);
//	}
}

void CharCluster::appendCluster( CharCluster& otherCluster )
{
	mCharList.splice(mCharList.end(), otherCluster.mCharList);
	mIsPrototypeUpToDate = false;
}

void CharCluster::minMaxDims(int *minWidth, int *maxWidth, int *minHeight, int *maxHeight) const
{
	std::list<ImageChar*>::const_iterator it = mCharList.begin();
	int w = (*it)->bBox.width(), h = (*it)->bBox.height();
	*minWidth = w; *minHeight = h;
	*maxWidth = w; *maxHeight = h;
	++it;
	while (it != mCharList.end()) {
		w = (*it)->bBox.width(); h = (*it)->bBox.height();
		if (w < *minWidth) *minWidth = w;
		if (w > *maxWidth) *maxWidth = w;
		if (h < *minWidth) *minHeight = w;
		if (h < *minWidth) *maxHeight = h;
		++it;
	} // end while
	return;
} // end minMaxDims

int CharCluster::determineImageWidth() const
{
	std::list<ImageChar*>::const_iterator it = mCharList.begin();
	int w = (*it)->bBox.width();
	int widthSum = w;
	++it;
	while (it != mCharList.end()) {
		widthSum += (*it)->bBox.width();
		++it;
	} // end while
	return widthSum;
}

void CharCluster::sortClusterByDistanceToClusterCenter()
{
//	std::cout << "Sorting cluster by distance to cluster center..." << std::endl;
	mCharList.sort(compareImageCharByDistToClustCenter);

	return;
} // end sortClusterByDistanceToClusterCenter

#if 0
void CharCluster::computeMean(ublas::vector<float> &mean) const
{
	std::list<ImageChar>::const_iterator it = mCharList.begin();
	if (!(*it).hasFeatureData()) {
		throw NoDataException("No feature data in CharCluster::computeMean!");
	}
	ublas::matrix<float> *pDataMat = (*it).pDataMat;
	mean.resize(pDataMat->size2()); mean.clear();
	int r = -1;
	int nSamples = 0;

	while (it != mCharList.end()) {
		if (!(*it).hasFeatureData()) {
			throw NoDataException("No feature data in CharCluster::computeMean!");
		}
		pDataMat = (*it).pDataMat; // get data matrix pointer
		r = (*it).dataRow; // get row of matrix
		ublas::matrix_row<ublas::matrix<float> > mRow(*pDataMat, r);
		mean += mRow;
		++nSamples;

		++it;
	} // end while
	mean /= nSamples;
}
#endif

// CLUSTER DIFFERENCE FUNCTIONS:

#if 0
float clusterDistMean(const CharCluster& c1, const CharCluster& c2)
{
	if (c1.empty() || c2.empty()) {
		throw Exception("One of the clusters is empty in clusterDistMean");
	}

	ublas::vector<float> m1, m2;
	c1.computeMean(m1); c2.computeMean(m2);
#if 1 // NEW VERSION -> needs massive workaround... FIXME!
	BoundingBox bbox(0,0,1,1); GrayImage<> *pImage = 0x0;
	ImageChar im1(bbox, pImage, -1, -1), im2(bbox, pImage, -1, -1);
	matrix<float> mat1(1,m1.size()); matrix<float> mat2(1, m2.size());
	for (int i=0;i<mat1.size2(); ++i) {
		mat1(0, i) = m1(i); mat2(0, i) = m2(i);
	}
	im1.setFeatureData(&mat1, 0);
	im2.setFeatureData(&mat2, 0);
	return (*CharCluster::sfpImageCharDistance)(im1, im2);
#else
	return ublas::norm_2(m1-m2); // OLD VERSION
#endif
}
#endif

#if 0
float clusterDistWeightedMean(const CharCluster& c1, const CharCluster& c2)
{
	float meanDist = clusterDistMean(c1, c2);
	const float sizeC1 = (float)c1.size();
	const float sizeC2 = (float)c2.size();

	return sqrt(sizeC1*sizeC2/(sizeC1+sizeC2))*meanDist;
}
#endif

float clusterDistAvg(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat)
{
	if (c1.empty() || c2.empty()) {
		throw Exception("One of the clusters is empty in clusterDistMean");
	}

	std::list<ImageChar*>::const_iterator it1 = c1.mCharList.begin();
	std::list<ImageChar*>::const_iterator it2;
	float avg=0;
	float dist=0;
	while (it1 != c1.mCharList.end()) {
		it2 = c2.mCharList.begin();
		while (it2 != c2.mCharList.end()) {
			dist = distMat( (*it1)->id, (*it2)->id );
			avg += dist;
			++it2;
		} // end while it2
		++it1;
	} // end while it1
	avg /= c1.mCharList.size() * c2.mCharList.size();

	return avg;
}

#if 0
float clusterDistAvgDirect(const CharCluster& c1, const CharCluster& c2)
{
	if (c1.empty() || c2.empty()) {
		throw Exception("One of the clusters is empty in clusterDistMean");
	}

	std::list<ImageChar>::const_iterator it1 = c1.mCharList.begin();
	std::list<ImageChar>::const_iterator it2;
	float avg=0;

	ublas::matrix_row<ublas::matrix<float> > mRow(*(*it1).pDataMat, (*it1).dataRow);

	GrayImage<> im1, im2;

	while (it1 != c1.mCharList.end()) {
//		ublas::matrix_row<ublas::matrix<float> > x1(*(*it1).pDataMat, (*it1).dataRow);
		(*it1).pImage->setRoi((*it1).bBox);
		(*it1).pImage->computeRoiImage(im1);
		(*it1).pImage->releaseRoi();

		it2 = c2.mCharList.begin();
		while (it2 != c2.mCharList.end()) {
//			ublas::matrix_row<ublas::matrix<float> > x2(*(*it2).pDataMat, (*it2).dataRow);
			(*it2).pImage->setRoi((*it2).bBox);
			(*it2).pImage->computeRoiImage(im2);
			(*it2).pImage->releaseRoi();
//			avg += ublas::norm_2(x1-x2);

//			double cvMatchShapes( const void* object1, const void* object2,
//			                      int method, double parameter=0 );
//
			const int method = 1; // 1, 2 or 3
			avg += cvMatchShapes( im1.iplImage(), im2.iplImage(), method );
			++it2;
		} // end while it2
		++it1;
	} // end while it1
	avg /= c1.mCharList.size() * c2.mCharList.size();

	return avg;
}
#endif

float clusterDistMin(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat)
{
	if (c1.empty() || c2.empty()) {
		throw Exception("One of the clusters is empty in clusterDistMean");
	}

	std::list<ImageChar*>::const_iterator it1 = c1.mCharList.begin();
	std::list<ImageChar*>::const_iterator it2;
	float min=1e32f;
	float dist=0.0f;

	while (it1 != c1.mCharList.end()) {
		it2 = c2.mCharList.begin();
		while (it2 != c2.mCharList.end()) {
			dist = distMat( (*it1)->id, (*it2)->id );
			if (dist < min) min = dist;
			++it2;
		} // end while it2
		++it1;
	} // end while it1

	return min;
}

float clusterDistMax(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat)
{
	if (c1.empty() || c2.empty()) {
		throw Exception("One of the clusters is empty in clusterDistMean");
	}

	std::list<ImageChar*>::const_iterator it1 = c1.mCharList.begin();
	std::list<ImageChar*>::const_iterator it2;
	float max=0.0f, dist=0.0f;

	while (it1 != c1.mCharList.end()) {
		it2 = c2.mCharList.begin();
		while (it2 != c2.mCharList.end()) {
			dist = distMat( (*it1)->id, (*it2)->id );
			if (dist > max) max = dist;
			++it2;
		} // end while it2
		++it1;
	} // end while it1

	return max;
}

} // end of namespace impact_tr4
