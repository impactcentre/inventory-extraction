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
#ifndef IMAGEROTATIONINVARIANT_HPP_
#define IMAGEROTATIONINVARIANT_HPP_

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "GrayImage.hpp"
#include "ImageFilter.hpp"
#include "Morphology.hpp"

namespace impact_tr4
{


class ImageRotationInvariant
{
public:
	ImageRotationInvariant( const GrayImage<uint8> *pImage )
		: mpImage(pImage) { }
	virtual ~ImageRotationInvariant() { mpImage = NULL; }

	void setImage( const GrayImage<uint8> *pImage )  { mpImage = pImage; }
	bool hasImage() const { return (mpImage!=NULL); }
	const GrayImage<uint8>* image() const { return mpImage; }

	void calcIntegralInvariant( const Vector2D<int> degRange = Vector2D<int>(0, 180) );
	ublas::vector<float>* resultVecPointer() { return &mResultVec; };
	const ublas::vector<float>& resultVec() const { return mResultVec; }
	const int resultVecSize() const { return mResultVec.size(); }

private:
	// MEMBERS:
	const GrayImage<uint8> *mpImage;
	ublas::vector<float> mResultVec;
};

} // end of namespace impact_tr4

#endif /*IMAGEROTATIONINVARIANT_HPP_*/
