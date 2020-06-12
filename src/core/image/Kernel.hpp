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
#ifndef KERNEL_HPP_
#define KERNEL_HPP_


#include "GrayImage.hpp"
#include "core/Vector2D.hpp"

namespace impact_tr4 {

// Forward References:
//template <class T1> class GrayImage;
//template <class T1> class Kernel;

/**
 * The Kernel class represents a 2D-kernel used for convolution methods.
 * It is inherited from the GrayImage class and adds an anchor object for
 * the filtering methods. The anchor must lie within the image and the special
 * value (-1,-1) means, that the center of the image is taken as anchor.
 *
 * 
 */
template <class T1=float>
//template <class T1>
class Kernel : public GrayImage<T1>
{
public:
	// CONSTRUCTORS:
	Kernel()
	: GrayImage<T1>()
	{
//		std::cout << "empty constructor kernel" << std::endl;
		mAnchor.set(-1,-1); // set anchor to (-1,-1). i.e. the center of the image
	}

	/** The copy-constructor. */
	Kernel(const Kernel& source)
	: GrayImage<T1>(source)
	{
//		std::cout << "kernel copy-constructor" << std::endl;
//		mpImage = 0x0;
//		this->init(source);
		mAnchor = source.mAnchor;
	}

	Kernel(const int width, const int height, const Index& anchor = Index(-1,-1))
	: GrayImage<T1>(width, height)
	{
		if ( this->isValidIndex(anchor) || anchor == Index(-1,-1) ) {
			mAnchor = anchor;
		}
		else {
			throw IndexOutOfBoundsException("Anchor not inside image in constructor Kernel(width, height, anchor)!");
		}
	} // end constructor

	~Kernel()
	{
		this->releaseImage();
	}

	// INTIALIZATION:

	// VALIDATION:


	// ACCESS:
	const Index& anchor() const { return mAnchor; }
	void setAnchor(const Vector2i& anchor) { mAnchor = anchor; }

	// OPERATORS:
	Kernel& operator=(const Kernel& rhs)
	{
	    if (this != &rhs)
	    {
//			GrayImage<T1>::init(rhs);
	        GrayImage<T1>::operator=(rhs);
	        mAnchor = rhs.mAnchor;
	    }
	    return *this;
	}

	// METHODS:
	/**
	 * Static function to create a new spherical Kernel object with given radius.
	 * @param mask [out] The resulting spherical mask.
	 * @param radius The radius of the spherical kernel.
	 * @param oddSize If true, the resulting kernel has odd size.
	 * */
	static void createSphereKernel(Kernel &mask, const int radius = 2, const bool oddSize = true);

	static void createCircleKernel(Kernel &mask, const int radius = 2, const bool oddSize = true);

	// FRIENDS:
	friend class ImageFilter;

private:
	// MEMBERS:
	Index mAnchor;

}; // end of class Kernel

template <class T1>
/*static*/
void
Kernel<T1>::createSphereKernel(Kernel<T1> &mask, const int radius /*= 3*/, const bool oddSize /*= true*/)
{
	int size=0; double l_bound=0;
	if (oddSize) {
		size = 2*radius+1;
		l_bound = -radius;
	}
	else {
		size = 2*radius;
		l_bound = -radius+0.5;
	}
	mask.init(size, size);
	double x = l_bound, y = l_bound;
	for (int i=0; i<size; ++i) {
		for (int j=0; j<size; ++j) {
			if ( (x*x + y*y) <= radius*radius )
//			if ( (x*x + y*y) == radius*radius )
				mask.set(i,j,1);
//				mask(i,j) = 1; // white inside
			else
				mask.set(i,j,0);
//				mask(i,j) = 0; // black outside
			++x;
		}
		y += 1; x = l_bound;
	}
	return;
} // end createSphereKernel

template <class T1>
/*static*/
void
Kernel<T1>::createCircleKernel(Kernel<T1> &mask, const int radius /*= 3*/, const bool oddSize /*= true*/)
{
	int size=0; double l_bound=0;
	if (oddSize) {
		size = 2*radius+1;
		l_bound = -radius;
	}
	else {
		size = 2*radius;
		l_bound = -radius+0.5;
	}
	mask.init(size, size);
	int radSqu = radius*radius;
	double x = l_bound, y = l_bound;
	for (int i=0; i<size; ++i) {
		for (int j=0; j<size; ++j) {
			int tmp = x*x + y*y;
//			if ( (x*x + y*y) <= radSqu )
//			if ( (x*x + y*y) == radSqu )
			if ( tmp <= radSqu+2 && tmp >= radSqu-2 )
				mask.set(i,j,1);
//				mask(i,j) = 1; // white inside
			else
				mask.set(i,j,0);
//				mask(i,j) = 0; // black outside
			++x;
		}
		y += 1; x = l_bound;
	}
	return;
} // end createCircleKernel


} // end of namespace impact_tr4

#endif /*KERNEL_HPP_*/
