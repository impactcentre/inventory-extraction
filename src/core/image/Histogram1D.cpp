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
#include "Histogram1D.hpp"

namespace impact_tr4 {

void Histogram1D::setRawDataUInt8( uchar *data, const int size, const Vector2D<double> dataRange /*= Vector2D<double>::MINUS_ONES*/ )
{
	cvReleaseImageHeader( &mpDataImage );
	mDataRange = dataRange;
	mpDataImage = cvCreateImage( cvSize(0,0), IPL_DEPTH_32F, 1 );
	cvInitImageHeader( mpDataImage, cvSize(size, 1), IPL_DEPTH_8U, 1 );
	mpDataImage->imageData = reinterpret_cast<char *> ( data );

	return;
} // end setRawDataUInt8

void Histogram1D::setRawDataFloat( float *data, const int size, const Vector2D<double> dataRange /*= Vector2D<double>::MINUS_ONES*/ )
{
	cvReleaseImageHeader( &mpDataImage );
	mDataRange = dataRange;
	mpDataImage = cvCreateImage( cvSize(0,0), IPL_DEPTH_32F, 1 );
	cvInitImageHeader( mpDataImage, cvSize(size, 1), IPL_DEPTH_32F, 1 );
	mpDataImage->imageData = reinterpret_cast<char *> ( data );

	return;
} // end setRawDataFloat

void Histogram1D::setVecDataUInt8( ublas::vector<uchar> &data, const Vector2D<double> dataRange /*= Vector2D<double>::MINUS_ONES*/ )
{
	cvReleaseImageHeader( &mpDataImage );
	mDataRange = dataRange;
	mpDataImage = cvCreateImage( cvSize(0,0), IPL_DEPTH_32F, 1 );
	cvInitImageHeader( mpDataImage, cvSize(data.size(), 1), IPL_DEPTH_8U, 1 );
	mpDataImage->imageData = reinterpret_cast<char *> ( &data[0] );

	return;
} // end setVecDataUInt8

void Histogram1D::setVecDataFloat( ublas::vector<float> &data, const Vector2D<double> dataRange /*= Vector2D<double>::MINUS_ONES*/ )
{
	cvReleaseImageHeader( &mpDataImage );
	mDataRange = dataRange;
	mpDataImage = cvCreateImage( cvSize(0,0), IPL_DEPTH_32F, 1 );
	cvInitImageHeader( mpDataImage, cvSize(data.size(), 1), IPL_DEPTH_32F, 1 );
	mpDataImage->imageData = reinterpret_cast<char *> ( &data[0] );

	return;
} // end setVecDataFloat

int Histogram1D::calcHistogram( bool doCalcCumulativeVals /*= true*/ )
{
	if (mpDataImage == 0x0) {
		std::cerr << "Warning: No data set yet - histogram not caculated!" << std::endl;
		return -1;
	}

    // calculate range if necessary:
    Vector2D<double> range = mDataRange;
    if ( range == Vector2D<double>::MINUS_ONES ) {
    	range(0) = 1; range(1) = 13;
    	cvMinMaxLoc( mpDataImage, &range(0), &range(1) );
    	std::cout << "data min/max: " << range(0) << ", " << range(1) << std::endl;
    }
//    mNBins = range(1)-range(0)+1;
    int hsize[] = { mNBins };
    // ranges
    float xranges[] = { float(range(0)), float(range(1)) };
    float* ranges[] = { xranges };
    // planes to obtain the histogram, in this case just one
    IplImage* planes[] = { mpDataImage };
    // calculate the histogram:
    releaseHistogram();
    mpHist = cvCreateHist( 1, hsize, CV_HIST_ARRAY, ranges, 1 );
    int accumulate = 1; // accumulate on histogram from before?
    const CvArr* mask = NULL; // data mask
	cvCalcHist( planes, mpHist, accumulate, mask );
	// --- calculate cumulative histogram values if desired:
	if ( doCalcCumulativeVals ) {
		calcCumulativeVals();
	}

	return 0;
} // end calcHistogram()

void Histogram1D::calcCumulativeVals()
{
	mCumulativeValues.resize(mNBins);

	double val = 0.0f;
    for ( int i=0; i < mNBins; i++ ) {
    	 val += this->histVal( i );
    	 mCumulativeValues[i] = val;
    } // end for i

	return;
} // end calcCumulativeVals

void Histogram1D::printHistogram( ) const
{
    for ( int i=0; i < mNBins; i++ ) {
    	std::cout << "Bin " << i+1 << ": " << histVal(i) << std::endl;
    }
} // end printHistogram

int Histogram1D::drawHistogram( const std::string windowName /*= std::string("Histogram")*/, const int histWidth /*= 400*/, const int histHeight /*= 300*/, const bool drawCumulative /* = false */ ) const
{
	if (!mpHist) {
		std::cerr << "Warning: No data set yet - histogram not caculated!" << std::endl;
		return -1;
	}
    // max and min value of the histogram
    float minVal = 0, maxVal = 0;
    float minValCum = 0, maxValCum = 0;
    getMinMax( &minVal, &maxVal );
    getMinMaxCum( &minValCum, &maxValCum );
	std::cout << "hist min/max: " << minVal << ", " << maxVal << std::endl;
	float binWidth = float(histWidth) / float(mNBins);
//	std::cout << "bin width = " << binWidth << std::endl;
    IplImage *imgHistogram = cvCreateImage( cvSize(histWidth, histHeight),8,1 );
    cvRectangle( imgHistogram, cvPoint(0,0), cvPoint(histWidth, histHeight), CV_RGB(255,255,255),-1 );
    // value and normalized value
    double value = 0;
    int scaledValue = 0;
    // draw the histogram:
    for ( int i=0; i < mNBins; i++ ) {
    	if (drawCumulative) {
	    	value = this->histValCum( i );
	    	scaledValue = cvRound( value*histHeight/maxValCum );
    	}
    	else {
    		value = this->histVal( i );
    		scaledValue = cvRound( value*histHeight/maxVal );
    	}

//  	cvLine(imgHistogram,cvPoint(i, histHeight), cvPoint(i, histHeight-scaledValue), CV_RGB(0,0,0));
    	cvRectangle( imgHistogram, cvPoint(int(i*binWidth), histHeight), cvPoint(int((i+1)*binWidth), histHeight-scaledValue), CV_RGB(0,0,0), CV_FILLED );


    }
    // show the histogram:
    cvNamedWindow( windowName.c_str(), 1 );
	cvShowImage( windowName.c_str(), imgHistogram );

	return 0;
} // end drawHistogram

#if 0
double Histogram1D::compare( const Histogram1D& other, int method )
{



} // end compare
#endif

} // end of namespace impact_tr4


