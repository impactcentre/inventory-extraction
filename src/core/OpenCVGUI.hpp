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
#ifndef OPENCVGUI_HPP_
#define OPENCVGUI_HPP_

// OpenCV includes:
#include <cv.h>
#include <highgui.h>

// Project includes:
#include "core/image/GrayImage.hpp"

namespace impact_tr4 {

//template <class T> class GrayImage;

/**
 * Class wrappping basic OpenCV GUI functions.
 * The class wraps the basic functions from the HighGUI module of OpenCV
 * as static inline functions.
 * 
 * */
class OpenCVGUI
{
public:
	static void waitForKeyStroke( int delayMilliSeconds=0 )
		{ cvWaitKey(delayMilliSeconds); return; }
	static void destroyWindow( const std::string &name )
		{ cvDestroyWindow( name.c_str() ); return; }
	static void destroyAllWindows( )
		{ cvDestroyAllWindows(); return; }
	static void resizeWindow(const std::string &name, const int width, const int height)
		{ cvResizeWindow( name.c_str(), width, height ); return; }
	static void moveWindow(const std::string &name, const int x, const int y)
		{ cvMoveWindow( name.c_str(), x, y ); return; }
	static int createNamedWindow( const std::string &name, int flags=CV_WINDOW_AUTOSIZE )
		{ return cvNamedWindow(name.c_str(), flags); }
	template <class T>
	static void showImage( const std::string &name, const GrayImage<T> &image )
		{ cvShowImage( name.c_str(), image.mpImage ); }
	template <class T>
	static void showManyImages(char* title, int nArgs, ...);

}; // end class OpenCVGUI

template <class T>
/*static*/
void OpenCVGUI::showManyImages( char* title, int nArgs, ... )
{
    // img - Used for getting the arguments
    IplImage *img;

    // DispImage - the image in which input images are to be copied
    IplImage *DispImage;
    int size;
    int i;
    int m, n;
    int x, y;

    // w - Maximum number of images in a row
    // h - Maximum number of images in a column
    int w, h;

    // scale - How much we have to resize the image
    float scale;
    int max;

    // If the number of arguments is lesser than 0 or greater than 12
    // return without displaying
    if(nArgs <= 0) {
        printf("showManyImages: Number of arguments too small....\n");
        return;
    }
    else if(nArgs > 12) {
        printf("showManyImages: Number of arguments too large....\n");
        return;
    }
    // Determine the size of the image,
    // and the number of rows/cols
    // from number of arguments
    else if (nArgs == 1) {
        w = h = 1;
        size = 300;
    }
    else if (nArgs == 2) {
        w = 2; h = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4) {
        w = 2; h = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6) {
        w = 3; h = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8) {
        w = 4; h = 2;
        size = 200;
    }
    else {
        w = 4; h = 3;
        size = 150;
    }

    // Create a new 3 channel image
//    DispImage = cvCreateImage( cvSize(100 + size*w, 60 + size*h), 8, 3 );
    DispImage = cvCreateImage( cvSize(100 + size*w, 60 + size*h), GrayImage<T>::determineDepth(), 1 ); // new

    // Used to get the arguments passed
    va_list args;
    va_start(args, nArgs);
    // Loop for nArgs number of arguments
    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
        // Get the Pointer to the IplImage
    	std::cout << "h1" << std::endl;
    	img = (va_arg(args, GrayImage<T>*))->iplImage(); // new
    	std::cout << "h2" << std::endl;
//        img = va_arg(args, IplImage*);

        // Check whether it is NULL or not
        // If it is NULL, release the image, and return
        if(img == 0) {
            printf("Invalid arguments");
            cvReleaseImage(&DispImage);
            return;
        }

        // Find the width and height of the image
        x = img->width;
        y = img->height;

        // Find whether height or width is greater in order to resize the image
        max = (x > y)? x: y;

        // Find the scaling factor to resize the image
        scale = (float) ( (float) max / size );

        // Used to Align the images
        if( i % w == 0 && m!= 20) {
            m = 20;
            n+= 20 + size;
        }
        std::cout << "h2aa" << std::endl;
        // Set the image ROI to display the current image
        cvSetImageROI(DispImage, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));
        std::cout << "h2a" << std::endl;
        // Resize the input image and copy the it to the Single Big Image
        cvResize(img, DispImage);
std::cout << "h2a" << std::endl;
        // Reset the ROI in order to display the next image
        cvResetImageROI(DispImage);
    }
    // Create a new window, and show the Single Big Image
    cvNamedWindow( title, 1 );
    cvShowImage( title, DispImage);

    cvWaitKey();
    cvDestroyWindow(title);

    // End the number of arguments
    va_end(args);

    // Release the Image Memory
    cvReleaseImage(&DispImage);
}

} // end namespace impact_tr4


#endif /*OPENCV_HPP_*/
