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
#include "LeaderFollower.hpp"
#include "clustering/ClusteringResult.hpp"
#include "clustering/Document.hpp"

namespace impact_tr4 {

#if 1 // new version
void LeaderFollower::startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
//	throw Exception("hans maulwurf!");

//	mpDocument = pDocument;
//	mpClusteringResult = pClusteringResult;

	pDocument->setDistanceType(pParameters->dataType);
	// preprocess glyphs or compute features, depending on distance type:
	if (pParameters->dataType == FEATURES_BASED) {
		pDocument->computeFeatures();
	}

	std::cout << "Starting extended Leader-Follower clustering..." << std::endl;
	const LeaderFollowerParameters *pParams = (const LeaderFollowerParameters*)(pParameters);
	std::cout << "parameters (threshold, stopOnFirstCluster, reassignThreshold, "
			"distToCenterThreshold, clusterSizeThreshold): " <<
				pParams->threshold << ", " <<
				pParams->stopOnFirstCluster << ", " <<
				pParams->reassignThreshold << ", " <<
				pParams->distToCenterThreshold << ", " <<
				pParams->clusterSizeThreshold << ", " << std::endl;


	pClusteringResult->deleteClustering(); // delete probably old clustering result

	const int nSamples = pDocument->nParsedImages();
	std::cout << "Nr. of samples = " << nSamples << std::endl;
	if (nSamples <= 0) {
		throw Exception("No sample images found!");
	}
	if (pParameters->dataType == FEATURES_BASED && pDocument->nFeatures() < 2) {
		throw NoDataException("No features found for clustering!");
	}

	// --- 1st step: extract prototypes ---
	int nClusts = 1; // first cluster is whitespace cluster
	std::vector<int> centers; centers.push_back(-1); // first center is -1, as whitespace cluster
	std::vector<int> clusterSizeCount; clusterSizeCount.push_back(0);
	std::vector<int> labels(nSamples, -1);

	// for all other samples i
	for (int i=0; i<nSamples; ++i) {
		std::cout << "processing sample " << i << " , k = " << centers.size() << std::endl;
		// compute preprocessing for this sample:
		pDocument->preprocessGlyph(i);
		ImageChar *pImChar = pDocument->getImageChar(i);
		PreprocessingResults *ppResults = &pImChar->mPreprocessingResults;

		// check if this is a whitespace character:
		if (ppResults->isWhiteSpace) {
//			std::cout << "whitespace!" << std::endl;
			labels[i] = 0;
			clusterSizeCount[0]++;

			pImChar->distToClusterCenter = 0;

			pDocument->clearPreprocessing(i); // clear preprocessing as not used anymore
			continue;
		}

		int min_j = 0;
		float min = (float)(1e32), val = -1;
		bool flag = false;

		// calculate index of maximum distance between x and all centers
		for (int j=1; j<centers.size(); ++j) {
			val = pDocument->computeDistance(i, centers[j]);

			if (val < min) { min = val; min_j = j; flag = true; }
			if (pParams->stopOnFirstCluster && flag && min < pParams->threshold) {
				break;
			}
		} // end for all centers j
//		std::cout << "min j = " << min_j << std::endl;
//		std::cout << "distance = " << min << std::endl;
		// if distance to max-center is smaller than given threshold --> add x to cluster
		if ( min < pParams->threshold ) {
			labels[i] = min_j; // label this sample according to min_j index
			clusterSizeCount[min_j]++; // increment cluster size count
			pImChar->distToClusterCenter = min;

			pDocument->clearPreprocessing(i); // clear preprocessing as not used anymore
		}
		// else: create new cluster with this sample
		else {
//			std::cout << "adding new cluster..." << std::endl;
			labels[i] = nClusts;
			nClusts++;
			centers.push_back(i);
			clusterSizeCount.push_back(1);
			pImChar->distToClusterCenter = 0;
		}
	} // end for all samples i
	std::cout << "finished first clustering phase!" << std::endl;
	std::cout << "nr of created clusters = " << nClusts << std::endl;


	// --- 2nd step: reassign samples to cluster prototypes ---
	// clear all cluster labels except the whitespace cluster:
	for (int i=0; i<labels.size(); ++i) {
		ImageChar *pImChar = pDocument->getImageChar(i);
		if (labels[i] == 0) { continue; } // if whitespace, proceed to next sample
		// is character no near to cluster center
		if ( pImChar->distToClusterCenter > pParams->distToCenterThreshold ||
			 clusterSizeCount[labels[i]] <= pParams->clusterSizeThreshold ) {
			labels[i] = -1;
		}
	}
	// reset labels of cluster centers:
	for (int j=1; j<centers.size(); ++j) {
//		std::cout << clusterSizeCount[j] << std::endl;
		if (clusterSizeCount[j] <= pParams->clusterSizeThreshold) { // singleton cluster? --> set centers to -1!
//			std::cout << "hello!" << std::endl;
			labels[centers[j]] = -1;
			pDocument->clearPreprocessing(centers[j]);
			centers[j] = -1;
			nClusts--;
		}
		else {
			clusterSizeCount[j] = 1;
			labels[centers[j]] = j;
		}
	}

	// for all other samples i
	#pragma omp parallel for
	for (int i=0; i<nSamples; ++i) {
		if (labels[i]!=-1) { continue; } // if assigned already, continue!

		std::cout << "processing sample " << i << " , k = " << nClusts << std::endl;
		// compute preprocessing for this sample:
		pDocument->preprocessGlyph(i);
		ImageChar *pImChar = pDocument->getImageChar(i);

		int min_j = 0;
		float min = (float)(1e32), val = -1;
		bool flag = false;

		// calculate index of maximum distance between x and all centers
		for (int j=1; j<centers.size(); ++j) {
			if (centers[j] == -1) { continue; }
			val = pDocument->computeDistance(i, centers[j]);

			if (val < min) { min = val; min_j = j; flag = true; }
			// stop on first cluster inquiry:
//			bool stopOnFirstCluster2 = false;
//			if (stopOnFirstCluster2 && flag && min < pParams->threshold) {
//				break;
//			}
		} // end for all centers j
//		std::cout << "min j = " << min_j << std::endl;
//		std::cout << "distance = " << min << std::endl;
		if ( min < pParams->reassignThreshold) {
			labels[i] = min_j; // label this sample according to min_j index
			clusterSizeCount[min_j]++; // increment cluster size count
			pImChar->distToClusterCenter = min;
			pDocument->clearPreprocessing(i); // clear preprocessing as not used anymore
		}
	} // end for all samples i

//	for (int i=0; i<labels.size(); ++i) { std::cout << labels[i] << " "; } std::cout << std::endl;

	pDocument->clearAllPreprocessing();
	pDocument->clearFeatures();

	pClusteringResult->createClusteringResultFromLabelVector(labels, pDocument);
	pClusteringResult->sortAllClustersByDistanceToClusterCenter(); // sorting by size to cluster center
	std::cout << "finished 2nd phase of clustering, nr of created clusters = " << pClusteringResult->nClusters() << std::endl;
	std::cout << "start computing features for prototypes..." << std::endl;
	pClusteringResult->computePrototypeFeatures();

	// 3rd step: agglomerative merging of cluster prototypes:
	std::cout << "is prototype distmat up to date: " << pClusteringResult->isDistMatUpToDate() << std::endl;
	pClusteringResult->computePrototypeDistanceMatrix();
	std::cout << "is prototype distmat up to date: " << pClusteringResult->isDistMatUpToDate() << std::endl;

#if 0 // currently only computation of distance matrix
	nClusts = pClusteringResult->nClusters();
	std::vector<GrayImage<> > clustPrototypeImages(nClusts);
	std::vector<GrayImage<float> > clustPrototypeIntInvImages(nClusts);
	std::vector<GrayImage<float> > clustPrototypeDistTransformImages(nClusts);

	// compute preprocessings for prototypes:
	std::cout << "computing pp for prototypes...";

	CharClustIt it = pClusteringResult->begin();

	for (int i=0; it!=pClusteringResult->end(); ++it, ++i) {
//		std::cout << "i=" << i << std::endl;
		clustPrototypeImages[i] = *((*it)->getPrototypeImageThreshPointer());
	}

	std::cout << "1...";

	#pragma omp parallel for
	for (int i=0; i<nClusts; ++i) {
		PreprocessingResults::computeOtherPreprocessings(	clustPrototypeImages[i],
															clustPrototypeIntInvImages[i],
															clustPrototypeDistTransformImages[i]);
	}

	std::cout << "2..." << std::endl;

	// compute distance matrix:
	std::cout << "computing distance matrix..." << std::endl;
	StopWatch watch;
	watch.start();
	ublas::matrix<float> protoDistMatrix(nClusts, nClusts, -1);
	#pragma omp parallel for
	for (int i=0; i<nClusts; ++i) {
//		#pragma omp parallel for
		for (int j=i; j<nClusts; ++j) {
			if (i==j) {
				protoDistMatrix(i,j) = 0;
				continue;
			}

			float dist = signatureDistance(	clustPrototypeImages[i],
											clustPrototypeImages[j],

											clustPrototypeDistTransformImages[i],
											clustPrototypeDistTransformImages[j],

											clustPrototypeIntInvImages[i],
											clustPrototypeIntInvImages[j] );

			protoDistMatrix(i,j) = dist;
			protoDistMatrix(j,i) = dist;
		}
	}
	std::cout << "computed distance matrix, time = " << watch.stop() << std::endl;
	std::cout << protoDistMatrix << std::endl;
#endif


	return;
} // end startClustering()
#endif

#if 0 // medium old version
void LeaderFollower::startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
//	mpDocument = pDocument;
//	mpClusteringResult = pClusteringResult;

	pDocument->setDistanceType(pParameters->dataType);
	// preprocess glyphs or compute features, depending on distance type:
	if (pParameters->dataType == FEATURES_BASED) {
		pDocument->createFeatures();
	}

	std::cout << "Starting extended Leader-Follower clustering..." << std::endl;
	const LeaderFollowerParameters *pParams = (const LeaderFollowerParameters*)(pParameters);
	std::cout << "parameters (threshold, learning-rate): " << pParams->threshold << ", " << pParams->learningRate << std::endl;
	pClusteringResult->deleteClustering(); // delete probably old clustering result

	const int nSamples = pDocument->nParsedImages();
	std::cout << "Nr. of samples = " << nSamples << std::endl;
	if (nSamples <= 0) {
		throw Exception("No sample images found!");
	}
	if (pParameters->dataType == FEATURES_BASED && pDocument->nFeatures() < 2) {
		throw NoDataException("No features found for clustering!");
	}

	// 1st phase: ----
	PreprocessingResults *ppResults;

	//////////////////////////////////////////////
	GrayImage<> tmpInt8Image;
	GrayImage<int32> tmpInt32Image;
	GrayImage<float> tmpFloatImage;

	std::vector<GrayImage<int32> > clustPrototypeSumImages;
	std::vector<GrayImage<> > clustPrototypeImages;
	std::vector<GrayImage<float> > clustPrototypeIntInvImages;
	std::vector<GrayImage<float> > clustPrototypeDistTransformImages;
	//////////////////////////////////////////////

	int nClusts = 1; // first cluster is whitespace cluster
	std::vector<int> centers; centers.push_back(-1); // first center is -1, as whitespace cluster
	std::vector<int> clusterSizeCount; clusterSizeCount.push_back(0);

	std::vector<int> labels(nSamples, -1);

	// for all other samples i
	for (int i=0; i<nSamples; ++i) {
		std::cout << "processing sample " << i << " , k = " << centers.size() << std::endl;
		// compute preprocessing for this sample:
		pDocument->preprocessGlyph(i);
		ImageChar *pImChar = pDocument->getImageChar(i);
		ppResults = &pImChar->mPreprocessingResults;

		// check if this is a whitespace character:
		if (ppResults->isWhiteSpace) {
			std::cout << "whitespace!" << std::endl;
			labels[i] = 0;
			clusterSizeCount[0]++;

			pImChar->distToClusterCenter = 0;

			pDocument->clearPreprocessing(i); // clear preprocessing as not used anymore
			continue;
		}

		int min_j = 0;
		float min = (float)(1e32), val = -1;
		bool flag = false;

		// calculate index of maximum distance between x and all centers
		for (int j=1; j<centers.size(); ++j) {
			val = pDocument->computeDistance(i, centers[j]);
//			if (clusterSizeCount[j] < 5) {
//				val = pDocument->computeDistance(i, centers[j]);
//			}
//			else {
//				std::cout << "computing distance to prototype" << std::endl;
//				val = signatureDistance(	*ppResults->mpProcessedImage,
//											clustPrototypeImages[j],
//
//											*ppResults->mpDistTransformImage,
//											clustPrototypeDistTransformImages[j],
//
//											*ppResults->mpIntInvImage,
//											clustPrototypeIntInvImages[j] );
//			}

			if (val < min) { min = val; min_j = j; flag = true; }
			if (pParams->stopOnFirstCluster && flag && min < pParams->threshold) {
				break;
			}
		} // end for all centers j
//		std::cout << "min j = " << min_j << std::endl;
//		std::cout << "distance = " << min << std::endl;
		// if distance to max-center is smaller than given threshold --> add x to cluster
		if ( min < pParams->threshold ) {
			labels[i] = min_j; // label this sample according to min_j index
			clusterSizeCount[min_j]++; // increment cluster size count
			pImChar->distToClusterCenter = min;

//			std::cout << "updating prototype image" << std::endl;
			// Update cluster prototype:
//			ImageOperations::convert<uint8, int32>(*ppResults->mpProcessedImage, tmpInt32Image);
//			ImageOperations::add<int32>(clustPrototypeSumImages[min_j], tmpInt32Image, clustPrototypeSumImages[min_j]);
//			double scaleFactor = 1.0f/(double)(clusterSizeCount[min_j]);
//			ImageOperations::convert<int32, uint8>(clustPrototypeSumImages[min_j], clustPrototypeImages[min_j], scaleFactor);
//			clustPrototypeImages[min_j].binarize(200);
//			// update preprocessings:
//			PreprocessingResults::computeOtherPreprocessings(	clustPrototypeImages[min_j],
//																clustPrototypeIntInvImages[min_j],
//																clustPrototypeDistTransformImages[min_j]);

			pDocument->clearPreprocessing(i); // clear preprocessing as not used anymore
		}
		// else: create new cluster with this sample
		else {
			std::cout << "adding new cluster..." << std::endl;
			labels[i] = nClusts;
			nClusts++;
			centers.push_back(i);
			clusterSizeCount.push_back(1);
			pImChar->distToClusterCenter = 0;

//			std::cout << "setting prototype image" << std::endl;
			// set prototype and preprocessing images:
//			clustPrototypeSumImages.push_back(tmpInt32Image);
//			clustPrototypeImages.push_back(tmpInt8Image);
//			clustPrototypeIntInvImages.push_back(tmpFloatImage);
//			clustPrototypeDistTransformImages.push_back(tmpFloatImage);
//
//			clustPrototypeImages[nClusts-1] = *ppResults->mpProcessedImage;
//			if (!ppResults->isWhiteSpace) {
//				ImageOperations::convert<uint8, int32>(*ppResults->mpProcessedImage, clustPrototypeSumImages[nClusts-1]);
//				clustPrototypeIntInvImages[nClusts-1] = *ppResults->mpIntInvImage;
//				clustPrototypeDistTransformImages[nClusts-1] = *ppResults->mpDistTransformImage;
//			}
		}
	} // end for all samples i
	std::cout << "finished clustering!" << std::endl;
	std::cout << "nr of created clusters = " << nClusts << std::endl;

	pDocument->clearAllPreprocessing();

	pClusteringResult->createClusteringResultFromLabelVector(labels, pDocument);
	std::cout << "start sorting..." << std::endl;
	pClusteringResult->sortAllClustersByDistanceToClusterCenter(); // sorting by size to cluster center
	std::cout << "start computing features for prototypes..." << std::endl;
	pClusteringResult->computePrototypeFeatures();


	return;
} // end startClustering()
#endif


#if 0 // old version
void LeaderFollower::startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
	mpDocument = pDocument;
	mpClusteringResult = pClusteringResult;

	this->mpDocument->setDistanceType(pParameters->dataType);
	// preprocess glyphs or compute features, depending on distance type:
	if (pParameters->dataType == DISTANCE_BASED) {
		this->mpDocument->preprocessAllGlyphs();
	}
	else if (pParameters->dataType == FEATURES_BASED) {
		this->mpDocument->createFeatures(); // TODO: store pca reduction parameters in document!
	}
	else {
		throw Exception("Unknown data distance type given in LeaderFollower clustering!");
	}

	std::cout << "Starting Leader-Follower clustering..." << std::endl;
	const LeaderFollowerParameters *pParams = (const LeaderFollowerParameters*)(pParameters);
	std::cout << "parameters (threshold, learning-rate): " << pParams->threshold << ", " << pParams->learningRate << std::endl;

	this->mpClusteringResult->deleteClustering(); // delete probably old clustering result

	const int nSamples = this->mpDocument->nParsedImages();
	std::cout << "Nr. of samples = " << nSamples << std::endl;
	if (nSamples <= 0) {
		throw Exception("No preprocessed glyph images found!");
	}
	if (pParameters->dataType == FEATURES_BASED && this->mpDocument->nFeatures() < 2) {
		throw NoDataException("No features found for clustering!");
	}

	// initialize: first center = first sample
	int nClusts = 0;
	std::vector<int> centers;
	std::vector<int> labels(nSamples, -1);

	CharCluster *pCluster=NULL;
	// for all other samples i
	for (int i=0; i<nSamples; ++i) {
		std::cout << "processing sample " << i << " , k = " << centers.size() << std::endl;
		int min_j = 0;
		float min = (float)(1e32), val = -1;
		bool flag = false;
		// calculate index of maximum distance between x and all centers
		for (int j=0; j<centers.size(); ++j) {
			val = this->mpDocument->computeDistance(i, centers[j]);
			if (val < min) { min = val; min_j = j; flag = true; }
			if (pParams->stopOnFirstCluster && flag && min < pParams->threshold) {
				break;
			}
		} // end for all centers j
//		std::cout << "min j = " << min_j << std::endl;
//		std::cout << "distance = " << min << std::endl;
		// if distance to max-center is smaller than given threshold --> add x to cluster
		if ( min < pParams->threshold ) {
			labels[i] = min_j; // label this sample according to min_j index
		}
		// else: create new cluster with this sample
		else {
			pCluster = this->mpClusteringResult->addEmptyCluster(mpDocument);
			nClusts++;
			labels[i] = nClusts-1;
			centers.push_back(i);
		}
	} // end for all samples i
	std::cout << "finished clustering!" << std::endl;
	std::cout << "nr of created clusters = " << nClusts << std::endl;

	this->mpDocument->clearAllPreprocessing();

	this->mpClusteringResult->createClusteringResultFromLabelVector(labels, mpDocument);
	mpClusteringResult->computePrototypeFeatures();

	return;
} // end startClustering()
#endif

} // end of namespace impact_tr4
