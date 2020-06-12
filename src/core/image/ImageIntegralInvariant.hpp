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
#ifndef IMAGEINTEGRALINVARIANT_HPP_
#define IMAGEINTEGRALINVARIANT_HPP_

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "GrayImage.hpp"
#include "Kernel.hpp"
#include "ImageFilter.hpp"
#include "Morphology.hpp"

//using namespace boost::numeric::ublas;

namespace impact_tr4 {

//struct InvariantProperties {
//	std::vector<int> radii; // the radii
//	std::vector<InvariantType> types; // the image invariants
//
//
//};

enum InvariantType {
	BALL = 0,
	CIRCLE,
	RECT
}; // end InvariantType

// Forward references:
template <class T> class GrayImage;

// Class definition:
class ImageIntegralInvariant
{
public:
	ImageIntegralInvariant( const GrayImage<uint8> *pImage )
	: mpImage(pImage)
		{ }

	void setImage( const GrayImage<uint8> *pImage )  { mpImage = pImage; }
	bool hasImage() const { return (mpImage!=NULL); }
	const GrayImage<uint8>* image() const { return mpImage; }
//	const int imageSize() const { return mpImage->size(); }

//	int calcIntegralInvariant( int radius = 3, InvariantType invType = BALL );
	int calcIntegralInvariant( const Kernel<float>& mask );

	ublas::vector<float>* resultVecPointer() { return &mResultVec; };
	const ublas::vector<float>& resultVec() const { return mResultVec; }
	const int resultVecSize() const { return mResultVec.size(); }
	const int sizeKernelNonZero() const { return mSizeKernelNonZero; }

	static void calcIntegralInvariantImage( const Kernel<float>& mask, const GrayImage<uint8> &src, GrayImage<float> &result );

private:
	// MEMBERS:
	const GrayImage<uint8> *mpImage;
	ublas::vector<float> mResultVec;
	ublas::vector<Index> mIndicesVec;
	int mSizeKernelNonZero;
};

} // end of namespace impact_tr4

#endif /*IMAGEINTEGRALINVARIANT_HPP_*/
