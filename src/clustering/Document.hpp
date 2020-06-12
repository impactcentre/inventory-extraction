/*
 * Document.hpp
 *
 *  Created on: 05.08.2009
 *      
 */

#ifndef DOCUMENT_HPP_
#define DOCUMENT_HPP_

#include <iostream>
#include <string>

#include "core_headers.h"
#include "image_headers.h"

#include "clustering/DataStructs.hpp"
#include "data_reader/CharDataReaderI.hpp"
#include "clustering/CharFeatureCollection.hpp"
//#include "algorithms/FeatureVectorClusterMethodI.hpp"
#include "algorithms/KMeans.hpp"
#include "algorithms/LeaderFollower.hpp"
#include "algorithms/AgglomerativeClustering.hpp"

namespace impact_tr4 {

class CharDataReaderI;

class Document
{
public:
	Document();
//	Document() { mFileName = ""; mpImage = 0x0; }
	virtual ~Document();
//	Document(const std::vector<std::string>& fileNames);

	void openDocumentImages();
	void clearDocument();
	void clearDocumentInputData();

	void setDocumentFileNames(const std::vector<std::string>& fileNames);

	int nFiles() const { return mFileNameVec.size(); }
	const std::string& fileName(const int i) const { return mFileNameVec[i]; }
	const std::string& segmentationResultFileName(const int i) const { return mSegmentationResultFileNameVec[i]; }

	void setSegmentationResultParsingMethod(const ParsingMethodType parsingMethod)
		{ mParsingParameters.parsingMethod = parsingMethod; }
	ParsingMethodType segmentationResultParsingMethod() const { return mParsingParameters.parsingMethod; }
	ParsingParameters parsingParameters() const { return mParsingParameters; }

	PreprocessingParameters preprocessingParameters() const { return mPreprocessingParameters; }
	void setPreprocessingParameters(const PreprocessingParameters& preprocessingParameters)
		{ mPreprocessingParameters = preprocessingParameters; }

	PCAReductionParams pcaParameters() const { return mPCAParameters; }
	void setPCAParameters(const PCAReductionParams& pars) { mPCAParameters = pars; }

	/// Create segmentation result filenames depending on parsing method
	/**
	 * Creates segmentation result filenames. The parameter strings specify the filename type, prefix or suffix
	 * that are appended to the base filename, s.t. the result filename is matched. Default values are
	 * xml for XML type files and no prefixes or suffixes.
	 */
	void createSegmentationResultFileNames(const std::string fnType="xml", const std::string prefix="", const std::string suffix="");
//	void createXMLSegmentationResultFileNames();

	const std::vector<std::string>& fileNameVec() const { return mFileNameVec; }
	GrayImage<>* imagePointer(const int i) { return mImageVec[i]; }
	void setFileName(const int i, const std::string& fileName) { mFileNameVec[i] = fileName; }
	bool isDocumentImagesLoaded() const { return mIsDocumentImagesLoaded; }
	bool isSegmentationResultFilesSet() const { return mIsSegmentationResultFilesSet; }

	void parseInputFiles(const ParsingParameters parsingParameters, const std::vector<std::string>& inputFns);
//	void loadImageCharsFromClusteringResult(const ClusteringResult& clusteringResult);
	/// Adds a single ImageChar to the end of the list and returns a pointer to it
	ImageChar* addImageChar(const ImageChar& imChar);
	ImageChar* getImageChar(const int i) { return mImageCharVec[i]; }
	void clearImageChars();
	/// Update all image pointers of the ImageChar's in the mImageCharVec vector according to their image-id.
	/// This is used e.g. after parsing a file from a clustering-xml-file
	void updateImagePointers();
	void printImageChars();

	void preprocessAllGlyphs();
	void preprocessGlyph(const int i);
	void clearAllPreprocessing();
	void clearPreprocessing(const int i);
	bool hasGlyphPreprocessing(const int i);

	void computeFeatures();
	void clearFeatures();

	void storeDataMatrix(const std::string& fn);

	// wrapper for function pointer to distance function:
	double computeDistance(const int i, const int j) const
		{ return (this->*mfpDistance)(i,j); }

//	double (Document::*getDistanceFp())(const int i, const int j) const
//			{ return mfpDistance; }

	// actual distance functions:
	/// signature distance function
	double sigDist(const int i, const int j) const;
	/// euclidean distance between feature vectors:
	double featureVecEuclDistance(const int i, const int j) const;

	// sets the type of distance function, i.e. 0 for distance based, 1 for feature based
	void setDistanceType(const ClusteringDataType dataType);

//	void clustering(const ClusterMethodType clusterMethod, const ClusterMethodParameters* pParameters); // TODO

	// getters & setters:
	std::vector<ImageChar*>* getImageCharsVecPointer() { return &mImageCharVec; }
	std::vector<ImageChar*>& getImageCharsVecRef() { return mImageCharVec; }
	int nParsedImages() const { return mImageCharVec.size(); }

//	std::vector<GrayImage<> *>* getProcessedImageVecPointer() { return &mProcessedImagesVec; }
//	std::vector<GrayImage<> *>& getProcessedImageVecRef() { return mProcessedImagesVec; }
//	int nProcessedImages() const { return mProcessedImagesVec.size(); }

	CharFeatureCollection* getCharFeatureCollectionPointer() { return &mCharFeatureCollection; }
	const int nFeatures() const { return mCharFeatureCollection.nFeatures(); }
	const int featuresDim() const { return mCharFeatureCollection.featuresDim(); }

private:
	// MEMBERS:
	// input:
	std::vector<GrayImage<>* > mImageVec;
	std::vector<std::string> mFileNameVec;
	std::vector<std::string> mSegmentationResultFileNameVec;
	ParsingParameters mParsingParameters;
	PreprocessingParameters mPreprocessingParameters;
	PCAReductionParams mPCAParameters;

	bool mIsDocumentImagesLoaded;
	bool mIsSegmentationResultFilesSet;

	// output:
	GrayImage<> mResultImage; // member variable for result image
	CharDataReaderI *mpCharDataReader; // member variable for xml data reader
	std::vector<ImageChar*> mImageCharVec;
	// Preprocessing results:
//	std::vector<GrayImage<> *> mProcessedImagesVec;
//	std::vector<GrayImage<float> *> mIntInvImagesVec;
//	std::vector<GrayImage<float> *> mDistTransformImages;
	// features:
	CharFeatureCollection mCharFeatureCollection;

	ClusterMethodType mClusterMethodType; //< Cluster method (FIXME: needed?)
	// function pointer to distance function
	double (Document::*mfpDistance)(const int i, const int j) const; //< function pointer to distance function

}; // end class Document

} // end of namespace impact_tr4


#endif /* DOCUMENT_HPP_ */
