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
#ifndef INVARIANTHISTOGRAM_HPP_
#define INVARIANTHISTOGRAM_HPP_

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "ImageIntegralInvariant.hpp"

//using namespace boost::numeric::ublas;

namespace impact_tr4 {

class ImageIntegralInvariant;

class InvariantHistogram {
public:
	// Default constructor
	InvariantHistogram() { ; }

	void getMinMax( float *min, float *max ) const
		{
		*min = *std::min_element( mBins.begin(), mBins.end() );
		*max = *std::max_element( mBins.begin(), mBins.end() );
		}
	void getMinMaxCum( float *min, float *max ) const
		{
		*min = (float) mCumBins(0);
		*max = (float) mCumBins(mCumBins.size()-1);
		return;
		}

	void calculateHistogram(const ImageIntegralInvariant& invariant, bool doCalcCumulativeVals = true, bool doCalcNormalizedVals = true);
	void printHistogram() const;
	void printCumulativeHistogram() const;
	void printNormalizedHistogram() const;
	void drawHistogram(	const std::string windowName = std::string("Histogram"),
						const int histWidth = 400, const int histHeight = 300,
						const bool drawCumulative = false ) const;
	void calcCumulativeVals();
	void calcNormalizedVals(const int dataSize);

	bool hasData() const { return (mBins.size()!=0); }
	int get(const int i) const { return mBins[i]; }

	ublas::vector<float>& getBins() { return mBins; }
	ublas::vector<float>& getNormBins() { return mNormBins; }
	ublas::vector<float>& getCumBins() { return mCumBins; }

private:
//	std::vector<unsigned int> mBins;
//	std::vector<double> mNormBins;
//	std::vector<unsigned int> mCumBins;

	ublas::vector<float> mBins;
	ublas::vector<float> mNormBins;
	ublas::vector<float> mCumBins;
}; // end InvariantHistogram

} // end of namespace impact_tr4

#endif /*INVARIANTHISTOGRAM_HPP_*/
