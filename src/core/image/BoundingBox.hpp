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
#ifndef BOUNDINGBOX_HPP_
#define BOUNDINGBOX_HPP_

// SYSTEM INCLUDES
#include <iostream>

// PROJECT INCLUDES
#include "core/core.h"
#include "core/Vector2D.hpp"
#include "core/Exception.hpp"

// OpenCV includes:
#include <cv.h>
#include <highgui.h>

namespace impact_tr4 {

template <class T> class GrayImage;
class BoundingBox;
inline std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox);

/**
 * This class represents a bounding box used for images.
 * The bounding box is created using an anchor point (x,y) and a rectangle specified by a width and height.
 * It can be set as a region of interest for a GrayImage object. After setting the region of
 * interest, all operations with the image are only performed on this area. At the end,
 * the bounding box should be released, to perform operations on the whole image again.
 * The following example will illustrate the basic usage of this class:
 *
 * @code
 * GrayImage image("test_image.jpg"); // load a test image
 * int x = 10;
 * int y = 15;
 * int width = 20;
 * int height = 40;
 * BoundingBox bbox(x, y, width, height); // create the bounding box with specified parameters
 * image.setRoi(bbox); // set the bounding box as region of interest for the image
 * // perform some operation here, e.g.:
 * image.save("test_roi_image.jpg"); // only saves the image inside the specified bounding box!
 * image.releaseRoi(); // release the region of interest
 * @endcode
 *
 * 
 * */
class BoundingBox
{
public:
	/** Creates an empty bounding box. */
	BoundingBox() { setEmpty(); };

	/** Destructor. */
	~BoundingBox() { setEmpty(); };

	/** The copy-constructor. */
	BoundingBox(const BoundingBox& source) { init(source); }

	/**
	 * Constructor inits the boudning box with pointer to image and left-, top-, bottom- and rightmost
	 * pixel values respectively.
	 * @param x The x-value of the bbox.
	 * @param y The y-value of the bbox.
	 * @param width The width value of the bbox.
	 * @param height The height value of the bbox.
	 * */
	BoundingBox(const int x, const int y, const int width, const int height)
	{
		mRoi.x = x; mRoi.y = y;
		mRoi.width = width; mRoi.height = height;
		checkDimensions();
	}

	bool checkDimensions()
	{
		if (mRoi.x < 0 || mRoi.y < 0 || mRoi.width <= 0 || mRoi.height <= 0) {
			std::cerr << "Warning: dimensions of BoundingBox not specified correctly - setting empty box!" << std::endl;
			std::cerr << *this << std::endl;
			setEmpty();
			return false;
		}
		return true;
	} // end checkDimensions()

	/** Init method to copy bounding box. */
	BoundingBox& init(const BoundingBox& source)
	{
		mRoi = source.mRoi;
		return *this;
	}

	/** Sets this to an empty box. */
	void setEmpty()
	{
	mRoi.x = 0; mRoi.y = 0;
	mRoi.width = 0; mRoi.height = 0;
	}

	// VALIDATION:
	/** Returns true, if the box is empty, i.e. its size is zero. */
	bool empty() const { return (mRoi.width<=0 || mRoi.height<=0); }

	// ACCESSORS:
	const int x() const { return mRoi.x; }
	const int y() const { return mRoi.y; }
	void setX(int x) { mRoi.x = x; }
	void setY(int y) { mRoi.y = y; }
	Index leftUpperIndex() const { return Index(mRoi.y, mRoi.x); }
	Index rightLowerIndex() const { return Index(mRoi.y+mRoi.height, mRoi.x+mRoi.width); }

	const int width() const { return mRoi.width; }
	const int columns() const { return mRoi.width; }
	const int height() const { return mRoi.height; }
	const int rows() const { return mRoi.height; }
	void setWidth(int width) { mRoi.width = width; }
	void setHeight(int height) { mRoi.width = height; }

	// OPERATIONS:
	/** Saves the image, that is covered by this bounding box.
	 * @param[in] image The image, where the bounding box refers to.
	 * @param[in] fileName The filename for saving the image.
	 * */
	template <class T>
	void saveImage(GrayImage<T>& image, const std::string fileName);

	const BoundingBox& operator=(const BoundingBox& rhs)
	{
		if (this != &rhs) {
			this->mRoi = rhs.mRoi;
		}
		return *this;
	}

	// Friends:
	template <class T> friend class GrayImage;
private:
	// MEMBERS:
	CvRect mRoi;
};

template <class T>
void BoundingBox::saveImage(GrayImage<T>& image, const std::string fileName)
{
	image.setRoi(*this);
	image.save(fileName);
	image.releaseRoi();
} // end saveImage

/** Abbreviation for the BoundingBox class. */
typedef BoundingBox BBox;

/** Output operator for bounding boxes. */
inline std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox)
{
	os << "Image-Bounding-Box: (x,y) = "
		<< bbox.x() << ", " << bbox.y()
		<< "; (width,height) = " << bbox.width() << ", " << bbox.height() << "\n";

	return os;
}

} // end of namespace impact_tr4

#endif /*BOUNDINGBOX_HPP_*/

