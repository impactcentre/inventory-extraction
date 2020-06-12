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
#include "ImageIntegralInvariantFeature.hpp"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

//using namespace boost::numeric::ublas;

namespace impact_tr4
{


void ImageIntegralInvariantFeature::calculateOutputVector(const GrayImage<> &image, const int rowIndex, const int colIndex, ublas::matrix<float>& outMatrix)
{
//	std::cout << "caclulateOutputVector, mask = " << std::endl;
//	std::cout << mMask << std::endl;

	// calculate integral invariant for region of interest:
	ImageIntegralInvariant intInv( &image );
	intInv.calcIntegralInvariant( mMask );
	const ublas::vector<float>& resultVec = intInv.resultVec();
//    	std::cout << "size of result vec = " << resultVec.size() << std::endl;
	// calculate histogram of integral invariant:
	InvariantHistogram invHisto;
	invHisto.calculateHistogram( intInv );

//	vector<unsigned int> *pBins;
	ublas::vector<float> *pBins = 0x0;
	switch (mHistogramType) {
		case 0:
//			std::cout << "using bins" << std::endl;
			pBins = &invHisto.getBins();
			break;
		case 1:
//			std::cout << "using normalized bins" << std::endl;
			pBins = &invHisto.getNormBins();
			break;
		case 2:
//			std::cout << "using cumulative bins" << std::endl;
			pBins = &invHisto.getCumBins();
			break;
		default:
			throw Exception("Error in calculateOutputVector: wrong histogram type specified!");
			break;
	} // end switch
//    	std::vector<unsigned int>& bins = invHisto.getCumBins();
	for (int j=colIndex; j<colIndex+pBins->size(); ++j) {
//		std::cout << (*pBins)[j-colIndex] << " ";
		outMatrix(rowIndex,j) = (*pBins)[j-colIndex];
	}
//	std::cout << std::endl;

	return;
} // end calculateOutputVector

} // end of namespace impact_tr4
