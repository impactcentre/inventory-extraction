/*
 * PreprocessingResults.hpp
 *
 *  Created on: 15.11.2010
 *      
 */

#ifndef PREPROCESSINGRESULTS_HPP_
#define PREPROCESSINGRESULTS_HPP_

#include "core_headers.h"
//#include "DataStructs.hpp"

namespace impact_tr4 {

class PreprocessingParameters;
class ImageChar;

class PreprocessingResults
{
public:
	PreprocessingResults()
		{
		mpProcessedImage = NULL;
		mpIntInvImage = NULL;
		mpDistTransformImage = NULL;
		isWhiteSpace = false;
		}
	~PreprocessingResults() { clear(); }
	bool isEmpty() const { return mpProcessedImage == NULL; }

	void computePreprocessings(	const PreprocessingParameters parameters,
								ImageChar *pImageChar);
	void computePreprocessings(	const PreprocessingParameters parameters,
								const GrayImage<> &glyphImage);

	static void computeOtherPreprocessings(
			const GrayImage<> &preprocessedImage, GrayImage<float> &intInvImage, GrayImage<float> &distTransformImage);

	void clear()
		{
		delete mpProcessedImage; mpProcessedImage = NULL;
		delete mpIntInvImage; mpIntInvImage = NULL;
		delete mpDistTransformImage; mpDistTransformImage = NULL;
		}

	static void preprocessGlyph(const PreprocessingParameters parameters, const GrayImage<> &src, GrayImage<> &dst, bool *whitespace=NULL);

	GrayImage<> *mpProcessedImage;
	GrayImage<float> *mpIntInvImage;
	GrayImage<float> *mpDistTransformImage;
	bool isWhiteSpace;
}; // end class PreprocessingResults

} // end namespace impact_tr4

#endif /* PREPROCESSINGRESULTS_HPP_ */
