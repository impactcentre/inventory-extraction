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
#ifndef INDEX_HPP_
#define INDEX_HPP_

// OpenCV includes:
#include <cv.h>
//#include <highgui.h>

#include "Vector2D.hpp"

namespace impact_tr4 {

/**
 * This class represents a two dimensional index that can be used for example
 * in the GrayImage class. Index inherits from a two dimensional vector and adds some
 * convenience functions for 2D-indices.
 * Its x-entry represents the row, while the y-entry specifies the column of the index.
 * */
class Index : public Vector2i
{
public:
	/** Constructs default index, i.e. (0,0). */
	inline Index() : Vector2i(0, 0) {}

    /** Initializes the index pair from a given Vector2D. */
    inline Index(const Vector2i& vector) : Vector2i(vector(0), vector(1)) {}

    /** Initializes the index pair from a given Vector2D with float entries.
     * The input values are floored, i.e. rounded to the next integer lower or
     * equal than the input.
     * */
    inline Index(const Vector2f& vector) :
    	Vector2i(int(floor(vector(0))), int(floor(vector(1)))) {}

    /** Initializes the index pair to (r, c). */
    inline Index(const int r, const int c) : Vector2i(r, c) {}

    /** Converts this index to a CvPoint structure - note that row and col indices are swapped! */
    inline CvPoint toCvPoint() const
    	{ return cvPoint((*this)(1), (*this)(0)); }

    /** Get the row index. */
    inline int row() const { return (*this)(0); }

    /** Get the column index. */
    inline int column() const { return (*this)(1); }

    /** Set values. */
    inline void set(const int row, const int column)
    	{ (*this)(0) = row; (*this)(1) = column; }

    /** Set row index */
    inline void setRow(const int row) { (*this)(0) = row; }

    /** Set column index. */
    inline void setColumn(const int column) { (*this)(1) = column; }

    /** Returns true if the index points to an element in the diagonal. */
    inline int isDiagonal() const { return (*this)(0) == (*this)(1); }

    /** Swaps first and second entry of the index. */
    inline Index transpose() const { return Index((*this)(1), (*this)(0)); }

    /** Returns true if the index points to an element in the upper triangle. */
    inline int isInUpperTriangle() const { return (*this)(0) <= (*this)(1); }

//    /** Addition of two index pairs using the operator '+'. */
//    inline Index operator+(const Index &rhs) const
//      { return Vector2i::operator+(rhs); };
//
//    /** Subtraction of two index pairs using the operator '-'. */
//    inline Index operator-(const Index &rhs) const
//      { return Vector2i::operator-(rhs); };

    /** Returns true if two vectors are equal. */
    inline bool operator==(const Index &rhs) const
      { return (*this)(0) == rhs(0) && (*this)(1) == rhs(1); };

}; // end of class Index

} // end of namespace impact_tr4

#endif /*INDEX_HPP_*/
