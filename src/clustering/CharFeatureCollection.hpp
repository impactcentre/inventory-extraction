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
#ifndef CHARFEATURECOLLECTION_HPP_
#define CHARFEATURECOLLECTION_HPP_

// system includes:
#include <iostream>
// boost includes:
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

// local includes:
#include "DataStructs.hpp"
#include "features/ImageCharFeatureI.hpp"

namespace impact_tr4
{

class CharFeatureCollection
{
public:
	CharFeatureCollection();
//	CharFeatureCollection(const CharFeatureCollection& source);
	virtual ~CharFeatureCollection();

	const bool hasDataMatrix() const
		{ return (mDataMatrix.size1()!=0 && mDataMatrix.size2()!=0); }
	void clearDataMatrix()
		{ mDataMatrix.clear(); }

	void calculateOutputDataMatrix(std::vector<ImageChar*> &imageChars);
	void calculateOutputDataMatrix(const std::vector<GrayImage<> *> &images);

	void calculateOutputDataRow(const GrayImage<> &image, const int rowIndex, ublas::matrix<float> &dataMatrix) const;

	ublas::matrix<float>* dataMatrixPointer() { return &mDataMatrix; }
	ublas::matrix<float>& dataMatrixRef() { return mDataMatrix; }
	const ublas::matrix<float>& dataMatrixRef() const { return mDataMatrix; }

	// PCA dimension reduction:
	void reduceMatrixDimensionsThreshold(const float thresh);
	void reduceMatrixDimensionsNewDim(const int newDim);

//	void setImageCharFeatureVec(const std::vector<ImageCharFeatureI*> &imageCharFeatureVec);
	const int nFeatures() const { return mDataMatrix.size1(); }
	const int featuresDim() const { return mDataMatrix.size2(); }
	void deleteFeatures();
	void addFeature(ImageCharFeatureI* pFeature);
	void deleteFeature(const int i);

private:
	ublas::matrix<float> mDataMatrix;
	std::vector<ImageCharFeatureI*> mImageCharFeatureVec;
};

} // end of namespace impact_tr4

#endif /*CHARFEATURECOLLECTION_HPP_*/
