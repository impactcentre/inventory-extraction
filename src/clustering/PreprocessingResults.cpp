/*
 * PreprocessingResults.cpp
 *
 *  Created on: 15.11.2010
 *      
 */

#include "PreprocessingResults.hpp"
#include "DataStructs.hpp"

namespace impact_tr4 {

/*static*/ void PreprocessingResults::preprocessGlyph(
												const PreprocessingParameters parameters,
												const GrayImage<> &src,
												GrayImage<> &dst,
												bool *whitespace/*=NULL*/
												)
{
	//		std::cout << preprocessedGlyphImage << std::endl;
			GrayImage<> tmpImage1 = src;
//			dst = src;

			if (parameters.binarize) { // binarize image
				tmpImage1.binarizeOtsu();
			}
			if (parameters.invert) { // invert image
				tmpImage1.invert();
			}
			if (parameters.preMedian) { // do pre median filtering
				dst = tmpImage1;
				ImFi::median(dst, tmpImage1, parameters.preMedianMaskSize);
			}

//			tmpImage1.show("nachinvert");

			double thresh = 0.05;
			double pixelDens = tmpImage1.pixelDensity();
			bool isWs = false;
			if (pixelDens < thresh) { isWs = true; }
			if (whitespace != NULL) *whitespace = isWs;

//			dst = tmpImage1;
//			isWs = makeWhitespaceImageOne(dst, tmpImage1, thresh);
			if (!isWs) {
//			tmpImage1.show("nachwhitespace");

	//		std::cout << tmpImage1 << std::endl;
			// do size normalization:
			dst = tmpImage1;
			uint8 padValue = 0;

#if 0
			// compute center of mass index:
			Index ctrOfMassIndex;
			dst.computeCenterOfMassIndex(ctrOfMassIndex, true);
			// center image on center of mass:
			ImOp::extendToCenter(dst, tmpImage1, ctrOfMassIndex, 0, padValue);
#else
			dst.computeCenterOfMassCenteredImage(tmpImage1, padValue);
//			tmpImage1.show("nachcenterofmass");
#endif
	//		std::cout << tmpImage2 << std::endl;
		 // 		CV_INTER_NN - nearest-neigbor interpolation,
		 // 		CV_INTER_LINEAR - bilinear interpolation (used by default)
		 // 		CV_INTER_AREA - resampling using pixel area relation. It is preferred method for image decimation that gives moire-free results. In case of zooming it is similar to CV_INTER_NN method.
		 // 		CV_INTER_CUBIC - bicubic interpolation.
			tmpImage1.resize(parameters.sizeNormWidth, parameters.sizeNormHeight, CV_INTER_LINEAR);
//			tmpImage1.show("nachresize");
//			std::cout << tmpImage1 << std::endl;

			if (parameters.postMedian) { // do pre median filtering
				dst = tmpImage1;
				ImFi::median(dst, tmpImage1, parameters.postMedianMaskSize);
			}

			tmpImage1.binarize();
//			tmpImage1.show("nachbinarize");

			dst = tmpImage1;
			} // end if not whitespace
			else {
				dst.init(parameters.sizeNormWidth, parameters.sizeNormHeight);
				dst.fill(0);
				dst(0,0) = 255;
			}

			return;
} // end preprocessGlyph

/*static*/
void PreprocessingResults::computeOtherPreprocessings(
		const GrayImage<> &preprocessedImage, GrayImage<float> &intInvImage, GrayImage<float> &distTransformImage)
{
#if 1
	// 2nd: compute integral invariant image:
	int radius = 2; bool oddSize = true;
	Kernel<> mask; Kernel<>::createSphereKernel( mask, radius, oddSize );
//	std::cout << mask << std::endl;
	ImageIntegralInvariant::calcIntegralInvariantImage( mask, preprocessedImage, intInvImage );
#endif

#if 1
	// 3rd: compute distance transform of inverted(!) glyph:
	GrayImage<> invPreprocessedGlyphImage = preprocessedImage;
	invPreprocessedGlyphImage.invert();
	ImageFilter::distTransform(invPreprocessedGlyphImage, distTransformImage);
#endif
	return;
}

void PreprocessingResults::computePreprocessings(	const PreprocessingParameters parameters,
							const GrayImage<> &glyphImage)
{
	this->clear();

	// 1st: preprocess glyph image:
	mpProcessedImage = new GrayImage<>();
	PreprocessingResults::preprocessGlyph(parameters, glyphImage, *mpProcessedImage, &isWhiteSpace);
//	pImageChar->pPreprImage = mpProcessedImage;
//	std::cout << "finished processing image " << i << std::endl;
//	std::cout << mProcessedImagesVec[i] << std::endl;

	if (isWhiteSpace) { // jump out as this is a whitespace character
		return;
	}

#if 1
	// 2nd: compute integral invariant image:
	int radius = 2; bool oddSize = true;
	Kernel<> mask;
	Kernel<>::createSphereKernel( mask, radius, oddSize );
//	std::cout << mask << std::endl;
	mpIntInvImage = new GrayImage<float>();
	ImageIntegralInvariant::calcIntegralInvariantImage( mask, *mpProcessedImage, *mpIntInvImage );
#endif

#if 1
	// 3rd: compute distance transform of inverted(!) glyph:
	GrayImage<> invPreprocessedGlyphImage = *mpProcessedImage;
	invPreprocessedGlyphImage.invert();
	mpDistTransformImage = new GrayImage<float>();
	ImageFilter::distTransform(invPreprocessedGlyphImage, *mpDistTransformImage);
#endif

	return;
}

void PreprocessingResults::computePreprocessings(const PreprocessingParameters parameters, ImageChar *pImageChar)
{
	this->clear();

	// extract glyph from document image:
	GrayImage<> *pImage = pImageChar->pImage;
	BoundingBox bbox = pImageChar->bBox;
	GrayImage<> glyphImage;
	#pragma omp critical // critical region, since a pointer to common images is set
	{
	pImage->setRoi(bbox);
	pImage->computeRoiImage(glyphImage);
	pImage->releaseRoi();
	}

//	printf("Hello World from thread %d\n", omp_get_thread_num());
//	std::cout << bbox << std::endl;

	// 1st: preprocess glyph image:
	mpProcessedImage = new GrayImage<>();
	PreprocessingResults::preprocessGlyph(parameters, glyphImage, *mpProcessedImage, &isWhiteSpace);
//	pImageChar->pPreprImage = mpProcessedImage;
//	std::cout << "finished processing image " << i << std::endl;
//	std::cout << mProcessedImagesVec[i] << std::endl;

	if (isWhiteSpace) { // jump out as this is a whitespace character
		return;
	}

#if 1
	// 2nd: compute integral invariant image:
	int radius = 2; bool oddSize = true;
	Kernel<> mask;
	Kernel<>::createSphereKernel( mask, radius, oddSize );
//	std::cout << mask << std::endl;
	mpIntInvImage = new GrayImage<float>();
	ImageIntegralInvariant::calcIntegralInvariantImage( mask, *mpProcessedImage, *mpIntInvImage );
#endif

#if 1
	// 3rd: compute distance transform of inverted(!) glyph:
	GrayImage<> invPreprocessedGlyphImage = *mpProcessedImage;
	invPreprocessedGlyphImage.invert();
	mpDistTransformImage = new GrayImage<float>();
	ImageFilter::distTransform(invPreprocessedGlyphImage, *mpDistTransformImage);
#endif

	return;
} // end computePreprocessings(ImageChar *pImageChar)

} // end of namespace impact_tr4

