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
#ifndef OPENCV_HPP_
#define OPENCV_HPP_

// OpenCV includes:
#include <cv.h>
#include <highgui.h>
#include <ml.h> // machine learning of opencv

// boost includes:
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

// Project includes:
#include "core/core.h"
#include "core/image/GrayImage.hpp"

namespace impact_tr4 {

//typedef struct CvSVMParams CvSVMParams;

//using namespace boost::numeric::ublas;

//template <class T> class GrayImage;

enum VectorType
{
	ROW_VECTOR = 0,
	COLUMN_VECTOR = 1
};

enum DataAlignmentType
{
	ROWWISE_ALIGNMENT = 0,
	COLUMNWISE_ALIGNMENT
};

enum InversionType
{
	LU_INVERSION = CV_LU,
	SVD_INVERSION = CV_SVD,
	SVD_SYM_INVERSION = CV_SVD_SYM
};

class OpenCV;

/**
 * Class including some basic function for OpenCV functionality.
 * 
 * */
class OpenCV
{
public:
	template <class T> static int openCVDepth()
		{ return GrayImage<T>::determineDepth(); }

	template <class T, int nChannels> static int cvMatType();

	template <class T> static CvMat*
	cvMatFromBoostMat(const ublas::matrix<T> &m);

	/** Create a matrix from a boost vector.
	 * @param vecType Specifies, if a row (vecType==ROW_VECTOR) or column (vecType!=COLUMN_VECTOR) vector
	 * should be created.
	 * */
	template <class T> static CvMat*
	cvMatFromBoostVec(const ublas::vector<T> &v, const int vecType /*= ROW_VECTOR || COLUMN_VECTOR */);

	/**
	 * Computes the covariance matrix of a given matrix of data vectors.
	 * @param inputMat The given matrix of input vectors.
	 * @param covMat The resulting covariance matrix.
	 * @param avgVec The resulting vector of vector averages.
	 * @param dataAlignment The alignment of the data, i.e. either ROWWISE_ALIGNMENT,
	 *  meaning that each input vector is stored as a row in inputMat or COLUMNWISE_ALIGNMENT
	 * vice versa. Default = ROWWISE_ALIGNMENT
	 * @param normalize If true, the covariance matrix is normalized (i.e. divided by nSamples-1). Default = true
	 */
	template <class T> static void
	computeCovarianceMatrix(const ublas::matrix<T> &inputMat, ublas::matrix<T> &covMat, ublas::vector<T> &avgVec, const DataAlignmentType dataAlignment = ROWWISE_ALIGNMENT, const bool normalize = true);

	/**
	 * Computes the inverse of the specified matrix.
	 * @param invType The inversion algorithm, must be either:
	 * - LU_INVERSION: Gaussian elimination with optimal pivot element chose (default)
	 * - SVD_INVERSION: Singular value decomposition (SVD) method
	 * - SVD_SYM_INVERSION: SVD method for a symmetric positively-defined matrix
	 * @return If LU_INVERSION, determinant is returned. If 0, input is singular and inverse is filled with zeros.
	 * In case of SVD methods the function returns the inversed condition number of input (ratio of the smallest singular value to the largest singular value)
	 *  and 0 if input is all zeros. The SVD methods calculate a pseudo-inverse matrix if input is singular.
	 */
	template <class T> static double
	computeInverseMatrix(const ublas::matrix<T> &input, ublas::matrix<T> &inverse, const InversionType invType = LU_INVERSION);


	/**
	 * Returns the Mahalanobis distance between two vectors vec1 and vec2.
	 * @param invCovMat The inverse covariance matrix.
	 */
	template <class T> static double
	mahalanobisDist( const ublas::vector<T> &vec1, const ublas::vector<T> &vec2, const ublas::matrix<T> invCovMat );


}; // end class OpenCV

template <class T> /*static*/ void
OpenCV::computeCovarianceMatrix(const ublas::matrix<T> &inputMat,
						ublas::matrix<T> &covMat, ublas::vector<T> &avgVec,
						const DataAlignmentType dataAlignment /*= ROWWISE_ALIGNMENT*/,
						const bool normalize /*= true*/)
{
	// create input matrix:
	const CvMat* pInputMatCv = cvMatFromBoostMat<T>(inputMat);
	// get nr of samples and sample dim:
	const int nSamples = (dataAlignment == ROWWISE_ALIGNMENT) ? inputMat.size1() : inputMat.size2();
	const int dim = (dataAlignment == ROWWISE_ALIGNMENT) ? inputMat.size2() : inputMat.size1();
	// create cov matrix:
	covMat.resize(dim, dim);
	CvMat *pCovMatCv = cvMatFromBoostMat<T>(covMat);
	// create average vector:
	avgVec.resize(dim);
	CvMat *pAvgVecCv = (dataAlignment == ROWWISE_ALIGNMENT) ?
						cvMatFromBoostVec<T>(avgVec, ROW_VECTOR) :
						cvMatFromBoostVec<T>(avgVec, COLUMN_VECTOR);
	// determine flags:
	int flags = (dataAlignment == ROWWISE_ALIGNMENT) ?
				(CV_COVAR_NORMAL | CV_COVAR_ROWS) :
				(CV_COVAR_NORMAL | CV_COVAR_COLS);
	// calculate covariance matrix using OpenCV function:
	cvCalcCovarMatrix( (const CvArr **)&pInputMatCv, nSamples, pCovMatCv, pAvgVecCv, flags );
	// normalize matrix by dividing with nSamples-1 if desired
	if (normalize) {
		covMat /= (nSamples-1);
	}
	return;
} // end computeCovarianceMatrix

template <class T> /*static*/ double
OpenCV::computeInverseMatrix(const ublas::matrix<T> &input, ublas::matrix<T> &inverse, const InversionType invType /*= LU_INVERSION*/)
{
	if (input.size1() != input.size2())
		throw Exception("Matrix is not square in OpenCV::computeInverseMatrix!");

	// create input matrix:
	const CvMat* pInputMatCv = cvMatFromBoostMat<T>(input);
	// create output matrix:
	inverse.resize(input.size1(), input.size2());
	CvMat* pInverseMatCv = cvMatFromBoostMat<T>(inverse);

	return cvInvert( pInputMatCv, pInverseMatCv, invType );
}

template <class T> /*static*/ double
OpenCV::mahalanobisDist( const ublas::vector<T> &vec1, const ublas::vector<T> &vec2, const ublas::matrix<T> invCovMat )
{
	// create the OpenCV structures:
	CvMat *pVec1Cv = cvMatFromBoostVec<T>(vec1, ROW_VECTOR);
	CvMat *pVec2Cv = cvMatFromBoostVec<T>(vec2, ROW_VECTOR);
	CvMat *pInvCovMatCv = cvMatFromBoostMat<T>(invCovMat);

	return cvMahalanobis( pVec1Cv, pVec2Cv, pInvCovMatCv );
}

template <class T> /*static*/ inline CvMat*
OpenCV::cvMatFromBoostMat(const ublas::matrix<T> &m)
{
	CvMat* cvMat = cvCreateMatHeader( m.size1(), m.size2(), OpenCV::cvMatType<T, 1>() );
	cvSetData( cvMat, (void*)(&m(0,0)), cvMat->step );
	return cvMat;
}

/** Create a matrix from a boost vector.
 * @param vecType Specifies, if a row (vecType==ROW_VECTOR) or column (vecType!=COLUMN_VECTOR) vector
 * should be created.
 * */
template <class T> /*static*/ inline CvMat*
OpenCV::cvMatFromBoostVec(const ublas::vector<T> &v, const int vecType /*=ROW_VECTOR || COLUMN_VECTOR*/ )
{
	CvMat *cvMat =
		(vecType==ROW_VECTOR)
			?
		cvCreateMatHeader( 1, v.size(), OpenCV::cvMatType<T, 1>() )
			:
		cvCreateMatHeader( v.size(), 1, OpenCV::cvMatType<T, 1>() );

		cvSetData( cvMat, (void*)(&v[0]), cvMat->step );
		return cvMat;
}

/// Returns cvMat type for unsigned int8 values, 1 channel
template <> /*static*/ inline int OpenCV::cvMatType<uint8, 1>() { return  CV_8UC1; }
/// Returns cvMat type for signed int8 values, 1 channel
template <> /*static*/ inline int OpenCV::cvMatType<int8, 1>() { return  CV_8SC1; }
/// Returns cvMat type for unsigned int16 values, 1 channel
template <> /*static*/ inline int OpenCV::cvMatType<uint16, 1>() { return  CV_16UC1; }
/// Returns cvMat type for signed int16 values, 1 channel
template <> /*static*/ inline int OpenCV::cvMatType<int16, 1>() { return  CV_16SC1; }
/// Returns cvMat type for signed int32 values, 1 channel
template <> /*static*/ inline int OpenCV::cvMatType<int32, 1>() { return  CV_32SC1; }
/// Returns cvMat type for float values, 1 channel
template <> /*static*/ inline int OpenCV::cvMatType<float, 1>() { return  CV_32FC1; }
/// Returns cvMat type for double values, 1 channel
template <> /*static*/ inline int  OpenCV::cvMatType<double, 1>() { return  CV_64FC1; }

} // end namespace impact_tr4

#endif /*OPENCV_HPP_*/
