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
#include "DataStructs.hpp"

namespace impact_tr4 {

// Default KMeans parameters:
/*static*/ const int KMeansParameters::defaultNClusters = 60;
/*static*/ const int KMeansParameters::defaultMaxIts = 1000;
/*static*/ const float KMeansParameters::defaultEps = (float)0.1f;

// Default Agglomerative parameters:
/*static*/ const int AgglomerativeParameters::defaultNClusters = 20;
/*static*/ const FeatureDistanceType AgglomerativeParameters::defaultFeatureDistType = EUCLIDEAN_DIST;
/*static*/ const ClusterDistanceType AgglomerativeParameters::defaultClusterDistType = MEAN_DIST;

// Default Leader-Follower parameters:
/*static*/ float LeaderFollowerParameters::defaultThreshold = 0.1f;
/*static*/ float LeaderFollowerParameters::defaultLearningRate = 0.1f;

/*static*/ int DBSCANParameters::defaultMinPts = 3;
/*static*/ int DBSCANParameters::defaultEps = 10.0f;

/*static*/ int CLARANSParameters::defaultNumClust = 60;
/*static*/ int CLARANSParameters::defaultNumLocal = 5;
/*static*/ int CLARANSParameters::defaultMaxNeighbor = 50;

// Default pointer to inverse covariance matrix set to nullpointer:
/*static*/ const ublas::matrix<float>* ImageChar::psInvCovMat = 0x0;

std::string type2str(ParsingMethodType parsingMethod)
{
	std::string parsingMethodStr = "unknownParsingMethod";
	switch (parsingMethod) {
	case FINEREADER_XML:
		return std::string("abbyyXML");
		break;
	case IM2CHARRECTS_XML:
		return std::string("im2CharRectsXML");
		break;
	case DAT_FILES:
		return std::string("datFiles");
		break;
	default:
		return "unknownParsingMethod";
		break;
	}
	return "unknownParsingMethod";
}


//GrayImage<uint8> *pImage;
//BoundingBox bBox;
//int imageID;
//int id;
//
//ublas::matrix<float> *pDataMat;
//int dataRow;
//// STATIC MEMBERS:
//static const ublas::matrix<float> *psInvCovMat;
std::ostream& operator<<(std::ostream& os, const ImageChar& imageChar)
{
	os << "ImageChar: ";
	os << "Bounding Box: " << imageChar.bBox.x() << ", " << imageChar.bBox.y() << ", " << imageChar.bBox.width() << imageChar.bBox.height() << "\n";
	os << "Image ID, Char ID: " << imageChar.imageID << ", " << imageChar.id << "\n";
	return os;
}

float imageCharDistEucl(const ImageChar& imc1, const ImageChar& imc2)
{
//	std::cout << "Using euclidean distance!" << std::endl;
	if (!imc1.hasFeatureData() || !imc2.hasFeatureData()) {
		throw NoDataException("No feature data in imageCharDistEucl!");
	}

	ublas::matrix_row<ublas::matrix<float> > vec1(*imc1.pDataMat, imc1.dataRow);
	ublas::matrix_row<ublas::matrix<float> > vec2(*imc2.pDataMat, imc2.dataRow);

	return ublas::norm_2(vec1 - vec2);
}

float imageCharDistMahalonobis(const ImageChar& imc1, const ImageChar& imc2)
{
//	std::cout << "Using mahalanobis distance!" << std::endl;
	if (!imc1.hasFeatureData() || !imc2.hasFeatureData()) {
		throw NoDataException("No feature data in imageCharDistMahalonobis!");
	}
	if (ImageChar::psInvCovMat==0x0) {
		throw NoDataException("Inverse covariance matrix not set in imageCharDistMahalonobis!");
	}

//	std::cout << ImageChar::psInvCovMat->size1() << " x " << ImageChar::psInvCovMat->size2() << std::endl;

	ublas::matrix_row<ublas::matrix<float> > vec1(*imc1.pDataMat, imc1.dataRow);
	ublas::matrix_row<ublas::matrix<float> > vec2(*imc2.pDataMat, imc2.dataRow);

	return OpenCV::mahalanobisDist<float>( vec1, vec2, *ImageChar::psInvCovMat );
}

ClusterMethodType clusterMethodType(const std::string& str)
{
	if (!str.compare("K-Means")) {
		return KMEANS;
	}
	else if (!str.compare("Leader-Follower (Default)")) {
		return LEADER_FOLLOWER;
	}
	else if (!str.compare("Agglomerative")) {
		return AGGLOMERATIVE;
	}
	else if (!str.compare("DBSCAN (experimental)")) {
		return DBSCAN;
	}
	else if (!str.compare("CLARANS (experimental)")) {
		return CLARANS;
	}

	return UNKNOWN_CLUSTER_METHOD;
}

FeatureDistanceType featureDistanceType(const std::string& str)
{
	if (!str.compare("Euclidean")) {
		return EUCLIDEAN_DIST;
	}
	else if (!str.compare("Mahalanobis")) {
		return MAHALANOBIS_DIST;
	}
	else {
		throw Exception("Unknown type in featureDistanceType(std::string)!");
	}
}

ClusterDistanceType clusterDistanceType(const std::string& str)
{
	if (!str.compare("Mean")) {
		return MEAN_DIST;
	}
	else if (!str.compare("Weighted Mean")) {
		return WEIGHTED_MEAN_DIST;
	}
	else if (!str.compare("Average")) {
		return AVG_DIST;
	}
	else if (!str.compare("Minimum")) {
		return MIN_DIST;
	}
	else if (!str.compare("Maximum")) {
		return MAX_DIST;
	}
	else if (!str.compare("Average Direct")) {
		return AVG_DIRECT_DIST;
	}

	return UNKNOWN_DIST;
}

} // end of namespace impact_tr4
