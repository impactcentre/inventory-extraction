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
#ifndef IMAGEITERATOR_HPP_
#define IMAGEITERATOR_HPP_

#include <iostream>

#include "core/Exception.hpp"
#include "core/Index.hpp"
#include "GrayImage.hpp"

namespace impact_tr4 {

template <class T> class ImageIterator;
template <class T> class GrayImage;

/**
 * Iterator class for the GrayImage class.
 * The iterator is inherited from the std iterator
 * class, implementing a forward iterator. All common operations for this type of iterator
 * are implemented.
 * 
 * */
template <class T = uint8>
class ImageIterator : public Index, std::iterator<std::forward_iterator_tag, uchar>
{
public:
	ImageIterator(GrayImage<T>* pImage, const int row=0, const int col=0)
		: Index(row,col), mpImage(pImage)
		{};

		ImageIterator(const ImageIterator& source)
		{
			this->mpImage = source.mpImage;
			this->setRow(source.row());
			this->setColumn(source.column());
		};
		~ImageIterator() { this->mpImage = 0x0; };

	    /** Returns true if two vectors are equal. */
	    bool operator==(const ImageIterator &rhs) const {
	    	return
	    		(this->row() == rhs.row() && this->column() == rhs.column())
	    		&&
	    		(this->mpImage == rhs.mpImage);
	      	}
	    /** Returns true if two vectors are not equal. */
	    bool operator!=(const ImageIterator& other) {
	               return !( (*this) == other );
	               }

	    /** The pre-increment operator. Note: Faster than post-increment! */
	    ImageIterator& operator++();
	    /** The post-increment operator. */
	    ImageIterator operator++(int);
	    /** The dereferenciation operator. */
		T operator*();

//		uchar* operator->();

private:
	GrayImage<T>* mpImage;

}; // end of class ImageIterator

//template <class T>
//ImageIterator<T>& ImageIterator<T>::operator++()
//{
//	int c = this->column();
//	int r = this->row();
//
//	 if ( c < (mpImage->columns()-1) ) { // increment cols if not exceeded
//		 this->setColumn(c+1);
//	 }
//	 else if ( r < (mpImage->rows()-1) ) { // if cols are exceeded and rows not -> step to next row
//		 this->setColumn(0);
//		 this->setRow(r+1);
//	 }
//	 else { // set to end if rows/cols are exceeded:
//		 this->setColumn(mpImage->columns());
//		 this->setRow(mpImage->rows()-1);
//	 }
//	 return(*this);
//
//} // end operator++

template <class T>
ImageIterator<T>& ImageIterator<T>::operator++()
{
	int c = this->column();
	int r = this->row();

	 if ( c < (mpImage->xRoi()+mpImage->columnsRoi()-1) ) { // increment cols if not exceeded
		 this->setColumn(c+1);
	 }
	 else if ( r < (mpImage->yRoi()+mpImage->rowsRoi()-1) ) { // if cols are exceeded and rows not -> step to next row
		 this->setColumn( mpImage->xRoi() );
		 this->setRow(r+1);
	 }
	 else { // set to end if rows/cols are exceeded:
		 this->setColumn(mpImage->columnsRoi());
		 this->setRow(mpImage->rowsRoi()-1);
	 }
	 return(*this);

} // end operator++

template <class T>
ImageIterator<T> ImageIterator<T>::operator++(int)
{
	ImageIterator tmp(*this);
    ++(*this);
    return (tmp);
}

template <class T>
T ImageIterator<T>::operator*()
{
//	assert(mpImage->isValidIndex(this->row(), this->column()));
	Assert(!ARG_CHECK || mpImage->isValidIndex(this->row(), this->column()), IndexOutOfBoundsException("IndexOutOfBoundsException in operator*() of ImageIterator.cpp!"));

	return (mpImage->get( this->row(), this->column()) );
} // end operator*

//template <class T>
//T* ImageIterator<T>::operator->()
//{
//   return(&*(ImageIterator)*this);
//}

} // end of namespace impact_tr4

#endif /*IMAGEINDEX_HPP_*/
