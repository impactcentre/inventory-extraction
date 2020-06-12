/*
 * Document.cpp
 *
 *  Created on: 05.08.2009
 *      
 */

#include <omp.h>

#include "Document.hpp"
#include "data_reader/FRXMLCharDataReader.hpp"
#include "data_reader/Im2CharRectsCharDataReader.hpp"
#include "data_reader/DatFileDataReader.hpp"
#include "core/image/ImageFilter.hpp"
#include "util/util.h"

namespace impact_tr4 {

Document::Document()
{
	mpCharDataReader = 0x0;

	clearDocument();

	// set functions pointer of distance function to signature distance initially:
	mfpDistance = &Document::sigDist;
}

Document::~Document()
{
	clearDocument();
}

void Document::setDocumentFileNames(const std::vector<std::string>& fileNames)
{
	// clear document first:
	clearDocumentInputData();

	for (int i=0; i<fileNames.size(); ++i) {
		if (!fileExists(fileNames[i])) {
			throw FileNotFoundException("Error: image file not found!");
		}
	}
	mFileNameVec = fileNames;
	for (int i=0; i<mFileNameVec.size(); ++i) {
		mImageVec.push_back(0x0);
	}
	mIsDocumentImagesLoaded = false;
	mIsSegmentationResultFilesSet = false;
}

void Document::openDocumentImages()
{
	#pragma omp parallel for
	for (int i=0; i<mFileNameVec.size(); ++i) {
		delete mImageVec[i];
		mImageVec[i] = new GrayImage<>(mFileNameVec[i]);
	}
	mIsDocumentImagesLoaded = true;
	return;
} // end openDocumentImages()

void Document::clearDocument()
{
	clearDocumentInputData();

	clearImageChars();
	clearAllPreprocessing();

	return;
} // end closeDocumentImages()

void Document::clearDocumentInputData()
{
	for (int i=0; i<mImageVec.size(); i++) {
		delete mImageVec[i];
		mImageVec[i] = 0x0;
	}
	mImageVec.clear();
	mFileNameVec.clear();
	mSegmentationResultFileNameVec.clear();
	mIsDocumentImagesLoaded = false;
	mIsSegmentationResultFilesSet = false;
	mParsingParameters = ParsingParameters();
//	mSegmentationResultParsingMethod = UNKNOWN_PARSING_METHOD;

	return;
}

void Document::createSegmentationResultFileNames(const std::string fnType/*="xml"*/, const std::string prefix/*=""*/, const std::string suffix/*=""*/)
{
	mSegmentationResultFileNameVec.clear(); // clear existing filenames
	// create XML filenames:
	for (int i=0; i<nFiles(); ++i) {
		std::string fnSeg = createSegmentationResultFileName(fileName(i), fnType, prefix, suffix);
		std::cout << "segmentation filename is: " << fnSeg << std::endl;
		if (fileExists(fnSeg)) {
			// store filenames for xml data:
			mSegmentationResultFileNameVec.push_back(fnSeg);
		}
		else {
			throw FileNotFoundException("Segmentation result file not found for file " + fileName(i));
		}
	} // end for all files i
	mIsSegmentationResultFilesSet = true;

	return;
}

void Document::parseInputFiles(const ParsingParameters parsingParameters, const std::vector<std::string>& inputFns)
{
	// Set parsing filenames and open corresponding images:
	this->clearDocumentInputData();
	this->setDocumentFileNames(inputFns);
	this->openDocumentImages();
//	this->setSegmentationResultParsingMethod(mParsingParameters.parsingMethod);

	mParsingParameters = parsingParameters;
	// create input file parser:
	delete mpCharDataReader; mpCharDataReader = 0x0;
	std::string errorMsg;
//	std::cout << "parsing method in Document: " << mParsingParameters.parsingMethod << std::endl;
	switch (mParsingParameters.parsingMethod) {
		case FINEREADER_XML:
			mpCharDataReader = new FRXMLCharDataReader();
			break;
		case IM2CHARRECTS_XML:
			mpCharDataReader = new Im2CharRectsCharDataReader();
			break;
		case DAT_FILES:
			mpCharDataReader = new DatFileDataReader();
			break;
		default:
			errorMsg = "Error parsing input files - unknown method!";
			throw Exception(errorMsg);
			break;
	}
//	mImageCharVec.clear();
	clearImageChars();
	std::vector<ImageChar> tmpImageCharVec;
	mpCharDataReader->parseFiles(this, tmpImageCharVec);
	const int nParsedChars = tmpImageCharVec.size();
	if (nParsedChars <= 0) {
		throw Exception("No Characters parsed!");
	}
	for (int i=0; i<nParsedChars;++i) {
		this->addImageChar(tmpImageCharVec[i]);
	}

	// set document pointer for all parsed chars:
	for (int i=0; i<nParsedChars; ++i) {
		mImageCharVec[i]->setDocumentPointer(this);
	}

	std::cout 	<< "Successfully created input file parser and parsed "
				<< nParsedChars
				<< " nr. of characters!" << std::endl;
	return;
} // end parseInputFiles

ImageChar* Document::addImageChar(const ImageChar& imChar)
{
	mImageCharVec.push_back(0x0);
	mImageCharVec[mImageCharVec.size()-1] = new ImageChar(imChar);
//	mImageCharVec.push_back(imChar);
	return mImageCharVec[mImageCharVec.size()-1];
}

void Document::clearImageChars()
{
	for (int i=0; i<mImageCharVec.size(); ++i) {
		delete mImageCharVec[i]; mImageCharVec[i]=NULL;
	}
	mImageCharVec.clear();
	return;
}

void Document::updateImagePointers()
{
	for (int i=0; i<mImageCharVec.size(); ++i) {
		mImageCharVec[i]->pImage = this->imagePointer(mImageCharVec[i]->imageID);
	}
	return;
}

void Document::printImageChars()
{
	for (int i=0; i<mImageCharVec.size(); ++i) {
		std::cout << *mImageCharVec[i] << std::endl;
	}
}

#if 0
void Document::loadImageCharsFromClusteringResult(const ClusteringResult& clusteringResult)
{
	const int nClusts = clusteringResult.nClusters();
	std::vector<ImageChar> tmpImageCharVec;

#if 0
	// first: gather all image chars from clusters
	for (int i=0; i<nClusts; ++i) {
		const CharCluster *pActClust = clusteringResult[i];
		const std::list<ImageChar> &charlistref = pActClust->charList();
		std::list<ImageChar>::const_iterator it = charlistref.begin();
		for (;it!=charlistref.end(); ++it) {
			tmpImageCharVec.push_back(*it);
		}
	} // end for all clusters i

	// second: reorder according to ImageChar id:
	mImageCharVec.resize(tmpImageCharVec.size());
	for (int i=0; i<mImageCharVec.size(); ++i) {
		if (tmpImageCharVec[i].id < 0 || tmpImageCharVec[i].id >= mImageCharVec.size()) {
			throw Exception("Wrong ImageChar id while loading clustering result in document!");
		}
		mImageCharVec[tmpImageCharVec[i].id] = tmpImageCharVec[i];
	}
#else // just put all chars in mImageCharVec, elsewise there will be problems with deleted clusters...
	if (!mImageCharVec.empty()) { throw Exception("Error while loading glyphs into document - document is not empty!"); }
	for (int i=0; i<nClusts; ++i) {
		const CharCluster *pActClust = clusteringResult[i];
		const std::list<ImageChar> &charlistref = pActClust->charList();
		std::list<ImageChar>::const_iterator it = charlistref.begin();
		for (;it!=charlistref.end(); ++it) {
			mImageCharVec.push_back(*it);
		}
	} // end for all clusters i
#endif

	return;
}
#endif

void Document::preprocessAllGlyphs()
{
	StopWatch watch;
#if 1
	watch.start();
	this->clearAllPreprocessing();
	#pragma omp parallel for
	for (int i=0; i<mImageCharVec.size(); ++i) {
		this->preprocessGlyph(i);
		std::cout << "processed glpyh nr " << i+1 << " of " << mImageCharVec.size() << std::endl;
	}
	watch.stop();
#else
	const int nParsedChars = mImageCharVec.size();

	watch.start();
	this->clearAllPreprocessing();
//	mProcessedImagesVec.clear();
//	mIntInvImagesVec.clear();
//	mDistTransformImages.clear();

	mProcessedImagesVec.resize(nParsedChars, 0x0);
	mIntInvImagesVec.resize(nParsedChars, 0x0);
	mDistTransformImages.resize(nParsedChars, 0x0);

	GrayImage<> *pImage;
	BoundingBox bbox;
	GrayImage<> glyphImage, preprocessedGlyphImage;
	GrayImage<float> intInvImage;
	Kernel<> mask;
	GrayImage<float> distTransformImage;
	GrayImage<> invPreprocessedGlyphImage;

	int i=0;
	#pragma omp parallel for private(i, pImage, bbox, glyphImage,preprocessedGlyphImage,intInvImage, mask,distTransformImage,invPreprocessedGlyphImage)
	for (i=0; i<nParsedChars; ++i) {
//	for (int i=0; i<2; ++i) {
		// extract glyph from document image:
		pImage = mImageCharVec[i]->pImage;
		bbox = mImageCharVec[i]->bBox;

//		printf("Hello World from thread %d\n", omp_get_thread_num());

//		std::cout << bbox << std::endl;
		#pragma omp critical // critical region, since a pointer to common images is set
		{
		pImage->setRoi(bbox);
		pImage->computeRoiImage(glyphImage);
		pImage->releaseRoi();
		}
//		printf("Hello World from thread %d\n", omp_get_thread_num());

		// preprocess glyph image:
		PreprocessingResults::preprocessGlyph(mPreprocessingParameters, glyphImage, preprocessedGlyphImage);
		mProcessedImagesVec[i] = new GrayImage<>(preprocessedGlyphImage);
		mImageCharVec[i]->pPreprImage = mProcessedImagesVec[i];
//		std::cout << "finished processing image " << i << std::endl;
//		std::cout << mProcessedImagesVec[i] << std::endl;

#if 1
		// compute integral invariant image:
		int radius = 2; bool oddSize = true;
		Kernel<>::createSphereKernel( mask, radius, oddSize );
//		std::cout << mask << std::endl;
		ImageIntegralInvariant::calcIntegralInvariantImage( mask, preprocessedGlyphImage, intInvImage );
		mIntInvImagesVec[i] = new GrayImage<float>(intInvImage);
//		intInvImage.show("intInvImage");
//		std::cout << intInvImage << std::endl;
#endif

#if 1
		// compute distance transform of inverted(!) glyph:
		invPreprocessedGlyphImage = preprocessedGlyphImage;
		invPreprocessedGlyphImage.invert();
		ImageFilter::distTransform(invPreprocessedGlyphImage, distTransformImage);
		mDistTransformImages[i] = new GrayImage<float>(distTransformImage);
#endif

#if 0
		// TEST: save preprocessed images:
		std::cout << "saving pp images... ";
		QString fn = "c:/foo/workspace/test_save/pp_" + QString::number(i) + ".jpg";
		mProcessedImagesVec[i].save(fn.toStdString());
//		preprocessedGlyphImage.save(fn.toStdString());
		std::cout << "done!" << std::endl;
#endif
		std::cout << "processed glpyh nr " << i+1 << " of " << nParsedChars << std::endl;
	} // end for all images i
	watch.stop();
	std::cout << "finished processing " << nParsedChars << " images!" << std::endl;
#endif

	return;
} // end preprocessGlyphs

void Document::clearAllPreprocessing()
{
#if 1
	for (int i=0; i<mImageCharVec.size(); ++i) {
		mImageCharVec[i]->mPreprocessingResults.clear();
	}
#else // old version
	for (int i=0; i<mProcessedImagesVec.size(); ++i) {
		delete mProcessedImagesVec[i];
	}
	mProcessedImagesVec.clear();
	for (int i=0; i<mIntInvImagesVec.size(); ++i) {
		delete mIntInvImagesVec[i];
	}
	mIntInvImagesVec.clear();
	for (int i=0; i<mDistTransformImages.size(); ++i) {
		delete mDistTransformImages[i];
	}
	mDistTransformImages.clear();
#endif

	return;
}

void Document::preprocessGlyph(const int i)
{
	assert(i>=0 && i<this->mImageCharVec.size());

	mImageCharVec[i]->mPreprocessingResults.computePreprocessings(mPreprocessingParameters, mImageCharVec[i]);
	return;
}

void Document::clearPreprocessing(const int i)
{
	assert(i>=0 && i<this->mImageCharVec.size());

	mImageCharVec[i]->mPreprocessingResults.clear();
	return;
}

bool Document::hasGlyphPreprocessing(const int i)
{
	assert(i>=0 && i<this->mImageCharVec.size());

	return (!mImageCharVec[i]->mPreprocessingResults.isEmpty());
}


void Document::computeFeatures()
{
	// 1: preprocess all glyphs:
	this->preprocessAllGlyphs();

	// 2: compute data:
//	mCharFeatureCollection.calculateOutputDataMatrix( mProcessedImagesVec );
	mCharFeatureCollection.calculateOutputDataMatrix( mImageCharVec );
	// reduce data dimensionality using PCA:
	if (mPCAParameters.newDim > 0) {
		mCharFeatureCollection.reduceMatrixDimensionsNewDim(mPCAParameters.newDim);
		std::cout << "Successfully reduced data dimension (newDim)!" << std::endl;
	}
	else if (mPCAParameters.threshold < 1 && mPCAParameters.threshold > 0) { // use threshold if new dim not stated explicitely
		mCharFeatureCollection.reduceMatrixDimensionsThreshold(mPCAParameters.threshold);
		std::cout << "Successfully reduced data dimension (threshold)!" << std::endl;
	}
	std::cout << "Successfully computed data matrix for given features!" << std::endl;

	return;
} // end createFeature

void Document::clearFeatures()
{
	this->mCharFeatureCollection.clearDataMatrix();
	return;
}

void Document::storeDataMatrix(const std::string& fn)
{
	if ( !mCharFeatureCollection.hasDataMatrix() ) {
		std::cout << "No data matrix in ClusteringController::storeDataMatrix!" << std::endl;
		throw NoDataException("No data matrix available to save!");
	}
	writeTxtFile(mCharFeatureCollection.dataMatrixRef(), fn);

	return;
} // end storeDataMatrix

double Document::sigDist(const int i, const int j) const
{
//	std::cout << "using signature distance" << std::endl;

#if 0
	double sigDist = signatureDistance(	*mProcessedImagesVec[i],
										*mProcessedImagesVec[j],
										*mDistTransformImages[i],
										*mDistTransformImages[j],
										*mIntInvImagesVec[i],
										*mIntInvImagesVec[j] );
#else
	PreprocessingResults *pP1 = &mImageCharVec[i]->mPreprocessingResults;
	PreprocessingResults *pP2 = &mImageCharVec[j]->mPreprocessingResults;

	assert(!pP1->isEmpty() && !pP2->isEmpty());

	double sigDist = 0;
	if (pP1->isWhiteSpace && pP2->isWhiteSpace) {
		sigDist = 0;
	}
	else if ((!pP1->isWhiteSpace && pP2->isWhiteSpace) || (pP1->isWhiteSpace && !pP2->isWhiteSpace)) {
		sigDist = 9999999;
	}
	else {
		sigDist = signatureDistance(	*pP1->mpProcessedImage,
										*pP2->mpProcessedImage,
										*pP1->mpDistTransformImage,
										*pP2->mpDistTransformImage,
										*pP1->mpIntInvImage,
										*pP2->mpIntInvImage );
	}
#endif

	return sigDist;
}

double Document::featureVecEuclDistance(const int i, const int j) const
{
//	std::cout << "using euclidean distance" << std::endl;
	double euclDist = 0;

	const ublas::matrix_row<const ublas::matrix<float> > vec1(mCharFeatureCollection.dataMatrixRef(), i);
	const ublas::matrix_row<const ublas::matrix<float> > vec2(mCharFeatureCollection.dataMatrixRef(), j);

	euclDist = ublas::norm_2(vec1 - vec2);

	return euclDist;
}

void Document::setDistanceType(const ClusteringDataType dataType)
{
	switch (dataType) {
		case DISTANCE_BASED:
			mfpDistance = &Document::sigDist;
			break;
		case FEATURES_BASED:
			mfpDistance = &Document::featureVecEuclDistance;
			break;
		default:
			std::cout << "Error: unknown distance type in Document::setDistanceType(...)!" << std::endl;
			throw Exception("Unkwon distance type!");
			break;
	} // end switch
	return;
}

} // end of namespace impact_tr4
