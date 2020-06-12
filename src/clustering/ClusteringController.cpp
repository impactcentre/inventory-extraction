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
#include "ClusteringController.hpp"

#include <numeric>
#include <algorithm>

//#include <boost/bind.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
//#include <boost/shared_ptr.hpp>

#include "data_reader/FRXMLCharDataReader.hpp"
#include "data_reader/Im2CharRectsCharDataReader.hpp"
#include "core/PCA.hpp"
#include "util/util.h"

//#include "algorithms/FeatureVectorClusterMethodI.hpp"

#include "clustering/ClusteringResult.hpp"
#include "algorithms/KMeans.hpp"
#include "algorithms/LeaderFollower.hpp"
#include "algorithms/AgglomerativeClustering.hpp"
#include "algorithms/DBSCANClustering.hpp"
#include "algorithms/CLARANSClustering.hpp"

namespace impact_tr4
{

ClusteringController::ClusteringController()
{

}

ClusteringController::~ClusteringController()
{
	clear();
}

void ClusteringController::parseInputFiles(const ParsingParameters parsingParameters, const std::vector<std::string>& inputFns)
{
#if 0
	boost::thread thread(boost::bind(&Document::parseInputFiles, mpDocument, parsingParameters, inputFns));
	thread.join();
#else
	mDocument.parseInputFiles(parsingParameters, inputFns);
#endif

	emit parsingFinished(true);
	return;
}

#if 1
void ClusteringController::preprocessGlyphs(const PreprocessingParameters parameters)
{
	mDocument.setPreprocessingParameters(parameters);
	mDocument.preprocessAllGlyphs();
	return;
}

void ClusteringController::createFeatures(	const PreprocessingParameters parameters,
											const PCAReductionParams &pcaParameters)
{
	mDocument.setPreprocessingParameters(parameters);
	mDocument.setPCAParameters(pcaParameters);
	mDocument.computeFeatures();
	return;
} // end createFeatures
#endif


void ClusteringController::clusterGlyphs(	const PreprocessingParameters ppParameters,
											const PCAReductionParams pcaParameters,
											const ClusterMethodType clusterMethod,
											const ClusterMethodParameters* pParameters)
{
//	if (this->mDocument.nProcessedImages() == 0) {
//		throw Exception("No preprocessed glyph images found!");
//	}
//	if (pParameters->dataType == FEATURES_BASED && this->mDocument.nFeatures() < 2) {
//		throw NoDataException("No features found for clustering!");
//	}

	mDocument.setPreprocessingParameters(ppParameters);
	mDocument.setPCAParameters(pcaParameters);

//	const int nSamples = this->mDocument.nProcessedImages();
//	// 1st: extract sample indices:
//	std::vector<int> sampleIndices(nSamples);
//	for (int i=0; i<sampleIndices.size(); ++i) { sampleIndices[i] = i; }
//	// 2nd:
//	std::vector<int> labelVector(nSamples, -1);

	// create cluster method
	ClusterMethodI *pClusterMethod=0x0;
	switch (clusterMethod) {
		case KMEANS:
			pClusterMethod = new KMeans(&mDocument, &mClusteringResult, pParameters);
//			throw Exception("Not supported now... currently re-implementing clustering methods!");
			break;
		case LEADER_FOLLOWER:
			pClusterMethod = new LeaderFollower(&mDocument, &mClusteringResult, pParameters);
//			pClusterMethod = new LeaderFollower(sampleIndices, &mDocument, &labelVector);
			break;
		case AGGLOMERATIVE:
			pClusterMethod = new AgglomerativeClustering(&mDocument, &mClusteringResult, pParameters);
//			throw Exception("Not supported now... currently re-implementing clustering methods!");
			break;
		case DBSCAN:
			pClusterMethod = new DBSCANClustering(&mDocument, &mClusteringResult, pParameters);
//			throw Exception("Not supported now... currently re-implementing clustering methods!");
			break;
		case CLARANS:
			pClusterMethod = new CLARANSClustering(&mDocument, &mClusteringResult, pParameters);
			throw Exception("Not supported now... to-implement!");
//			throw Exception("Not supported now... currently re-implementing clustering methods!");
			break;
		default:
			throw Exception("Error creating cluster method in ClusteringController::ClusteringController(...)!");
			break;
	} // end switch
	connect(pClusterMethod, SIGNAL(clusteringFinished(bool)), this, SIGNAL(clusteringFinished(bool)));

#if 0 // version with boost threads --> problem: exceptions cannot be caught by Qt!!
#if 0
	boost::thread thread(boost::bind(&ClusterMethodI::startClustering, pParameters, &mDocument, &mClusteringResult));
//	thread.join();
#else
	pClusterMethod->startClustering(pParameters, &mDocument, &mClusteringResult);
#endif
#else
	pClusterMethod->start(); // start QThread that represents this cluster method
#endif

//	this->mClusteringResult.createClusteringResultFromLabelVector(labelVector, &mDocument);
//	mClusteringResult.computePrototypeFeatures();

//	emit clusteringFinished();

	return;
}

void ClusteringController::clear()
{
	mDocument.clearDocument();
	mClusteringResult.deleteClustering();
}

void ClusteringController::storeDataMatrix(const std::string& fn)
{
	mDocument.storeDataMatrix(fn);
	return;
} // end storeDataMatrix

void ClusteringController::createClusterImage(GrayImage<> &image)
{
	if (!mClusteringResult.isEmpty()) {
		writeClusterImageBlocked( this->mClusteringResult, image, 1200 );
	}
	else {
		// TODO: proper error handling here
		std::cout << "No clustering result available in createClusterImage!" << std::endl;
	}
	return;
}

} // end of namespace impact_tr4
