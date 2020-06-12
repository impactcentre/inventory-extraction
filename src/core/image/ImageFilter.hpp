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
#ifndef IMAGEFILTER_HPP_
#define IMAGEFILTER_HPP_

// System includes:
#include <iostream>

// Local includes:
#include "GrayImage.hpp"
#include "Kernel.hpp"
#include "ImageOperations.hpp"
#include "types.h"

namespace impact_tr4 {

//template <class T> class GrayImage;
//template <class T> class Kernel;

/**
 * Class representing filtering operations for images.
 * @see GrayImage, Morhology, ImageOperations
 * 
 * */
//template <class T = uint8>
class ImageFilter
{
public:
	/** Performs thresholding of this image, using a specified method.
	 * @param[in] src The source image. 8-bit of 32-bit floating point.
	 * @param[out] result The resulting image. Either the same type as src or 8-bit.
	 * @param[in] threshold The threshold value, default = 128.
	 * @param[in] maxValue The maximum value (used for binary thresholing only), default = 255.
	 * @param[in] thresholdType The tresholding type, default = CV_THRESH_BINARY.
	 * 	thresholdType=THRESH_BINARY:
		result(x,y) = maxValue, if src(x,y)>threshold
		           0, otherwise

		thresholdType=THRESH_BINARY_INV:
		result(x,y) = 0, if src(x,y)>threshold
		           maxValue, otherwise

		thresholdType=THRESH_TRUNC:
		result(x,y) = threshold, if src(x,y)>threshold
		           src(x,y), otherwise

		thresholdType=THRESH_TOZERO:
		result(x,y) = src(x,y), if src(x,y)>threshold
		           0, otherwise

		thresholdType=THRESH_TOZERO_INV:
		result(x,y) = 0, if src(x,y)>threshold
		           src(x,y), otherwise
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int threshold(const GrayImage<T>& src, GrayImage<T>& result, double threshold=128,
            double maxValue=255, ThreshType thresholdType=THRESH_BINARY);


	/**
	 * Binarizes the given image using adaptive thresholding.
	 * The function transforms grayscale images to binary images according to the formulae:
		thresholdType=CV_THRESH_BINARY:
		result(x,y) = max_value, if src(x,y)>T(x,y)
		           0, otherwise

		thresholdType=CV_THRESH_BINARY_INV:
		result(x,y) = 0, if src(x,y)>T(x,y)
		           max_value, otherwise

		where T is a threshold calculated individually for each pixel.
		For the method CV_ADAPTIVE_THRESH_MEAN_C it is a mean of blockSize x blockSize pixel neighborhood, subtracted by param1.
		For the method CV_ADAPTIVE_THRESH_GAUSSIAN_C it is a weighted sum (gaussian) of blockSize x blockSize pixel neighborhood, subtracted by param1.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] adaptiveMethod The adaptive thresholding method, default = CV_ADAPTIVE_THRESH_MEAN_C.
	 * @param[in] thresholdType The type of threshold, default = CV_THRESH_BINARY.
	 * @param[in] blockSize The block size for the adaptive thresholding, default = 3.
	 * @param[in] param1 The value, substracted from the calculated threshold at the and, default = 5.
	 * */
	template <class T>
	static int adaptiveThreshold(const GrayImage<T>& src, GrayImage<T>& result, double maxValue=255, int adaptiveMethod=CV_ADAPTIVE_THRESH_MEAN_C,
	        int thresholdType=CV_THRESH_BINARY,
	        int blockSize=3, double param1=5);

	/** Filters this image with a gaussian kernel.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] sizeX Size of the gauss kernel in x-direction
	 * @param[in] sizeY Size of the gauss kernel in y-direction
	 * @param[in] sigmaHor Standard deviation of gauss kernel in x-direction (default = 0 sets sigmaHor = (sizeX/2 - 1)*0.3 + 0.8)
	 * @param[in] sigmaVer Standard deviation of gauss kernel in x-direction (default = 0 sets sigmaVer = (sizeY/2 - 1)*0.3 + 0.8)
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int gaussian(const GrayImage<T>& src, GrayImage<T>& result, int sizeX = 3, int sizeY = 3,
										double sigmaHor = 0, double sigmaVer = 0);

	/** Filters this image with size x size mask using the median filter.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] size The size of the kernel.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int median(const GrayImage<T>& src, GrayImage<T>& result,	int size = 3);

	/**
	 * Computes the sobel of this image.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] xOrder Order of the x-derivative.
	 * @param[in] yOrder Order of the y-derivative.
	 * @param[in] apertureSize Size of kernel; must be 1, 3, 5 or 7.
	 * The special value CV_SCHARR (=-1) can be used to apply the Scharr filter. Default = 3.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T1, class T2>
	static int sobel(const GrayImage<T1>& src, GrayImage<T2>& result, int xOrder, int yOrder, int apertureSize=3);

	/**
	 * Computes the laplacian of this image.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] apertureSize Size of kernel; must be 1, 3, 5 or 7. Default = 3.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int laplace(const GrayImage<T>& src, GrayImage<T>& result, int apertureSize = 3);

	/**
	 * Detects edges in this image using the canny algorithm.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] thresh1 The first threshold for the canny algorithm.
	 * @param[in] thresh1 The second threshold for the canny algorithm.
	 * @param[in] apertureSize Size of the kernel. Default = 3.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int canny(const GrayImage<T>& src, GrayImage<T>& result, double thresh1, double thresh2, int apertureSize = 3);

	/**
	 * Filters the image with an arbitrary kernel.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] kernel The given kernel object.
	 * @return 1 for success, 0 for failure.
	 * */
//	template <class T>
	template <class T1, class T2>
	static int filter2D(const GrayImage<T1>& src, GrayImage<T2>& result, const Kernel<float>& kernel);

	static int distTransform(const GrayImage<uint8>& src, GrayImage<float>& result, int disType=CV_DIST_L2, int maskSize=3, float *mask=0 );
}; // end class ImageFilter

/** Abbreviation of the ImageFilter class. */
typedef ImageFilter ImFi;

template <class T>
/*static*/ inline
int
ImageFilter::threshold(const GrayImage<T>& src, GrayImage<T>& result, double threshold/*=128*/,
        double maxValue/*=255*/, ThreshType thresholdType/*=THRESH_BINARY*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
	if (&src != &result) { // if the two images are not the same, i.e. in-place operation, allocate memory for result image
		result.init(src.widthRoi(), src.heightRoi());
	}

	cvThreshold(src.mpImage, result.mpImage, threshold, maxValue, thresholdType);
	return 1;
} // end threshold

template <class T>
/*static*/ inline
int
ImageFilter::adaptiveThreshold(	const GrayImage<T>& src, GrayImage<T>& result,
								double maxValue/*=255*/,
								int adaptiveMethod/*=CV_ADAPTIVE_THRESH_MEAN_C*/,
								int thresholdType/*=CV_THRESH_BINARY*/,
								int blockSize/*=3*/, double param1/*=5*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
//	if ()
	result.init(src.widthRoi(), src.heightRoi());

	cvAdaptiveThreshold(src.mpImage, result.mpImage, maxValue, adaptiveMethod, thresholdType, blockSize, param1);
	return 1;
} // end threshold

template <class T>
/*static*/ inline
int
ImageFilter::gaussian(	const GrayImage<T>& src, GrayImage<T>& result,
						int sizeX /*= 3*/, int sizeY /*= 3*/,
						double sigmaHor /*= 0*/, double sigmaVer /*= 0*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	cvSmooth(src.mpImage, result.mpImage, CV_GAUSSIAN, sizeX, sizeY, sigmaHor, sigmaVer);
	return 1;
} // end gaussian

template <class T>
/*static*/ inline
int
ImageFilter::median(const GrayImage<T>& src, GrayImage<T>& result, int size/*= 3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	cvSmooth(src.mpImage, result.mpImage, CV_MEDIAN, size);
	return 1;
} // end median

template <class T1, class T2>
/*static*/ inline
int
ImageFilter::sobel(const GrayImage<T1>& src, GrayImage<T2>& result, int xOrder, int yOrder, int apertureSize /*= 3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi()/*, DEPTH_16U*/);

//	IplImage *pTempIplImage = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), IPL_DEPTH_16S, 1 );

	cvSobel( src.mpImage, result.mpImage, xOrder, yOrder, apertureSize );
//	cvConvert(pTempIplImage, result.mpImage); // convert back to unsigned char format
//	cvReleaseImage(&pTempIplImage);
	return 1;
} // end sobel

template <class T>
/*static*/ inline
int
ImageFilter::laplace(const GrayImage<T>& src, GrayImage<T>& result, int apertureSize /*= 3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi()/*, DEPTH_16U*/);
//	IplImage *pTempIplImage = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), IPL_DEPTH_16S, 1 );

	cvLaplace( src.mpImage, result.mpImage, apertureSize );
//	cvConvert(pTempIplImage, result.mpImage); // convert back to unsigned char format
//	cvReleaseImage(&pTempIplImage);
	return 1;
} // end laplace

template <class T>
/*static*/ inline
int
ImageFilter::canny(const GrayImage<T>& src, GrayImage<T>& result, double thresh1, double thresh2, int apertureSize /*= 3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	cvCanny(src.mpImage, result.mpImage, thresh1, thresh2, apertureSize);
	return 1;
} // end canny

template <class T1, class T2>
/*static*/ inline
int
ImageFilter::filter2D(const GrayImage<T1>& src, GrayImage<T2>& result, const Kernel<float>& kernel)
{
	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}

	GrayImage<T2> src2(src.widthRoi(), src.heightRoi() );
	ImOp::convert<T1, T2>(src, src2);

	result.init( src.widthRoi(), src.heightRoi()/*, DEPTH_16U*/ );
	CvMat kernelMatHdr;
	CvMat *kernelMat = cvGetMat( kernel.mpImage, &kernelMatHdr );
//	CvMat *kernelMat = cvGetMat( kernel.iplImage(), &kernelMatHdr );

//	CvMat *kernelMatTmp = cvCreateMat(src.widthRoi(), src. heightRoi(), CV_32F /*CV_8UC1*/);
//	CvMat *kernelMat = 0x0;
//	kernelMat = cvGetMat(kernel.mpImage, kernelMatTmp);

	CvPoint anchor = cvPoint(kernel.mAnchor(0), kernel.mAnchor(1));
	cvFilter2D( src2.mpImage, result.mpImage, kernelMat, anchor );

//	cvReleaseMat( &kernelMat );

//	IplImage *pTempIplImageSrc = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), IPL_DEPTH_16S, 1 );
//	cvConvert(src.mpImage, pTempIplImageSrc);
//	IplImage *pTempIplImage = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), IPL_DEPTH_16S, 1 );
//	CvPoint anchor = cvPoint(kernel.mAnchor(0), kernel.mAnchor(1));
////	std::cout << anchor.x << ", " << anchor.y << std::endl;
//	CvMat *kernelMat=cvCreateMat(src.widthRoi(), src. heightRoi(), CV_8UC1);
//	kernelMat = cvGetMat(kernel.mpImage, kernelMat);
//	cvFilter2D( pTempIplImageSrc /*src.mpImage*/, pTempIplImage /*result.mpImage*/, kernelMat, anchor );
//	cvConvert( pTempIplImage, result.mpImage );
//
//	cvReleaseImage( &pTempIplImageSrc );
//	cvReleaseImage( &pTempIplImage );

	return 1;
} // end filter2D


} // end of namespace impact_tr4

#endif /*IMAGEFILTER_HPP_*/
