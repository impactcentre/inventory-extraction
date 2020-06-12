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
#ifndef DATASTRUCTS_HPP_
#define DATASTRUCTS_HPP_

//system includes:
#include <iostream>
// boost includes:
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
// project includes:
#include "core_headers.h"
#include "image_headers.h"
#include "util/util.h"
#include "PreprocessingResults.hpp"
// local includes:
//#include "CharCluster.hpp"

namespace impact_tr4 {

// Forward references:
class Document;
class PreprocessingResults;

enum FeatureDistanceType
{
	EUCLIDEAN_DIST = 0,
	MAHALANOBIS_DIST
};

enum ClusterDistanceType
{
	UNKNOWN_DIST = -1,
	MEAN_DIST = 0,
	WEIGHTED_MEAN_DIST,
	AVG_DIST,
	MIN_DIST,
	MAX_DIST,
	AVG_DIRECT_DIST
};

enum ParsingMethodType
{
	UNKNOWN_PARSING_METHOD = -1,
	FINEREADER_XML = 0,
	IM2CHARRECTS_XML,
	DAT_FILES
};

std::string type2str(ParsingMethodType parsingMethod);

enum ClusteringDataType
{
	UNKNOWN_CLUSTER_DISTANCE_TYPE = -1,
	DISTANCE_BASED = 0,
	FEATURES_BASED = 1
};

enum ClusterMethodType
{
	UNKNOWN_CLUSTER_METHOD = -1,
	KMEANS = 0,
	LEADER_FOLLOWER,
	AGGLOMERATIVE,
	DBSCAN,
	CLARANS
};

enum ParseSubsetType
{
	UNKNOWN_PARSESUBSET_TYPE = -1,
	PARSE_ALL = 0,
	PARSE_ONLY_NON_SUSP,
	PARSE_ONLY_SUSP
};

enum SubsetGenerationType
{
	UNKNOWN_GENERATION_TYPE = -1,
	PARSE_RANDOMLY = 0,
	PARSE_FIRST,
	PARSE_LAST
};

struct ParsingParameters
{
	ParsingParameters() {
		parsingMethod = UNKNOWN_PARSING_METHOD;
		filenamePrefix = ""; filenameSuffix = "";
		parsingType = UNKNOWN_PARSESUBSET_TYPE;
		generationType = UNKNOWN_GENERATION_TYPE;
		subsetPerc = 0;
	}
//	ParsingParameters(const ParsingParameters& other) {
//		parsingMethod = other.parsingMethod;
//		filenamePrefix = other.filenamePrefix;
//		filenameSuffix = other.filenameSuffix;
//		parsingType = other.parsingType;
//	}

	ParsingMethodType parsingMethod;
	std::string filenamePrefix;
	std::string filenameSuffix;
	ParseSubsetType parsingType;
	SubsetGenerationType generationType;
	int subsetPerc;
};

/** Structure for preprocessing parameters. */
class PreprocessingParameters
{
public:
	PreprocessingParameters()
	{
		binarize = false;
		invert = true;
		preMedian = false;
		preMedianMaskSize = 3;
		postMedian = false;
		postMedianMaskSize = 3;
		sizeNormWidth = 80;
		sizeNormHeight = 80;
	}

	void print()
	{
		std::cout << "binarize = " << binarize << ", invert = " << invert << std::endl;
		std::cout << "preMedian = " << preMedian << ", preMedianMaskSize = " << preMedianMaskSize << std::endl;
		std::cout << "sizeNormWidth = " << sizeNormWidth << ", sizeNormHeight = " << sizeNormHeight << std::endl;
		std::cout << "postMedian = " << postMedian << ", postMedianMaskSize = " << postMedianMaskSize << std::endl;
	}

	// parameters:
	bool binarize;
	bool invert;
	bool preMedian; int preMedianMaskSize;
	int sizeNormWidth; int sizeNormHeight;
	bool postMedian; int postMedianMaskSize;
};

/** Structure for PCA dimension reduction parameters. */
struct PCAReductionParams
{
	PCAReductionParams() {
		threshold = (float)0.9;
		newDim = 0;
	}

	float threshold;
	int newDim; // if <= 0, threshold is used!
};


/// Just a marker class for cluster parameters
class ClusterMethodParameters
{
	public:
		ClusterMethodParameters()
			{ dataType = DISTANCE_BASED; }
		virtual ~ClusterMethodParameters() {}

		ClusteringDataType dataType;
};

class KMeansParameters : public ClusterMethodParameters
{
public:
	KMeansParameters()
	: ClusterMethodParameters(), nClusters(defaultNClusters), maxIts(defaultMaxIts), eps(defaultEps)
		{ }

	int nClusters;
	int maxIts;
	float eps;
	static const int defaultNClusters;
	static const int defaultMaxIts;
	static const float defaultEps;
};

class AgglomerativeParameters : public ClusterMethodParameters
{
public:
	AgglomerativeParameters()
	: ClusterMethodParameters(), nClusters(defaultNClusters), featureDistType(defaultFeatureDistType), clusterDistType(defaultClusterDistType)
		{ }

	int nClusters;
	FeatureDistanceType featureDistType;
	ClusterDistanceType clusterDistType;

	static const int defaultNClusters;
	static const FeatureDistanceType defaultFeatureDistType;
	static const ClusterDistanceType defaultClusterDistType;
};

class LeaderFollowerParameters : public ClusterMethodParameters
{
public:
	LeaderFollowerParameters()
	: ClusterMethodParameters(), threshold(defaultThreshold), stopOnFirstCluster(true),
	  reassignThreshold(30), distToCenterThreshold(22), clusterSizeThreshold(3)
	{
	}

	float threshold;
	float learningRate;
	bool stopOnFirstCluster;

	float reassignThreshold;
	float distToCenterThreshold;
	int clusterSizeThreshold;

	static float defaultThreshold;
	static float defaultLearningRate;
};

class DBSCANParameters : public ClusterMethodParameters
{
public:
	DBSCANParameters() : ClusterMethodParameters(), minPts(defaultMinPts), eps(defaultEps)
		{ }

	int minPts;
	float eps;
	static int defaultMinPts;
	static int defaultEps;
};

class CLARANSParameters : public ClusterMethodParameters
{
public:
	CLARANSParameters() : ClusterMethodParameters(), numClust(defaultNumClust), numLocal(defaultNumLocal), maxNeighbor(defaultMaxNeighbor)
		{ }

	int numClust;
	int numLocal;
	int maxNeighbor;
	static int defaultNumClust;
	static int defaultNumLocal;
	static int defaultMaxNeighbor;
};

struct CharDataVecFlags
{
	bool invariant;
	bool basicImageStats;
	bool advancedImageStats;
};

class ImageChar {
public:
	ImageChar()
	: pImage(0x0), bBox(BBox()), imageID(-1), id(-1), text(""),
	  suspicious(false), pDocument(0x0), /*pPreprImage(0x0),*/ pDataMat(0x0),  dataRow(-1)
	{};
	// FUNCTIONS:
//	ImageChar(const BoundingBox& _bBox, GrayImage<uint8>* _pImage, const int imageID_ = -1, const int id = -1)
	ImageChar(const BoundingBox& _bBox, GrayImage<uint8>* _pImage, const int imageID_, const int id_)
		: pImage(_pImage), bBox(_bBox), imageID(imageID_), id(id_)
		{
		pDataMat = 0x0; dataRow = -1; text = ""; suspicious = false;
		pDocument = 0x0; /*pPreprImage = 0x0;*/
		distToClusterCenter = 0;
		}

//	~ImageChar() { pImage = 0x0; pDataMat = 0x0; }
//	ImageChar(const ImageChar &source)
//		{
//		pImage = source.pImage;
//		bBox = source.bBox;
//		imageID = source.imageID;
//		id = source.id;
//		text = source.text;
//		pDataMat = source.pDataMat;
//		dataRow = source.dataRow;
//		suspicious = source.suspicious;
//		pDocument = source.pDocument;
////		pPreprImage = source.pPreprImage;
//		mPreprocessingResults = source.mPreprocessingResults;
//		}

	void setDocumentPointer(Document *_pDocument) { this->pDocument = _pDocument; }
	bool hasDocumentPointer() const { return (this->pDocument != 0x0); }

	void setFeatureData(ublas::matrix<float> *_pDataMat, const int _dataRow)
		{ pDataMat = _pDataMat; dataRow = _dataRow; }
	void clearFeatureData()
		{ pDataMat = 0x0; dataRow = -1; }
	bool hasFeatureData() const
		{ return ((pDataMat != 0x0) && (dataRow != -1)); }

	static void setInvCovMatPointer(const ublas::matrix<float> *pInvCovMat)
		{ psInvCovMat = pInvCovMat; }

//	const ImageChar& operator=(const ImageChar& rhs)
//	{
//		if (this != &rhs) {
//			pImage = rhs.pImage;
//			bBox = rhs.bBox;
//			imageID = rhs.imageID;
//			id = rhs.id;
//			text = rhs.text;
//			pDataMat = rhs.pDataMat;
//			dataRow = rhs.dataRow;
//			suspicious = rhs.suspicious;
//			pDocument = rhs.pDocument;
////			pPreprImage = rhs.pPreprImage;
//			mPreprocessingResults = rhs.mPreprocessingResults;
//		}
//		return *this;
//	}

	// MEMBERS:
	GrayImage<uint8> *pImage;
	BoundingBox bBox;
	int imageID;
	int id;
	std::string text;
	bool suspicious;
	Document *pDocument;
//	GrayImage<> *pPreprImage; //< Pointer to the prepprocessed image of this ImageChar
	PreprocessingResults mPreprocessingResults;
	float distToClusterCenter;

	ublas::matrix<float> *pDataMat;
	int dataRow;
	// STATIC MEMBERS:
	static const ublas::matrix<float> *psInvCovMat; // a pointer to the inverse of the covariance matrix used by the mahalanobis distance
};

// operator<< for ImageChar's:
std::ostream& operator<<(std::ostream& os, const ImageChar& imageChar);


/** Computes the distance between the two feature vector representations of ImageChar's
 * using euclidean distance. */
float imageCharDistEucl(const ImageChar& imc1, const ImageChar& imc2);
/** Computes the distance between the two feature vector representations of ImageChar's
 * using mahalanobis distance. */
float imageCharDistMahalonobis(const ImageChar& imc1, const ImageChar& imc2);

/** Distance between two ImageChar's i.e. their feature vectors.
 * The Template argument distType specifies the type of distance.
 */
//template <FeatureDistanceType distType> inline
//float imageCharDist(const ImageChar& imc1, const ImageChar& imc2);
//
//// Template specializations of imageCharDist function:
//template <> inline
//float imageCharDist<EUCLIDEAN_DIST>(const ImageChar& imc1, const ImageChar& imc2)
//	{ return imageCharDistEucl(imc1, imc2); }
//template <> inline
//float imageCharDist<MAHALANOBIS_DIST>(const ImageChar& imc1, const ImageChar& imc2)
//{
//	if (ImageChar::psInvCovMat==0x0) {
//		throw NoDataException("Inverse covariance matrix not set in imageCharDist<MAHALANOBIS_DIST>!");
//	}
//	return imageCharDistMahalonobis(imc1, imc2, *ImageChar::psInvCovMat);
//}
////////////////////////////////////////////

/** Returns the cluster method type for the corresponding string. */
ClusterMethodType clusterMethodType(const std::string& str);
/** Returns the feature distance type for the corresponding string. */
FeatureDistanceType featureDistanceType(const std::string& str);
/** Returns the cluster distance type for the corresponding string. */
ClusterDistanceType clusterDistanceType(const std::string& str);

} // end of namespace impact_tr4

#endif /*DATASTRUCTS_HPP_*/
