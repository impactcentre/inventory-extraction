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
#include "InvariantHistogram.hpp"

namespace impact_tr4 {

void InvariantHistogram::calculateHistogram(const ImageIntegralInvariant& invariant, bool doCalcCumulativeVals /*= true*/, bool doCalcNormalizedVals /*= true*/)
{

	const ublas::vector<float>& data = invariant.resultVec();
	mBins.resize( invariant.sizeKernelNonZero() );
	for (int i=0; i<mBins.size(); ++i) mBins[i] = 0;

	for (int i=0; i<data.size(); ++i) {
//		std::cout << data[i] << std::endl;
		mBins( data[i]-1 )++;
	} // end for i

	// --- calculate cumulative histogram values if desired:
	if ( doCalcCumulativeVals ) {
		calcCumulativeVals();
	}

	// --- calculate normalized histogram values if desired:
	if ( doCalcNormalizedVals ) {
		calcNormalizedVals( data.size() );
	}

	return;
} // end calculateHistogram

void InvariantHistogram::calcCumulativeVals()
{
	mCumBins.resize( mBins.size() );

	float val = 0.0f;
    for ( int i=0; i < mBins.size(); i++ ) {
    	 val += mBins[i];
    	 mCumBins[i] = val;
    } // end for i

	return;
} // end calcCumulativeVals

void InvariantHistogram::calcNormalizedVals(const int dataSize)
{
	mNormBins.resize( mBins.size() );

	if (dataSize > 0) { // only if dataSize is greater zero, calc normalized value
	    for ( int i=0; i < mNormBins.size(); i++ ) {
	    	mNormBins[i] = float(mBins[i]) / float(dataSize);
	    } // end for i
	}
	else { // else: set all values to zero
		for (int i=0; i<mNormBins.size(); i++ ) {
			mNormBins[i] = 0;
		}
	}

	return;
} // end calcNormalizedVals

void InvariantHistogram::printHistogram() const
{
    for ( int i=0; i < mBins.size(); i++ ) {
    	std::cout << "Bin " << i+1 << ": " << mBins[i] << std::endl;
    }
} // end printHistogram

void InvariantHistogram::printNormalizedHistogram() const
{
//	double checkSum = 0;
    for ( int i=0; i < mNormBins.size(); i++ ) {
    	std::cout << "Bin " << i+1 << ": " << mNormBins[i] << std::endl;
//    	checkSum+=mNormBins[i];
    }
//    std::cout << "Checksum is: " << checkSum << std::endl;
} // end printNormalizedHistogram

void InvariantHistogram::printCumulativeHistogram() const
{
    for ( int i=0; i < mCumBins.size(); i++ ) {
    	std::cout << "Bin " << i+1 << ": " << mCumBins[i] << std::endl;
    }
} // end printCumulativeHistogram

void InvariantHistogram::drawHistogram( const std::string windowName /*= std::string("Histogram")*/, const int histWidth /*= 400*/, const int histHeight /*= 300*/, const bool drawCumulative /* = false */ ) const
{
	if ( !hasData() ) {
		std::cerr << "Warning: No data set yet - histogram not caculated!" << std::endl;
		return;
	}
    // max and min value of the histogram

	float minVal = 0, maxVal = 0;
    float minValCum = 0, maxValCum = 0;
    getMinMax( &minVal, &maxVal );
    getMinMaxCum( &minValCum, &maxValCum );
    maxVal = *std::max_element( mNormBins.begin(), mNormBins.end() );

    std::cout << "maxVal is: " << maxVal << std::endl;

//	std::cout << "hist min/max: " << minVal << ", " << maxVal << std::endl;
	float binWidth = float(histWidth) / float(mBins.size());
//	std::cout << "bin width = " << binWidth << std::endl;
    IplImage *imgHistogram = cvCreateImage( cvSize(histWidth, histHeight),8,1 );
    cvRectangle( imgHistogram, cvPoint(0,0), cvPoint(histWidth, histHeight), CV_RGB(255,255,255),-1 );
    // value and normalized value
    double value = 0;
    int scaledValue = 0;
    // draw the histogram:
    for ( int i=0; i < mBins.size(); i++ ) {
    	if (drawCumulative) {
	    	value = mCumBins[i];
	    	scaledValue = cvRound( value*histHeight/maxValCum );
    	}
    	else {
//    		value = mBins[i];
    		value = mNormBins[i];
    		scaledValue = cvRound( value*histHeight/maxVal );
    	}
//  	cvLine(imgHistogram,cvPoint(i, histHeight), cvPoint(i, histHeight-scaledValue), CV_RGB(0,0,0));
    	cvRectangle( imgHistogram, cvPoint(int(i*binWidth), histHeight), cvPoint(int((i+1)*binWidth), histHeight-scaledValue), CV_RGB(0,0,0), CV_FILLED );
    } // end for i
    // show the histogram:
    cvNamedWindow( windowName.c_str(), 1 );
	cvShowImage( windowName.c_str(), imgHistogram );

	return;
} // end drawHistogram


} // end of namespace impact_tr4
