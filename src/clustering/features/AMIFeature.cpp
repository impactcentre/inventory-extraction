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
#include "AMIFeature.hpp"

namespace impact_tr4
{


AMIFeature::AMIFeature()
{
}

void AMIFeature::calculateOutputVector(const GrayImage<> &image, const int rowIndex, const int colIndex, ublas::matrix<float>& outMatrix)
{
	// extract object indices out of binary image:
	std::vector<Index> objectIndices;
	image.computeNonZeroIndexVec(objectIndices);

	// calc ami moments:
	Moments moments;
	ImageMoments::moments(image, moments, true);
	AmiMoments amiMoments;
	ImageMoments::amiMoments(moments, amiMoments);
	// save moments to data matrix:
	double *pAmiMoment = &amiMoments.ami1;
	for (int j=colIndex; j<colIndex+amiMoments.size; ++j) {
		outMatrix(rowIndex,j) = *pAmiMoment;
		++pAmiMoment; // go to next ami moment
//		std::cout << outMatrix(rowIndex,j) << std::endl;
	}

#if 0 // old version without OpenCV --> slow!
	// calc ami moments:
	std::vector<float_t> amiMoments1To4;
	BinaryImageMoments::ami1To4(objectIndices, amiMoments1To4);

	for (int j=colIndex; j<colIndex+amiMoments1To4.size(); ++j) {
		outMatrix(rowIndex,j) = amiMoments1To4[j-colIndex];
//		std::cout << outMatrix(rowIndex,j) << std::endl;
	}
#endif
	return;
}


} // end of namespace impact_tr4
