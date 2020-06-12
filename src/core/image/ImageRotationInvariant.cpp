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
#include "ImageRotationInvariant.hpp"

namespace impact_tr4
{

void ImageRotationInvariant::calcIntegralInvariant(
		const Vector2D<int> degRange /*= Vector2D<int>(1, 180)*/ )
{
	GrayImage<uint8> imageBinInv;
	ImageFilter::threshold<uint8>( *mpImage, imageBinInv );
	imageBinInv.invert();
	std::cout << imageBinInv << std::endl;

	GrayImage<uint8> centeredImage;
	Index centerOfMassIndex;
	imageBinInv.computeCenterOfMassIndex(centerOfMassIndex, true);
//	ImageMoments::centerOfMassIndex<uint8>(src1, centerOfMassIndex, true);
	std::cout << "center of mass index = " << std::endl;
	std::cout << centerOfMassIndex << std::endl;

	const int border = std::max(mpImage->width()/2, mpImage->height()/2);
	const uint8 extVal = 255;
	ImageOperations::extendToCenter(imageBinInv, centeredImage, centerOfMassIndex, border, extVal);

	GrayImage<uint8> rotImage = centeredImage;
	GrayImage<uint8> resImage = centeredImage;

	// for all angles in the range:
	mResultVec.resize(degRange(1)-degRange(0)+1);
	int c = 0;
	for (int i = degRange(0); i<= degRange(1); ++i) {
		ImageOperations::rotateAndScale<uint8>( centeredImage, rotImage, centerOfMassIndex, i, 1, 255);

		ImageOperations::andOp(centeredImage, rotImage, resImage);
		std::cout << resImage << std::endl;

		mResultVec[c] = resImage.countNonZero();
		++c;
	} // end for i

	return;
} // end calcIntegralInvariant


} // end of namespace impact_tr4
