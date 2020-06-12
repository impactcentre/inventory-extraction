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
#ifndef HISTOGRAM_HPP_
#define HISTOGRAM_HPP_

// OpenCV includes:
#include <cv.h>
#include <highgui.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "core/core.h"
#include "core/Vector2D.hpp"

namespace impact_tr4 {

class Histogram1D {
public:
	Histogram1D( const int nBins = 20 )
	: mNBins( nBins ), mpHist( 0x0 ), mpDataImage( 0x0 )
		{ }

	// GETTERS AND SETTERS:
	void setNBins( const int nBins ) { mNBins = nBins; }
	const int nBins( ) const { return mNBins; }

	void setRawDataUInt8( uchar *data, const int size, const Vector2D<double> dataRange = Vector2D<double>::MINUS_ONES );
	void setRawDataFloat( float *data, const int size, const Vector2D<double> dataRange = Vector2D<double>::MINUS_ONES );
	void setVecDataUInt8( ublas::vector<uchar> &data, const Vector2D<double> dataRange = Vector2D<double>::MINUS_ONES );
	void setVecDataFloat( ublas::vector<float> &data, const Vector2D<double> dataRange = Vector2D<double>::MINUS_ONES );

	/** Returns the histogram value at index i. */
	double histVal( const int i) const
		{ return cvQueryHistValue_1D(mpHist, i ); }
	/** Returns the cumulative histogram value at index i. */
	double histValCum( const int i ) const
		{ return mCumulativeValues[i]; }

	void getMinMax( float *min, float *max ) const
		{ cvGetMinMaxHistValue( mpHist, min, max); }

	void getMinMaxCum( float *min, float *max ) const
		{
//		*min = (float) mCumulativeValues.front();
		*min = (float) mCumulativeValues[0];
//		*max = (float) mCumulativeValues.back();
		*max = (float) mCumulativeValues[mCumulativeValues.size()-1];
		return;
		}

	// OPERATIONS:
	void normalize( double factor = 1.0f )
		{ cvNormalizeHist( mpHist, factor ); }
	int calcHistogram( bool doCalcCumulativeVals = true );
	void calcCumulativeVals();

	void releaseHistogram( )
		{ cvReleaseHist( &mpHist ); }
	int drawHistogram(	const std::string windowName = std::string("Histogram"),
						const int histWidth = 400, const int histHeight = 300,
						const bool drawCumulative = false ) const;
	void printHistogram( ) const;

	double compare( const Histogram1D& other, int method )
		{
		return cvCompareHist(mpHist, other.mpHist, method);
		}
private:
	// PRIVATE FUNCTIONS:
	void basicInit();

	// MEMBER VARIABLES:
	int mNBins; // the number of bins in the histogram
	Vector2D<double> mDataRange; // the range of the data
	CvHistogram *mpHist; // the OpenCV histogram variable
	IplImage *mpDataImage; // the image for the data
	ublas::vector<double> mCumulativeValues; // cumulative values of histogram
};


} // end of namespace impact_tr4

#endif /*HISTOGRAM_HPP_*/
