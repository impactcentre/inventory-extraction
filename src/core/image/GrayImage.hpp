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
#ifndef GRAYIMAGE_HPP_
#define GRAYIMAGE_HPP_

// System includes:
#include <iostream>
#include <typeinfo>
// QT-INCLUDES:
#if 1
	#include <QImage>
#endif
// OpenCV includes:
#include <cv.h>
#include <highgui.h>
// boost includes:
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
// Project includes:
#include "core/core.h"
#include "core/Index.hpp"
#include "util/util.h"
#include "util/StopWatch.hpp"
// local includes:
//#include "ImageFilter.hpp"
#include "ImageIterator.hpp"
#include "BoundingBox.hpp"
#include "types.h"
//#include "ImageMoments.hpp"

namespace impact_tr4 {

template <class T> class GrayImage;
class Morphology;
template <class T> class ImageIterator;
class ImageOperations;
class ImageFilter;
class ImageMoments;
struct Moments;
struct HuMoments;
struct AmiMoments;
struct BasicMoments;
class BoundingBox;
class OpenCVGUI;

enum LineType
{
	EIGHT_CONNECTED = 8,
	FOUR_CONNECTED = 4,
	ANTI_ALIASED = CV_AA
};

/**
 * Output operator for images. Note, that image values are also printed, which can
 * lead to a long output time for large images.
 * */
template <class T>
std::ostream& operator<<( std::ostream& os, const GrayImage<T>& image );

/** Class representing a grayscale image.
 * This is basically a wrapper class for an OpenCV image. Basic image operations are
 * wrapped in an object-oriented manner for convenience. Note, that more sophisticated
 * filtering functions, such as sobel, laplace or morphological operations are wrapped
 * by special filter classes also present in this library (see for example the classes ImageFilter or Morphology).
 * @see ImageFilter
 * @see Morphology
 * 
 * */
template <class T = uint8>
class GrayImage
{
public:
//	/** Typedef wrap for an Iterator regaring to this class. */
//	typedef ImageIterator Iterator;

	/** Default constructor. Image is  to 0x0. */
	GrayImage( )
	{
//		std::cout << "empty constructor called" << std::endl;
//		mpImage = cvCreateImage( cvSize(0, 0), determineDepth(), 1 );
		mpImage = cvCreateImageHeader( cvSize(0, 0), determineDepth(), 1 );
//		std::cout << mpImage->width << ", " << mpImage->height << std::endl;
	}

	/** Constructs a GrayImage with the specified width and height. */
	GrayImage( const int width, const int height )
	{
		mpImage = 0x0;
		createImage( width, height );
//		mpImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
	}

	/** The copy-constructor. */
	GrayImage( const GrayImage& source )
	{
		mpImage = 0x0;
		this->init(source);
#if 0
		// Copy the image:
#if 1
		releaseImage( );
		mpImage = cvCloneImage( source.mpImage );
#else
//		mpImage = cvCreateImage( cvSize(source.width(), source.height()), IPL_DEPTH_8U, 1 );
//		cvCopy( source.mpImage, mpImage );
#endif
#endif
	}

	/** s the  from the given IplImage pointer to this GrayImage. Note, that
	 * the  is NOT copied, so the  region must not be deleted after this call!
	 * */
	explicit GrayImage( IplImage* pIplImage )
	{
		releaseImage();
		mpImage = pIplImage;
	}

#if 1
	/** Constructs a GrayImage out of a QImage. */
	GrayImage( const QImage& qImage )
		{ mpImage = 0x0; this->init(qImage); }
#endif

	/** Constructor loading an image from a file. Only 8-bit images are supported by this function! */
	explicit GrayImage( const std::string& fileName ) { mpImage = 0x0; this->load(fileName); }

	/** Constructor taking a pointer to an existing IplImage. */
//	GrayImage(IplImage* pImage) { mpImage = pImage; }

	/** Destructor. */
	virtual ~GrayImage()
	{
//		std::cout << "destructor called" << std::endl;
		releaseImage();
//		cvReleaseImage(&mpImage); // release the image
	}
	GrayImage& init( const GrayImage& source )
	{
		// Copy the image:
#if 1
		releaseImage();
		mpImage = cvCloneImage( source.mpImage );
#else
//		mpImage = cvCreateImage( cvSize(source.width(), source.height()), IPL_DEPTH_8U, 1 );
//		cvCopy( source.mpImage, mpImage );
#endif
		return *this;
	}

#if 1
	/** Initializes this object using a QImage object. */
	 GrayImage& init(const QImage& qImage);
#endif

	/** Inits this image with the specified width and height, i.e. it creates space
	 * for a width x height dimensional image. This function should be faster than
	 * the resize(width, height) function, as it does not interpolate existing pixel
	 * values. Old pixel values are just deleted instead.
	 * */
	GrayImage& init(const int width, const int height)
	{
		releaseImage();
		createImage( width, height );

		return *this;
	}

	// VALIDATION
	/** Returns true, if this image is empty. */
	bool empty() const { return (size()==0); }

	/** Returns true, if a region of interest is  for this image. */
	bool hasRoi() const { return (mpImage->roi != 0x0); }

	/** Returns true, if (i,j) is a valid pixel index of this image. */
	bool isValidIndex(const size_t i, const size_t j) const
		{ return (i>=0 && i<rows()) && (j>=0 && j<columns()); }

	/** Returns true, if the given index object is a valid index of this image. */
	bool isValidIndex(const Index& index) const
		{ return isValidIndex(index.row(), index.column()); }

	/** Returns true, if otherImage has equal size as this image. */
	bool hasEqualSize(const GrayImage& otherImage) const
		{ return ( width() == otherImage.width() && height() == otherImage.height() ); }

	/** Returns true, if otherImage has the same roi as this image. */
	bool hasEqualSizeRoi(const GrayImage& otherImage) const
		{ return ( widthRoi() == otherImage.widthRoi() && heightRoi() == otherImage.heightRoi() ); }

	// ACCESS FUNCTIONS
	/** Returns the width of the image. */
	const int width() const { return mpImage->width; }
	/** Returns the height of the image. */
	const int height() const { return mpImage->height; }
	/** Returns the size of the image, i.e. rows*columns */
	const int size() const { return width() * height(); }
	/** Returns the nr. of rows in this image, i.e. its height. */
	const int rows() const { return height(); }
	/** Returns the nr. of columns in this image, i.e. its width. */
	const int columns() const { return width(); }
	/** Returns the depth of this image. */
	const int depth() const { return mpImage->depth; }

	/** Returns the x-position of the region of interest. */
	const int xRoi() const
		{ return (hasRoi()) ? mpImage->roi->xOffset : 0; }
	/** Returns the y-position of the region of interest. */
	const int yRoi() const
		{ return (hasRoi()) ? mpImage->roi->yOffset : 0; }

	/** Returns the width of the region of interest.
	 * The original image width is returned, if no roi is .
	 * */
	const int widthRoi() const
		{ return (hasRoi()) ? mpImage->roi->width : mpImage->width; }
	/** Returns the height of the region of interest.
	 * The original image height is returned, if no roi is .
	 * */
	const int heightRoi() const
		{ return (hasRoi()) ? mpImage->roi->height : mpImage->height; }
	/** Returns the size of the region of interest.
	 * The original image size is returned if no roi is .
	 * */
	const int sizeRoi() const { return widthRoi() * heightRoi(); }

	const int rowStartRoi() const
		{ return yRoi(); }
	const int columnStartRoi() const
		{ return xRoi(); }

	/** Returns the nr. of rows in the region of interest, i.e. its height. */
	const int rowsRoi() const { return heightRoi(); }
	/** Returns the nr. of columns in the region of interest, i.e. its width. */
	const int columnsRoi() const { return widthRoi(); }

	/** Returns a pointer to the image data. */
	char* data() { return (mpImage->imageData); }

	/** Returns the pointer to the IplImage. */
	IplImage* iplImage() { return mpImage; }

	/** s the specified region of interest for this image.
	 * @param bbox The BoundingBox object that represents the region of interest.
	 * */
	void setRoi(const BoundingBox& bbox)
	{
		if (!bbox.empty()) {
			cvSetImageROI(mpImage, bbox.mRoi);
		}
		else {
			std::cout << "Warning: empty BoundingBox - nothing changed!" << std::endl;
		}
//		printf("address after create roi: 0x%x\n", mpImage->roi);
		return;
	} // end Roi

	/** Releases the current region of interest for this image. */
	void releaseRoi()
	{
		cvResetImageROI(mpImage);
//		printf("address after release roi (should be 0x0!): 0x%x\n", mpImage->roi);
	} // end releaseRoi

	void computeRoiImage(GrayImage& roiImage) const
	{
		roiImage.init(widthRoi(), heightRoi());
		cvCopy( mpImage, roiImage.mpImage );
		return;
	}

	/** Returns a reference to pixel (i,j) of this image.
	 * Note, that i specifies the row (i.e. the y-component), while j specifies
	 * the column (i.e. the x-component) of the pixel. This rather confusing
	 * indexing is most commonly used for images.
	 * */
	T& get(const size_t i, const size_t j)
	{
		assert( isValidIndex(i, j) );
		return CV_IMAGE_ELEM( mpImage, T , i ,j );
//		return (uchar &) (mpImage->image[i*columns() + j]);
	}

	/** Const-version of get(i,j) method. */
	const T& get(const size_t i, const size_t j) const
	{
		assert( isValidIndex(i, j) );
//		return (T) (cvGet2D( mpImage, i, j ).val[0]);
		return CV_IMAGE_ELEM( mpImage, T , i, j );
	}

	void set(const size_t i, const size_t j, const T value)
	{
		assert(isValidIndex(i,j));
//		CvScalar sc = cvRealScalar(value);
//		cv2D( mpImage, i, j, sc );
		(*this)(i,j) = value;

		return;
	}

	/** Copies the given sub-image into this image.
	 * NOTE: the roi with regard to the base image is not respected!
	 * @return 0 if success, -1 elsewise
	 * */
	int set(const size_t i, const size_t j, const GrayImage<T>& subImage);

//	int& GrayImage::get(const size_t i, const size_t j)
//	{
//		assert( isValidIndex(i,j) );
//
//
//		CvScalar s = cvGet2D( mpImage, i, j );
//		return s[0];
////		return CV_IMAGE_ELEM(mpImage,uchar,i,j);
////		return (uchar &) (mpImage->image[i*columns() + j]);
//	}
//	/** Const-version of get(i,j) method. */
//	const int& GrayImage::get(const size_t i, const size_t j) const
//	{
//		assert( isValidIndex(i,j) );
//		CvScalar s = cvGet2D( mpImage, i, j );
//		return s[0];
//	}

	/** Returns the pixel that the given index points to */
	T& get(const Index index)
		{ return get(index.row(), index.column()); }

	/** Returns the pixel that the given index points to */
	const T& get(const Index index) const
		{ return get(index.row(), index.column()); }

	// OPERATORS
	/** Overloaded ()-operator returns reference of pixel at index (row,col) */
	T& operator()(const size_t row, const size_t col)
		{ return get(row, col); }

	/** Const-version of ()-operator. */
	const T& operator()(const size_t row, const size_t col) const
		{ return get(row, col); }

	/** Overloaded ()-operator returns pixel for the given index object */
	T& operator()(const Index index)
		{ return get(index); }

	/** Overloaded ()-operator returns pixel for the given index object */
	const T operator()(const Index index) const
		{ return get(index); }

	const GrayImage& operator=(const GrayImage& rhs)
	{
//		std::cout << "operator= in grayimage called" << std::endl;
		if (this != &rhs) {
			releaseImage();
			mpImage = cvCloneImage( rhs.mpImage );
		}
		return *this;
	}

	// OPERATIONS
	void load(const std::string& fileName);

	/** Saves this image to file with given filename.
	 * The image format is determined automatically using the prefix.
	 * Note, that this function is only supported for 8-bit images!
	 * */
	void save(const std::string& fileName) const;

#if 1
	/**
	 * Computes a QImage object of this image.
	 * @param qImage [out] The resulting pointer to the QImage object
	 */
	void computeQImage(QImage& qImage) const;
#endif

	/** Resizes the image to the specified size using the given interpolation method.
	 * @param width The new width of the image.
	 * @param height The new height of the image.
	 * @param interpolation Methods:
	 * 		CV_INTER_NN - nearest-neigbor interpolation,
	 * 		CV_INTER_LINEAR - bilinear interpolation (used by default)
	 * 		CV_INTER_AREA - resampling using pixel area relation. It is preferred method for image decimation that gives moire-free results. In case of zooming it is similar to CV_INTER_NN method.
	 * 		CV_INTER_CUBIC - bicubic interpolation.
	 * */
	void resize(const int width, const int height, int interpolation=CV_INTER_LINEAR);

	void randomFill(int distributionType=CV_RAND_UNI, double param1=0, double param2=256)
	{
		CvRNG rng_state = cvRNG(StopWatch::getMsTickCount());
		cvRandArr( &rng_state, mpImage, distributionType, cvRealScalar(param1), cvRealScalar(param2) );
		return;
	} // randomFill()

	void computeCenterOfMassCenteredImage(GrayImage<T>& centeredImage, const T padVal/*=0*/);

	void resizeRelativeToCenterOfMass(const int newWidth, const int newHeight, const T padVal/*=0*/);

	/** Fills this image with given value. */
	void fill(const T& value) { cvSet( mpImage, cvScalar(value) ); }
	/** Clears the image. */
	void setZero() { cvSetZero( mpImage ); }

//	/** Return iterator pointing to begin of image. */
	ImageIterator<T> begin() const {
//		return(Iterator(this, 0, 0));
		return(ImageIterator<T>((GrayImage*)(this), yRoi(), xRoi()));
	}
//	/** Returns iterator pointing to end of image. Note that *end() is no member of this image! */
	ImageIterator<T> end() const {
//		return(Iterator(this, rows()-1, columns()));
		return(ImageIterator<T>((GrayImage*)(this), rowsRoi()-1, columnsRoi()));
	}

	/** Creates a new image with specified width, height. */
	static IplImage* createIplImage(const int width, const int height)
		{
		IplImage* pNewImage = cvCreateImage( cvSize(width, height), determineDepth(), 1 );
		return pNewImage;
		}

	/** Binarizes this image using the inverted binary method,
	 * s.t. all background pixels are 0 and all foreground (or character)
	 *  pixels are 1.
	 * */
//	inline void binarizeCharacter(double thresholdValue=128)
//		{ binarize(thresholdValue, 1, THRESH_BINARY_INV); }

	/** Binarizes this image with the given threshold value.
	 * See function ImageFilter::threshold for more details.
	 * */
	void binarize(double thresholdValue=128, double maxValue=255, ThreshType thresholdType=THRESH_BINARY);

	/**
	 * Binarize this image using Otsu's method.
	 */
	void binarizeOtsu(double maxValue=255, ThreshType thresholdType=THRESH_BINARY);

	/** Binarizes this image using the specified adaptive thresholding method.
	 * See function ImageFilter::adaptiveThreshold for more details.
	 * */
	void adaptiveBinarize(double maxValue=255, int adaptiveMethod=CV_ADAPTIVE_THRESH_MEAN_C, int blockSize=3, double param1=5);

	/** Inverts this image. Only supported for 8-bit images. */
	void invert();

	/** Returns the number of non-zero elements. */
	int countNonZero() const
		{ return cvCountNonZero(mpImage); }

	double pixelDensity() const
	{
		return (double)countNonZero() / (double)size();
	}

	/** Stores all nonzero elements in the vector nonZeroVec. */
	void computeNonZeroVec(ublas::vector<T> &nonZeroVec) const;
	/** Stores all indices of nonzero elements in the vector nonZeroIndexVec. */
	void computeNonZeroIndexVec(std::vector<Index> &nonZeroIndexVec) const;

	/** Computes the center of mass for this image.
	 * centerOfMas(0) = center of mass in x-direction
	 * centerOfMas(1) = center of mass in y-direction
	 * */
	void computeCenterOfMass(Vector2D<double> &centerOfMass, const bool binary = true) const;

	/**
	 * Computes the index for the center of mass, i.e. the rounded center of mass.
	 * Note, that the original center of mass values are swapped, since an Index object
	 * refers to rows/cols, i.e. y/x coordinates!
	 * centerOfMassIndex(0) = rounded center of mass in y-direction
	 * centerOfMassIndex(1) = rounded center of mass in x-direction
	 * */
	void computeCenterOfMassIndex(Index &centerOfMassIndex, const bool binary = true) const;

	void computeHorizontalProfiles(const Index &center, std::vector<int> &profileUpOuter, std::vector<int> &profileUpInner, std::vector<int> &profileDownOuter, std::vector<int> &profileDownInner) const;
	void computeVerticalProfiles(const Index &center, std::vector<int> &profileLeftOuter, std::vector<int> &profileLeftInner, std::vector<int> &profileRightOuter, std::vector<int> &profileRightInner) const;

	void computeHorizontalProfileBlocks(const int nBlocks, const Index &center, std::vector<float> &profileUpOuterB, std::vector<float> &profileUpInnerB, std::vector<float> &profileDownOuterB, std::vector<float> &profileDownInnerB) const;
	void computeVerticalProfileBlocks(const int nBlocks, const Index &center, std::vector<float> &profileLeftOuterB, std::vector<float> &profileLeftInnerB, std::vector<float> &profileRightOuterB, std::vector<float> &profileRightInnerB) const;


	/** Returns the sum of the pixel values.  */
	double sum() const
		{ return (cvSum(mpImage)).val[0]; }

	/** Returns the average of the pixel values.  */
	double avg() const
		{ return (cvAvg(mpImage)).val[0]; }


	// VISUALIZATION:
	/**
	 * Shows this image in a named window using the highgui module of OpenCV.
	 * @param windowTitle The title of the window.
	 * @param Flags of the window. Currently the only supported flag is
	 * CV_WINDOW_AUTOSIZE. If it is , window size is automatically adjusted to
	 * fit the displayed image.
	 */
	void show(const std::string& windowTitle, int flags=CV_WINDOW_AUTOSIZE ) const
	{
		cvNamedWindow(windowTitle.c_str(), flags);
		cvShowImage(windowTitle.c_str(), mpImage );
	}
	/** Determines the depth (i.e. nr. of bytes) of this image and returns */
	static int determineDepth() /*const*/;

	// DRAWING FUNCTIONS:
	void drawLine(const Index &pt1, const Index &pt2, const T value,
			const int thickness = 1, const LineType lineType = EIGHT_CONNECTED, const int shift = 0)
	{
		cvLine( mpImage, pt1.toCvPoint(), pt2.toCvPoint(), cvScalar(value), thickness, lineType, shift );
	}
	void drawRectangle( const Index &pt1, const Index &pt2, const T value,
			const int thickness=1, const LineType lineType = EIGHT_CONNECTED, const int shift = 0)
	{
		cvRectangle( mpImage, pt1.toCvPoint(), pt2.toCvPoint(), cvScalar(value),
                thickness, lineType, shift );
	}
	void drawBoundingBox( const BoundingBox& bbox, const T value,
			const int thickness=1, const LineType lineType = EIGHT_CONNECTED, const int shift = 0 )
	{
		drawRectangle( bbox.leftUpperIndex(), bbox.rightLowerIndex(), value, thickness, lineType, shift );
	}

	// FRIENDS:
	friend class ImageOperations;
	friend class ImageFilter;
	friend class Morphology;
	friend class ImageMoments;
	friend class OpenCVGUI;
protected:
	// PROTECTED MEMBERS:
	IplImage* mpImage;

	// PROTECTED FUNCTIONS:
	/** Creates a new image with specified width, height. */
	void createImage(const int width, const int height)
		{ mpImage = cvCreateImage( cvSize(width, height), determineDepth(), 1 ); }

	/** Releases the currently  image. */
	void releaseImage()
		{
		if (mpImage) {
			cvReleaseImage(&mpImage);
			mpImage = 0x0;
			}
		}
private:
	// PRIVATE FUNCTIONS:


	// PRIVATE MEMBERS:
//	IplImage* mpImage;

	// STATIC MEMBERS:
	static const std::bad_typeid unsupportedTypeException;
//	static const bad_typeid unsupportedTypeException;

}; // end of class GrayImage

template <>
GrayImage<uint8>& GrayImage<uint8>::init(const QImage& qImage);

// util functions for GrayImage:
void writeClusterImage( const std::vector<CharCluster> &clusters, GrayImage<uint8> &resultImage );
void writeOneClusterImage( const CharCluster &cluster, GrayImage<uint8> &resultImage, const int maxWidth = 1000);
void writeClusterImageBlocked( const ClusteringResult &clusteringResult, GrayImage<uint8> &resultImage, const int maxClustW = 1500 );
const std::string createSegmentationResultFileName(const std::string inputFn, const std::string fnType="xml", const std::string prefix="", const std::string suffix="");
bool makeWhitespaceImageOne(const GrayImage<uint8> &inputImage, GrayImage<uint8> &resultImage, double thresh = 0.05);
double signatureDistance(const GrayImage<uint8> &im1, const GrayImage<uint8> &im2, const GrayImage<float> &distTransf1, const GrayImage<float> &distTransf2, const GrayImage<float> &intInv1, const GrayImage<float> &intInv2);

template <class T>
/*static*/ const std::bad_typeid GrayImage<T>::unsupportedTypeException
	= std::bad_typeid("Type not supported in GrayImage! Supported types are: uint8, int8, uint16, int16, int32, float, double.");

// outstream operator:
template <class T>
std::ostream& operator<<(std::ostream& os, const GrayImage<T>& image)
{
	os << "GrayImage: Rows x Columns = " << image.rows() << " x " << image.columns() << "\n";
	os << "Data:" << "\n";
#if 0 // version without iterator
//	for (int i=0; i<image.rows(); ++i) {
//		for (int j=0; j<image.columns(); ++j) {
	std::cout << image.yRoi() << ", " << image.heightRoi() << std::endl;
	std::cout << image.xRoi() << ", " << image.widthRoi() << std::endl;

	for (int i=image.yRoi(); i<image.yRoi()+image.heightRoi(); ++i) {
		for (int j=image.xRoi(); j<image.xRoi()+image.widthRoi(); ++j) {
//			os << image.get(i,j) << " ";
			os << (double)(image(i,j)) << " ";
		}
		os << "\n";
	}
#else // version with iterator
	ImageIterator<T> it = image.begin();
	ImageIterator<T> endIt = image.end();
	int actRow = it.row();
	while (it != endIt) {
		os << (double)(*it) << " ";
		++it;
		if (actRow != it.row()) {
			os << "\n";
			actRow = it.row();
		}
	} // end while
#endif
	os << std::endl;

	return os;
} // end operator<< for Images

template <class T>
void GrayImage<T>::resize(const int width, const int height, int interpolation/*=CV_INTER_LINEAR*/)
{
	if ( !empty() ) {
//		cvResize(mpImage, mpImage, interpolation);

#if 1
//		std::cout << "resizing..." << std::endl;
		// store old image pointer:
		IplImage* pImageOld = mpImage;
		// create a new image with specified size:c
		mpImage = cvCreateImage( cvSize(width, height), determineDepth(), 1 );

		cvResize(pImageOld, mpImage, interpolation);
		if (pImageOld != 0x0) {
			cvReleaseImage(&pImageOld);
		}
#endif
	}
	else {
		releaseImage();
		mpImage = cvCreateImage( cvSize(width, height), determineDepth(), 1 );
	}
}

template <class T>
void GrayImage<T>::binarize(double thresholdValue/*=128*/, double maxValue/*=255*/, ThreshType thresholdType/*=THRESH_BINARY*/)
{
	if (this->empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return;
	}

	cvThreshold(this->mpImage, this->mpImage, thresholdValue, maxValue, thresholdType);
	return;
}

/**
 * Binarize this image using Otsu's method.
 */
template <class T>
void GrayImage<T>::binarizeOtsu(double maxValue/*=255*/, ThreshType thresholdType/*=THRESH_BINARY*/)
{
	if (this->empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return;
	}

	cvThreshold(this->mpImage, this->mpImage, 0, maxValue, thresholdType | CV_THRESH_OTSU);
	return;
}

template <class T>
void GrayImage<T>::adaptiveBinarize(double maxValue/*=255*/, int adaptiveMethod/*=CV_ADAPTIVE_THRESH_MEAN_C*/, int blockSize/*=3*/, double param1/*=5*/)
{
	if (this->empty()) {
		std::cerr << "Warning: The given image is empty!" << std::endl;
		return;
	}

	cvAdaptiveThreshold(this->mpImage, this->mpImage, maxValue, adaptiveMethod, CV_THRESH_BINARY, blockSize, param1);

	return;
}

template <class T>
void GrayImage<T>::computeNonZeroVec(ublas::vector<T> &nonZeroVec) const
{
	nonZeroVec.clear();
#if 0
	for (int i=0; i<rows(); ++i) {
		for (int j=0; j<columns(); ++j) {
			T val = get(i,j);
			if (val!=0) nonZeroVec.push_back( val );
		}
	}
#else
	ImageIterator<T> it = this->begin();
	ImageIterator<T> endIt = this->end();
	int actRow = it.row();
	while (it != endIt) {
		T val = (*it);
		if (val!=0) {
			nonZeroVec.resize(nonZeroVec.size()+1);
			nonZeroVec[nonZeroVec.size()-1] = val;
		}
		++it;
	} // end while
#endif
	return;
} // end computeNonZeroVec(...)

template <class T>
void GrayImage<T>::computeNonZeroIndexVec(std::vector<Index> &nonZeroIndexVec) const
{
	nonZeroIndexVec.clear();

	for (int i=0; i<rows(); ++i) {
		for (int j=0; j<columns(); ++j) {
			T val = get(i,j);
			if (val!=0) nonZeroIndexVec.push_back( Index(i,j) );
		}
	}

	return;
}

template <class T>
int GrayImage<T>::set( const size_t r, const size_t c, const GrayImage<T>& subImage )
{
	assert( isValidIndex(r,c) );

	const int endRow = r+subImage.heightRoi()-1;
	const int endCol = c+subImage.widthRoi()-1;

	if ( (endRow >= height()) || (endCol >= width()) ) {
		std::cout << "Error in set(r,c,subImage): Given sub-image does not fit into existing one - leaving image unchanged." << std::endl;
		return -1;
	}

	BoundingBox roiBox( c, r, subImage.widthRoi(), subImage.heightRoi() );
	this->setRoi(roiBox);
	cvCopy( subImage.mpImage, mpImage );
	this->releaseRoi();

	// TODO
//	const int endRow = ((r+subImage.heightRoi()) < height()) : r+subImage.heightRoi() ? height()-1;
//	const int endCol = ((c+subImage.widthRoi()) < width()) : c+subImage.widthRoi() ? width()-1;
//
//	for (int i=r; i<endRow; ++i) {
//		for (int j=c; j<endCol; ++j) {
//			(*this)(i,j) = subImage(i-r, j-c);
//		}
//	} // end for all rows i

	return 0;
} // end (const size_t i, const size_t j, const GrayImage<T>& subImage)

// Define general functions with specializations to throw an unsupported type exception,
// when no specialization is given:
//template <class T>
//GrayImage<T>& GrayImage<T>::init(const QImage& qImage)
//{ throw std::bad_typeid("init(QImage&) operation only supported for uint8 images!"); }

template <class T>
inline
int GrayImage<T>::determineDepth() /*const*/
{ throw GrayImage<T>::unsupportedTypeException; return -1; }

template <class T>
inline
void GrayImage<T>::save(const std::string& fileName) const
{ throw std::bad_typeid("Save operation only supported for uint8 images!"); return -1; }

template <class T>
inline
void
GrayImage<T>::invert()
{ throw std::bad_typeid("Invert operation only supported for uint8 images!"); return 0x0; }

template <class T>
void GrayImage<T>::computeCenterOfMass(Vector2D<double> &centerOfMass, const bool binary /*= true*/) const
{
	CvMoments moments;
	cvMoments(this->mpImage, &moments, binary);
	centerOfMass(0) = moments.m10 / moments.m00;
	centerOfMass(1) = moments.m01 / moments.m00;
	return;
}

template <class T>
void GrayImage<T>::computeCenterOfMassIndex(Index &centerOfMassIndex, const bool binary /*= true*/) const
{
	Vector2D<double> center;
	computeCenterOfMass(center, binary);
	centerOfMassIndex(0) = (int)round(center(1));
	centerOfMassIndex(1) = (int)round(center(0));
	return;
}

// INLINE TEMPLATE SPECIALIZATIONS:
////////////////////////////////////////////////////////////////////////////
// load specializations:
template <>
inline void
GrayImage<uint8>::load(const std::string& fileName)
{
#if 0
#if 0
	releaseImage();
	std::cout << "in load: " << fileName.c_str() << std::endl;
	mpImage = cvLoadImage( fileName.c_str(), CV_LOAD_IMAGE_GRAYSCALE );
	if (!mpImage) {
		throw ImageException("Could not read image with filename "+fileName);
	}
#else
	// Note: this version of the constructor using QImage is a workaround,
	// since cvLoadImage does not seem to work with .tif files
	mpImage = 0x0;
	QImage qImage(fileName.c_str());
	if (!qImage.isNull()) {
		this->init(qImage);
	}
	else {
		throw ImageException("Could not read image with filename "+fileName);
	}
#endif
#else
	if (!fileExists( fileName )) {
		throw FileNotFoundException("File does not exist in GrayImage::load ("+fileName+")");
	}
	releaseImage();
	mpImage = cvLoadImage( fileName.c_str(), CV_LOAD_IMAGE_GRAYSCALE );
	if (!mpImage) {
		std::cout << "Could not read image using cvLoadImage ... switching to Qt for loading image!" << std::endl;
		// Note: this version of the constructor using QImage is a workaround,
		// since cvLoadImage does not seem to work with .tif files
		mpImage = 0x0;
		QImage qImage(fileName.c_str());
		if (!qImage.isNull()) {
			this->init(qImage);
		}
		else {
			throw ImageException("Could not read image with filename "+fileName);
		}
	} // end if !mpImage
#endif
} // end GrayImage<uint8>(const std::string& fileName)
////////////////////////////////////////////////////////////////////////////
// determineDepth specializations:


template <>
inline
int GrayImage<uint8>::determineDepth() /*const*/
{
	return IPL_DEPTH_8U;
}

template <>
inline
int GrayImage<int8>::determineDepth() /*const*/
{
	return IPL_DEPTH_8S;
}

template <>
inline
int GrayImage<uint16>::determineDepth() /*const*/
{
	return IPL_DEPTH_16U;
}

template <>
inline
int GrayImage<int16>::determineDepth() /*const*/
{
	return IPL_DEPTH_16S;
}

template <>
inline
int GrayImage<int32>::determineDepth() /*const*/
{
	return IPL_DEPTH_32S;
}

template <>
inline
int GrayImage<float>::determineDepth() /*const*/
{
	return IPL_DEPTH_32F;
}

template <>
inline
int GrayImage<double>::determineDepth() /*const*/
{
	return IPL_DEPTH_64F;
}

////////////////////////////////////////////////////////////////////////////
// save function specializations:

template <>
inline
void GrayImage<uint8>::save(const std::string& fileName) const
{
	if(!cvSaveImage(fileName.c_str(), mpImage))
		throw ImageException("Could not save image in GrayImage::save!");
}

////////////////////////////////////////////////////////////////////////////
// invert function specializations:

template <>
inline
void
GrayImage<uint8>::invert()
{
	cvSubRS( mpImage, cvScalar(255), mpImage);
} // end of ImageFilter::invert()

} // end of namespace impact_tr4

#endif /*GRAYIMAGE2_HPP_*/
