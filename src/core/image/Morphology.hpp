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
#ifndef MORPHOLOGY_HPP_
#define MORPHOLOGY_HPP_

// System includes:
#include <iostream>
// OpenCV includes:
#include <cv.h>

// Local includes:
#include "GrayImage.hpp"

namespace impact_tr4 {

class StructureElement;
class Morphology;

/**
 * This class represents a structuring element used for morphological operations.
 * It can be used in a dynamic as well as static manner (by calling the createStructureElement).
 * Moreover, some basic structuring elements are predifined by a set of static const class members.
 * This class is intended to be used in combination with the functions that are defined in the
 * Morphology class.
 * */
class StructureElement
{
public:
	/**
	 * Creates a new structure element.
	 * @param shape Defines the shape of the structure element - can be one of the following values:
	 * 		CV_SHAPE_RECT, a rectangular element;
	 * 		CV_SHAPE_CROSS, a cross-shaped element;
	 *		CV_SHAPE_ELLIPSE, an elliptic element;
	 * 		CV_SHAPE_CUSTOM, a user-defined element. In this case the parameter values specifies the mask, that is, which neighbors of the pixel must be considered.
	 * @param width The width of the structure element. Default = 3.
	 * @param height The height of the structure element. Default = 3.
	 * @param anchor_x The x-value of the anchor point. If -1, anchor_x is set to (width-1)/2. Default = -1.
	 * @param anchor_y The y-value of the anchor point. If -1, anchor_y is set to (height-1)/2. Default = -1.
	 * @param values The values for this structure element as rowwise array. Is only used if shape = CV_SHAPE_CUSTOM. Default = 0x0.
	 * */
	StructureElement(const int shape, const int width=3, const int height=3,
	const int anchor_x=-1, const int anchor_y=-1, int* values=0x0)
	{
	// if anchor is not set (i.e. -1), set them to middle of respective direction:
	const int a_x = (anchor_x < 0) ? (width-1)/2 : anchor_x;
	const int a_y = (anchor_y < 0) ? (height-1)/2 : anchor_y;

	mpStructureElement = cvCreateStructuringElementEx( width, height, a_x, a_y,
	                             shape, values );
	} // end constructor

	/** Destructor. */
	~StructureElement()
	{
		deleteStructureElement();
	}

	/**
	 * Static function to create a new structure element.
	 * Parameters have the same meaning as for the constructor with the same signature.
	 * @return The resulting structuring element.
	 * */
	static const StructureElement createStructureElement(const int shape, const int width=3, const int height=3,
	const int anchor_x=-1, const int anchor_y=-1, int* values=0x0)
	{
		StructureElement *se = new StructureElement(shape, width, height, anchor_x, anchor_y, values);
		return *se;
	} // end createStructureElement

	// Some basic structuring elements:
	/** 3x3 structure element with rectangle shape and origin = (1,1). */
	static const StructureElement RECT_SE_3x3;
	/** 5x5 structure element with rectangle shape and origin = (2,2). */
	static const StructureElement RECT_SE_5x5;
	/** 7x7 structure element with rectangle shape and origin = (3,3). */
	static const StructureElement RECT_SE_7x7;

	/** 3x3 structure element with cross shape and origin = (1,1). */
	static const StructureElement CROSS_SE_3x3;
	/** 5x5 structure element with cross shape and origin = (2,2). */
	static const StructureElement CROSS_SE_5x5;
	/** 7x7 structure element with cross shape and origin = (3,3). */
	static const StructureElement CROSS_SE_7x7;

	/** 3x3 structure element with circle shape and origin = (1,1). */
	static const StructureElement CIRCLE_SE_3x3;
	/** 5x5 structure element with circle shape and origin = (2,2). */
	static const StructureElement CIRCLE_SE_5x5;
	/** 7x7 structure element with circle shape and origin = (3,3). */
	static const StructureElement CIRCLE_SE_7x7;

	// Friends:
	friend class Morphology;

private:
	/** Deletes the structure element in this class and sets the pointer to it to 0x0. */
	void deleteStructureElement()
	{
		if (mpStructureElement!=0x0) {
			cvReleaseStructuringElement( &mpStructureElement );
			mpStructureElement=0x0;
		}
	} // end deleteStructureElement()

	IplConvKernel* mpStructureElement;
}; // end structure element

/** Abbreviation for the StructureElement class. */
typedef StructureElement SE;

/**
 * Class representing morphological operations (basically erode, dilate, open and close).
 * @see ImageOperations, GrayImage, ImageFilter
 * 
 * */
class Morphology
{
public:
	/**
	 * Erodes this image with the current structure element nIterations times.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] nIterations The number of iterations, default = 1.
	 * @param[in] structureElement The structure element, default = 3x3 circle.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int erode(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations = 1, const SE& structureElement = SE::CIRCLE_SE_3x3);

	/**
	 * Dilates this image with the current structure element nIterations times.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] nIterations The number of iterations, default = 1.
	 * @param[in] structureElement The structure element, default = 3x3 circle.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int dilate(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations = 1, const SE& structureElement = SE::CIRCLE_SE_3x3);

	/**
	 * Performs opening of this image. nIterations spefifies the nr. of times the image is eroded or dilated.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] nIterations The number of iterations, default = 1.
	 * @param[in] structureElement The structure element, default = 3x3 circle.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int open(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations = 1, const SE& structureElement = SE::CIRCLE_SE_3x3);

	/**
	 * Performs closing of this image. nIterations spefifies the nr. of times the image is eroded or dilated.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] nIterations The number of iterations, default = 1.
	 * @param[in] structureElement The structure element, default = 3x3 circle.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int close(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations = 1, const SE& structureElement = SE::CIRCLE_SE_3x3);

	/**
	 * Calculates the morphological gradient of this image. nIterations spefifies the nr. of times the image is eroded or dilated.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] nIterations The number of iterations, default = 1.
	 * @param[in] structureElement The structure element, default = 3x3 circle.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int gradient(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations = 1, const SE& structureElement = SE::CIRCLE_SE_3x3);

	/**
	 * Calculates the top hat of this image. nIterations spefifies the nr. of times the image is eroded or dilated.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] nIterations The number of iterations, default = 1.
	 * @param[in] structureElement The structure element, default = 3x3 circle.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int topHat(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations = 1, const SE& structureElement = SE::CIRCLE_SE_3x3);

	/**
	 * Calculates the black hat of this image. nIterations spefifies the nr. of times the image is eroded or dilated.
	 * @param[in] src The source image.
	 * @param[out] result The resulting image.
	 * @param[in] nIterations The number of iterations, default = 1.
	 * @param[in] structureElement The structure element, default = 3x3 circle.
	 * @return 1 for success, 0 for failure.
	 * */
	template <class T>
	static int blackHat(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations = 1, const SE& structureElement = SE::CIRCLE_SE_3x3);
}; // end class Morphology

/** Abbreviation for the Morphology class. */
typedef Morphology Morph;

template <class T>
/*static*/ inline
int
Morphology::erode(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations /*= 1*/, const SE& structureElement /*= SE::CIRCLE_SE_3x3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The image is empty - returning 0x0 pointer!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	cvErode(src.mpImage, result.mpImage, structureElement.mpStructureElement, nIterations);
	return 1;
} // end erode

template <class T>
/*static*/ inline
int
Morphology::dilate(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations /*= 1*/, const SE& structureElement /*= SE::CIRCLE_SE_3x3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The image is empty - returning 0x0 pointer!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	cvDilate(src.mpImage, result.mpImage, structureElement.mpStructureElement, nIterations);
	return 1;
} // end dilate

template <class T>
/*static*/ inline
int
Morphology::open(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations /*= 1*/, const SE& structureElement /*= SE::CIRCLE_SE_3x3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The image is empty - returning 0x0 pointer!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	IplImage *temp = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), src.determineDepth(), 1 );
	cvMorphologyEx( src.mpImage, result.mpImage, temp,
			structureElement.mpStructureElement, CV_MOP_OPEN, nIterations );
	cvReleaseImage(&temp);
	return 1;
} // end open

template <class T>
/*static*/ inline
int
Morphology::close(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations /*= 1*/, const SE& structureElement /*= SE::CIRCLE_SE_3x3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The image is empty - returning 0x0 pointer!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	IplImage *temp = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), src.determineDepth(), 1 );
	cvMorphologyEx( src.mpImage, result.mpImage, temp,
			structureElement.mpStructureElement, CV_MOP_CLOSE, nIterations );
	cvReleaseImage(&temp);
	return 1;
} // end close

template <class T>
/*static*/ inline
int
Morphology::gradient(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations /*= 1*/, const SE& structureElement /*= SE::CIRCLE_SE_3x3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The image is empty - returning 0x0 pointer!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	IplImage *temp = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), src.determineDepth(), 1 );
	cvMorphologyEx( src.mpImage, result.mpImage, temp,
			structureElement.mpStructureElement, CV_MOP_GRADIENT, nIterations );
	cvReleaseImage(&temp);
	return 1;
} // end gradient

template <class T>
/*static*/ inline
int
Morphology::topHat(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations /*= 1*/, const SE& structureElement /*= SE::CIRCLE_SE_3x3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The image is empty - returning 0x0 pointer!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	IplImage *temp = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), src.determineDepth(), 1 );
	cvMorphologyEx( src.mpImage, result.mpImage, temp,
			structureElement.mpStructureElement, CV_MOP_TOPHAT, nIterations );
	cvReleaseImage(&temp);
	return 1;
} // end topHat

template <class T>
/*static*/ inline
int
Morphology::blackHat(const GrayImage<T>& src, GrayImage<T>& result, const int nIterations /*= 1*/, const SE& structureElement /*= SE::CIRCLE_SE_3x3*/)
{
	if (src.empty()) {
		std::cerr << "Warning: The image is empty - returning 0x0 pointer!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());

	IplImage *temp = cvCreateImage( cvSize(src.widthRoi(), src.heightRoi()), src.determineDepth(), 1 );
	cvMorphologyEx( src.mpImage, result.mpImage, temp,
			structureElement.mpStructureElement, CV_MOP_BLACKHAT, nIterations );
	cvReleaseImage(&temp);
	return 1;
} // end blackHat

} // end of namespace impact_tr4

#endif /*MORPHOLOGY_HPP_*/
