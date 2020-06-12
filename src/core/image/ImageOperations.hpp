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
#ifndef IMAGEOPERATIONS_HPP_
#define IMAGEOPERATIONS_HPP_

// System includes:
#include <iostream>

// Local includes:
#include "util/platform.h"
#include "GrayImage.hpp"

namespace impact_tr4 {

template <class T> class GrayImage;

/**
 * Class representing basic operations for images (e.g. add, sub, mul etc.).
 * @see GrayImage, Morhology, ImageFilter
 * 
 * */
//template <class T = uint8>
class ImageOperations
{
public:
	// BASIC OPERATIONS:
	/** Performs element-wise maximum operation of src1 and src2 and stores the result int result. */
	template <class T>
	static int max(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise minimum operation of src1 and src2 and stores the result int result. */
	template <class T>
	static int min(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise addition of src1 and src2 and stores the result int result. */
	template <class T>
	static int add(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise substraction of src1 and src2 and stores the result int result. */
	template <class T>
	static int sub(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise multiplication of src1 and src2 and stores the result int result. */
	template <class T>
	static int mul(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise division of src1 and src2 and stores the result int result. */
	template <class T>
	static int div(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Adds value to all element of src1 and stores the result in result. */
	template <class T>
	static int addScalar(const GrayImage<T>& src1, T value, GrayImage<T>& result);
	/** Calculates src1 - value for all elements and stores the result in result. */
	template <class T>
	static int subScalar(const GrayImage<T>& src1, T value, GrayImage<T>& result);
	/** Calculates value - src1 for all elements and stores the result in result. */
	template <class T>
	static int subScalar(T value, const GrayImage<T>& src1, GrayImage<T>& result);
	/** Calculates the absolute difference between the two images. */
	template <class T>
	static int absDiff(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);

	/** Extends the src image in all directions using the specified value val. */
	template <class T>
	static int extend(const GrayImage<T>& src, GrayImage<T>& result, const int nLeft, const int nRight, const int nTop, const int nBottom, const T val = 0);

	template <class T>
	static int extend(const GrayImage<T>& src, GrayImage<T>& result, const int n, const T val = 0)
		{ return extend(src, result, n, n, n, n, val); }

	/** Extends the image, s.t. the center of the image is the specified index. */
	template <class T>
	static int extendToCenter(const GrayImage<T>& src, GrayImage<T>& result, const Index &center, const int border=0, const T extVal = 0);

	template <class T>
	static int rotateAndScale(const GrayImage<T>& src, GrayImage<T>& result, const Index center, const double angle, const double scale = 1, const T fillVal = 0, const int flags = CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS );

	/** Converts the given image src to the image with the depth of result.
	 * Additionally a scaling and shifting factor can be specified for the conversion.
	 * result = scale*src + shift.
	 * */
	template <class T1, class T2>
	static int convert(const GrayImage<T1>& src, GrayImage<T2> &result, double scale=1, double shift=0 );

	// LOGIC:
	/** Performs element-wise bitwise 'and'-operation of src1 and src2 and stores it in result. */
	template <class T>
	static int andOp(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise bitwise 'and'-operation of src1 and value and stores it in result. */
	template <class T>
	static int andScalarOp(const GrayImage<T>& src1, T value, GrayImage<T>& result);
	/** Performs element-wise bitwise 'or'-operation of src1 and src2 and stores it in result. */
	template <class T>
	static int orOp(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise bitwise 'or'-operation of src1 and value and stores it in result. */
	template <class T>
	static int orScalarOp(const GrayImage<T>& src1, T value, GrayImage<T>& result);
	/** Performs element-wise bitwise 'xor'-operation of src1 and src2 and stores it in result. */
	template <class T>
	static int xorOp(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result);
	/** Performs element-wise bitwise 'xor'-operation of src1 and value and stores it in result. */
	template <class T>
	static int xorScalarOp(const GrayImage<T>& src1, T value, GrayImage<T>& result);
	/** Performs element-wise bitwise inversion of the source image src and stores it in result. */
	template <class T>
	static int notOp(const GrayImage<T>& src, GrayImage<T>& result);

	// STATISTICS:
	/** Counts the non-zero elements of the specified image. */
	template <class T>
	static int countNonZero( const GrayImage<T>& src );
	/** Calculates the sum of all pixel values of the specified image. */
	template <class T>
	static double sum( const GrayImage<T>& src );
	/** Calculates the average of all pixel values of the specified image. */
	template <class T>
	static double avg( const GrayImage<T>& src );
}; // end class ImageOperations

/** Abbreviation for the ImageOperations class, s.t. functions can be used typing ImOp::xyz() */
typedef ImageOperations ImOp;


// BASIC OPERATIONS:

//template <class T>1
template <class T>
/*static*/ inline
int
ImageOperations::max(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::min: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvMax(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end max

template <class T>
/*static*/ inline
int
ImageOperations::min(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::min: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvMin(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end min

template <class T>
/*static*/ inline
int
ImageOperations::add(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::add: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvAdd(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end add

template <class T>
/*static*/ inline
int
ImageOperations::sub(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::sub: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvSub(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end sub

template <class T>
/*static*/ inline
int
ImageOperations::mul(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::sub: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvMul(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end sub

template <class T>
/*static*/ inline
int
ImageOperations::div(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::sub: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvDiv(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end div

template <class T>
/*static*/ inline
int
ImageOperations::addScalar(const GrayImage<T>& src1, const T value, GrayImage<T>& result)
{
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvAddS(src1.mpImage, cvRealScalar( value ), result.mpImage);

	return 1;
} // end addScalar

template <class T>
/*static*/ inline
int
ImageOperations::subScalar(const GrayImage<T>& src1, const T value, GrayImage<T>& result)
{
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvSubS(src1.mpImage, cvRealScalar( value ), result.mpImage);

	return 1;
} // end subScalarRight

template <class T>
/*static*/ inline
int
ImageOperations::subScalar(const T value, const GrayImage<T>& src1, GrayImage<T>& result)
{
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvSubRS(src1.mpImage, cvRealScalar( value ), result.mpImage);

	return 1;
} // end subScalarLeft

template <class T>
/*static*/ inline
int
ImageOperations::absDiff(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2)) {
		std::cerr << "Error in ImageOperations::absDiff: Given image dimensions do not agree!" << std::endl;
		return 0;
		}
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvAbsDiff(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
}

template <class T>
/*static*/
int
ImageOperations::extend(const GrayImage<T>& src, GrayImage<T>& result, const int nLeft, const int nRight, const int nTop, const int nBottom, const T val/* = 0*/)
{
	const int oldW = src.width();
	const int oldH = src.height();
	const int newW = oldW + nLeft + nRight;
	const int newH = oldH + nTop + nBottom;

	std::cout << oldW << ", " << oldH << ", " << newW << ", " << newH << std::endl;

	result.init(newW, newH);
//	if (val!=0) // fill with background values; if val zero, it is already set!
	result.fill(val);

	result.set(nTop, nLeft, src);

	return 1;
}

template <class T>
/*static*/
int
ImageOperations::extendToCenter(const GrayImage<T>& src, GrayImage<T>& result, const Index &center, const int border/*=0*/, const T extVal/*=0*/)
{
	const int oldH = src.height();
	const int oldW = src.width();
	std::cout << oldW << ", " << oldH << std::endl;

	int max = -1;
	const int diffLeft = center(0); max = diffLeft;
	const int diffRight = oldH - center(0) - 1; if (diffRight > max) max = diffRight;
	const int diffTop = center(1);  if (diffTop > max) max = diffTop;
	const int diffBottom = oldW - center(1) - 1;  if (diffBottom > max) max = diffBottom;
	std::cout << diffLeft << ", " << diffRight << ", " << diffTop << ", " << diffBottom << ", " << max << std::endl;
	// determine extension in each direction:
	const int nLeft = max - diffLeft + border;
	const int nRight = max - diffRight + border;
	const int nTop = max - diffTop + border;
	const int nBottom = max - diffBottom + border;
	std::cout << nLeft << ", " << nRight << ", " << nTop << ", " << nBottom << std::endl;

	ImageOperations::extend<T>(src, result, nLeft, nRight, nTop, nBottom, extVal);

	return 1;
}

template <class T>
/*static*/ int
ImageOperations::rotateAndScale(const GrayImage<T>& src, GrayImage<T>& result, const Index center, const double angle, const double scale /*= 1*/, const T fillVal /*= 0*/, const int flags /*= CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS*/)
{
//	CvPoint2D32f centre;
	CvMat *transMatrix = cvCreateMat(2, 3, CV_32FC1);
	cvSetZero(transMatrix);
//	centre.x = centerIndex(0);
//	centre.y = centerIndex(1);
	cv2DRotationMatrix(cvPointTo32f(center.toCvPoint()), angle, scale, transMatrix);
	cvWarpAffine(src.mpImage, result.mpImage, transMatrix, flags, cvScalarAll(fillVal));
	cvReleaseMat(&transMatrix);

	return 1;
} // end rotateAndScale

template <class T1, class T2>
/*static*/ inline
int
ImageOperations::convert(const GrayImage<T1>& src, GrayImage<T2> &result, double scale/*=1*/, double shift/*=0*/ )
{
//	if (!src.hasEqualSizeRoi(result))
//		{ std::cerr << "Error in ImageOperations::sub: Given image dimensions do not agree!" << std::endl; return -1; }

	result.init( src.width(), src.height() );

	cvConvertScale( src.mpImage, result.mpImage, scale, shift );

	return 1;
} // end convert

// LOGIC:

template <class T>
/*static*/ inline
int
ImageOperations::andOp(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::sub: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvAnd(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end andOp

template <class T>
/*static*/ inline
int
ImageOperations::andScalarOp(const GrayImage<T>& src1, T value, GrayImage<T>& result)
{
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvAndS(src1.mpImage, cvRealScalar( value ), result.mpImage);

	return 1;
} // end andScalarOp

template <class T>
/*static*/ inline
int
ImageOperations::orOp(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::sub: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvOr(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end orOp

template <class T>
/*static*/ inline
int
ImageOperations::orScalarOp(const GrayImage<T>& src1, T value, GrayImage<T>& result)
{
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvOrS(src1.mpImage, cvRealScalar( value ), result.mpImage);

	return 1;
} // end orScalarOp

template <class T>
/*static*/ inline
int
ImageOperations::xorOp(const GrayImage<T>& src1, const GrayImage<T>& src2, GrayImage<T>& result)
{
	if (!src1.hasEqualSizeRoi(src2))
		{ std::cerr << "Error in ImageOperations::sub: Given image dimensions do not agree!" << std::endl; return -1; }
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvXor(src1.mpImage, src2.mpImage, result.mpImage);

	return 1;
} // end xorOp

template <class T>
/*static*/ inline
int
ImageOperations::xorScalarOp(const GrayImage<T>& src1, T value, GrayImage<T>& result)
{
	result.init( src1.widthRoi(), src1.heightRoi() );
	cvXorS(src1.mpImage, cvRealScalar( value ), result.mpImage);

	return 1;
} // end xorScalarOp

template <class T>
/*static*/ inline
int
ImageOperations::notOp(const GrayImage<T>& src, GrayImage<T>& result)
{
	result.init( src.widthRoi(), src.heightRoi() );
	cvNot(src.mpImage, result.mpImage);

	return 1;
} // end inversion

// STATISTICS:

template <class T>
/*static*/ inline
int
ImageOperations::countNonZero( const GrayImage<T>& src )
{
	return cvCountNonZero(src.mpImage);
} // end countNonZero

template <class T>
/*static*/ inline
double
ImageOperations::sum( const GrayImage<T>& src )
{
	return (cvSum( src.mpImage )).val[0];
} // end sum

template <class T>
/*static*/ inline
double
ImageOperations::avg( const GrayImage<T>& src )
{
	return (cvAvg( src.mpImage )).val[0];
} // end avg

} // end of namespace impact_tr4

#endif /*IMAGEOPERATIONS_HPP_*/
