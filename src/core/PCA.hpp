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
#ifndef PCA_HPP_
#define PCA_HPP_

// system includes
#include <iostream>

// boost includes
// boost includes:
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

// project includes
#include "core_headers.h"
#include "core/OpenCV.hpp"
//#include "image_headers.h"

namespace impact_tr4
{

template <class T>
class PCA
{
public:
	PCA(const ublas::matrix<T> &dataMatrix);
	virtual ~PCA() { mpDataMatrix=0x0; }

	void setDataMatrix(const ublas::matrix<T> &dataMatrix);

	/** Clears all pca data. */
	void clearPCAData() {
		mAvgVec.resize(0);
		mEigenvaluesVec.resize(0);
		mEigenvectorsMat.resize(0,0);
		mEigenvalueSum = 0;
		return;
	}

	/** Returns true, if PCA was calculated for current data matrix. */
	bool hasPCAData() const
		{ return (mAvgVec.size()!=0); }


	/** Computes the PCA for the currently set data matrix. */
	void computePCA();
	void project(const int newDim, ublas::matrix<T> &coefficients);
	void backProject(const ublas::matrix<T>& coefficients, ublas::matrix<T> &result);
	void printPCAValues();

	/** Returns the dimension of the input data. */
	const int dimension() const { return mAvgVec.size(); }
	/** Returns the weight of a specified dimenion, i.e. a scalar in [0,1]. */
	const T dimWeight(const int i) {
		return mEigenvaluesVec[i]/mEigenvalueSum;
	}
	/** Returns the index i, for which the sum of the dimension weights
	 * up to this index are smaller than or equal than the given value thresh.
	 * Note, that the thresh should (but must not) be in [0, 1].
	 * This function can be used for dimension reduction using PCA
	 * (i.e. "select the first i dimensions, which cover 90% of the data"...).
	 * */
	const int dimReductionIndex(const T thresh);

	const int eigenvalueSum() const { return mEigenvalueSum; }

	const ublas::vector<T>& avgVec() const { return mAvgVec; }
	const ublas::vector<T>& eigenvaluesVec() const { return mEigenvaluesVec; }
	const ublas::matrix<T>& eigenvectorsMat() const { return mEigenvectorsMat; }

private:
	const ublas::matrix<T> *mpDataMatrix;
	int mType;
	T mEigenvalueSum;

	ublas::vector<T> mAvgVec, mEigenvaluesVec;
	ublas::matrix<T> mEigenvectorsMat;
//	CvMat *mpDataCvMat, *mpAvgCvMat, *mpEigenvaluesCvMat, *mpEigenvectorsCvMat;
};

template <class T>
PCA<T>::PCA(const ublas::matrix<T> &dataMatrix)
{
	setDataMatrix( dataMatrix );
}

template <class T>
void PCA<T>::setDataMatrix(const ublas::matrix<T> &dataMatrix)
{
	mType = OpenCV::cvMatType<T, 1>();
	if (mType != CV_32FC1 && mType != CV_64FC1) {
		throw UnsupportedTypeException("Unsupported type in PCA::setDataMatrix - only float or double are supported!");
	}
	mpDataMatrix = &dataMatrix;
	return;
}

template <class T>
const int PCA<T>::dimReductionIndex(const T thresh) {
	T sum = 0;
	for (int i=0; i<dimension(); ++i) {
		sum += dimWeight(i);
		if (sum > thresh) return i;
	}
	return dimension()-1;
}

template <class T>
void PCA<T>::computePCA()
{
	// Create necessary data structures:
	CvMat *pDataCvMat = OpenCV::cvMatFromBoostMat<T>(*mpDataMatrix);

	const int nDims = pDataCvMat->cols;
//	const int nSamples = pDataCvMat->rows;

	mAvgVec.resize( nDims );
	CvMat *pAvgCvMat = OpenCV::cvMatFromBoostVec<T>( mAvgVec, ROW_VECTOR );
	mEigenvaluesVec.resize( nDims );
	CvMat *pEigenvaluesCvMat = OpenCV::cvMatFromBoostVec<T>( mEigenvaluesVec, ROW_VECTOR );
	mEigenvectorsMat.resize( nDims, nDims );
	CvMat *pEigenvectorsCvMat = OpenCV::cvMatFromBoostMat<T>( mEigenvectorsMat );

	// Calculate PCA:
	cvCalcPCA(	pDataCvMat,
				pAvgCvMat,
				pEigenvaluesCvMat,
				pEigenvectorsCvMat,
				CV_PCA_DATA_AS_ROW );

	mEigenvalueSum = sumVec<T>(mEigenvaluesVec);
//	std::cout << "eigenvalue sum = " << mEigenvalueSum << std::endl;

	return;
} // end computePCA

template <class T>
void PCA<T>::project(const int newDim, ublas::matrix<T> &coefficients)
{
	if (!hasPCAData()) {
		throw NoDataException("No PCA data calculated yet!");
	}
	if ( newDim > mEigenvectorsMat.size1() ) {
		throw WrongDimensionException("Wrong dimension in PCA::project: number of projection dimensions must be <= number of rows in eigenvector matrix!");
	}

	coefficients.resize(mpDataMatrix->size1(), newDim);

	// create data structures:
	CvMat *pDataCvMat = OpenCV::cvMatFromBoostMat<T>(*mpDataMatrix);
	CvMat *pAvgCvMat = OpenCV::cvMatFromBoostVec<T>( mAvgVec, ROW_VECTOR );
	CvMat *pEigenvectorsCvMat = OpenCV::cvMatFromBoostMat<T>( mEigenvectorsMat );
	CvMat *pCoeffMat = OpenCV::cvMatFromBoostMat<T>( coefficients );

	cvProjectPCA( pDataCvMat, pAvgCvMat, pEigenvectorsCvMat, pCoeffMat );

	return;
}

template <class T>
void PCA<T>::backProject(const ublas::matrix<T> &coefficients, ublas::matrix<T> &result)
{
	// create data structures:
	CvMat *pCoeffMat = OpenCV::cvMatFromBoostMat<T>( coefficients );
	CvMat *pAvgCvMat = OpenCV::cvMatFromBoostVec<T>( mAvgVec, ROW_VECTOR );
	CvMat *pEigenvectorsCvMat = OpenCV::cvMatFromBoostMat<T>( mEigenvectorsMat );
	result.resize( coefficients.size1(), mAvgVec.size() );
	CvMat *pResultMat = OpenCV::cvMatFromBoostMat<T>( result );

	cvBackProjectPCA( pCoeffMat, pAvgCvMat, pEigenvectorsCvMat, pResultMat );

	return;
}

template <class T>
void PCA<T>::printPCAValues()
{
	std::cout << "Average Vector:" << std::endl;
	std::cout << mAvgVec << std::endl;
	std::cout << "------------------------" << std::endl;
	std::cout << "Eigenvalues Vector:" << std::endl;
	std::cout << mEigenvaluesVec << std::endl;
	std::cout << "------------------------" << std::endl;
	std::cout << "Eigenvectors Matrix:" << std::endl;
	std::cout << mEigenvectorsMat << std::endl;
	std::cout << "------------------------" << std::endl;
	return;
} // end printPCAValues

} // end of namespace impact_tr4

#endif /*PCA_HPP_*/
