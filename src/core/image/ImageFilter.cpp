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
#include "ImageFilter.hpp"

namespace impact_tr4 {

/*static*/
int ImageFilter::distTransform(const GrayImage<uint8>& src, GrayImage<float>& result, int disType/*=CV_DIST_L2*/, int maskSize/*=3*/, float *mask/*=0*/ )
{
//	std::cout << "distTransform!" << std::endl;

	if (src.empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return 0;
	}
	result.init(src.widthRoi(), src.heightRoi());


	cvDistTransform(src.mpImage, result.mpImage, disType, maskSize, mask );
	return 1;
}

} // end of namespace impact_tr4

