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
#include "HuMomentsFeature.hpp"

namespace impact_tr4
{

void HuMomentsFeature::calculateOutputVector(const GrayImage<> &image, const int rowIndex, const int colIndex, ublas::matrix<float>& outMatrix)
{
	// extract object indices out of binary image:
	std::vector<Index> objectIndices;
	image.computeNonZeroIndexVec(objectIndices);
	// calc hu moments:
	Moments moments;
	ImageMoments::moments(image, moments, true);
	HuMoments huMoments;
	ImageMoments::huMoments(moments, huMoments);
	// save moments to data matrix:
	double *pHuMoment = &huMoments.hu1;
	for (int j=colIndex; j<colIndex+7; ++j) {
		outMatrix(rowIndex,j) = *pHuMoment;
		++pHuMoment; // go to next hu moment
//		std::cout << outMatrix(rowIndex,j) << std::endl;
	}

#if 0 // old version without OpenCV --> slow!
	std::vector<float_t> huMoments1To4;
	BinaryImageMoments::hu1To4(objectIndices, huMoments1To4);
	for (int j=colIndex; j<colIndex+huMoments1To4.size(); ++j) {
		outMatrix(rowIndex,j) = huMoments1To4[j-colIndex];
	//	std::cout << outMatrix(rowIndex,j) << std::endl;
	}
#endif
	return;
}

} // end of namespace impact_tr4
