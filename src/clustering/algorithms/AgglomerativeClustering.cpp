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
#include "AgglomerativeClustering.hpp"

#include "clustering/ClusteringResult.hpp"
#include "clustering/Document.hpp"

#include "ext/cluster/cluster.h"

namespace impact_tr4 {

void AgglomerativeClustering::startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
	mpDocument = pDocument;
	mpClusteringResult = pClusteringResult;
	// preprocess glyphs or compute features, depending on distance type:
	if (pParameters->dataType == FEATURES_BASED) {
		pDocument->computeFeatures();
	}
	else {
		pDocument->preprocessAllGlyphs();
	}
//	this->mpDocument->setDistanceType(pParameters->dataType);

	// retrieve parameters:
	const AgglomerativeParameters *pParams = (const AgglomerativeParameters*)(pParameters);

	// print some debug info:
	std::cout << "Starting agglomerative clustering algorithm..." << std::endl;
	std::cout << "nr of clusters: " << pParams->nClusters << std::endl;
	std::cout << "feature dist type: " << pParams->featureDistType << std::endl;
	std::cout << "cluster dist type: " << pParams->clusterDistType << std::endl;

	mpClusteringResult->deleteClustering(); // delete probably old clustering result
//	setClusterDistanceTypeFunctionPointer(pParams->clusterDistType);
	const int nSamples = this->mpDocument->nParsedImages();
	if (nSamples < 2) {
		throw NoDataException("No data found for clustering!");
	}
	if (pParameters->dataType == FEATURES_BASED && this->mpDocument->nFeatures() < 2) {
		throw NoDataException("No features found for clustering!");
	}

	StopWatch watch;
	// initialize distance matrix:
	std::cout << "computing distance matrix..." << std::endl;
	watch.start();
	initDistanceMatrix();
	watch.stop();
	std::cout << "successfully computed distance matrix" << std::endl;

	// TEST: USING CLUSTER LIBRARY:
	double **tmpdistmatrix;

	// Allocate memory for distance matrix
	tmpdistmatrix = new double*[nSamples];
	for (int i = 0; i < nSamples; ++i)
		tmpdistmatrix[i] = new double[nSamples];
	// copy distance matrix
	for (int i=0;i<nSamples;++i){
		for (int j=0; j<=i; ++j) {
			tmpdistmatrix[i][j] = mDistMat(i,j);
			tmpdistmatrix[j][i] = mDistMat(i,j);
		}
	}
	std::vector<int> labels(nSamples);
#if 0
	int npass = 100;
	double error;
	int ifound;
	watch.start();
	kmedoids (pParams->nClusters, nSamples, tmpdistmatrix,
			npass, &labels[0], &error, &ifound);
	watch.stop();
	std::cout << "finished kmedioids, error = " << error << ", ifound = " << ifound << std::endl;

#else
	// method = 's' (single-linkage), 'm' (complete-linkage), 'a' (average-linkage) or 'c' (centroid-linkage):
	char methodChar='a';
	switch (pParams->clusterDistType) {
		case AVG_DIST:
			methodChar='a';
			break;
		case MIN_DIST:
			methodChar='s';
			break;
		case MAX_DIST:
			methodChar='m';
			break;
		default:
			throw Exception("Unknown distance type in AgglomerativeClustering::startClustering()");
			break;
	} // end switch

	Node* tree = treecluster(nSamples, 1, NULL, NULL, NULL, 0, '_', methodChar, tmpdistmatrix);
	if (tree==NULL)
		std::cerr << "FATAL ERROR - NULL POINTER IN CLUSTER RESULT!" << std::endl;

	// cut hierarchical cluster tree at specified nr of clusters:
	cuttree(nSamples, tree, pParams->nClusters, &labels[0]);
	delete [] tree;
#endif

	// De-Allocate memory for temporary distance matrix:
	for (int i = 0; i < nSamples; ++i)
		delete [] tmpdistmatrix[i];
	delete [] tmpdistmatrix;
	std::cout << "finished agglo clustering with cluster libarary!!" << std::endl;
	pClusteringResult->createClusteringResultFromLabelVector(labels, pDocument);
	/////////////////// END TEST

#if 0
	// get pointer to image chars:
	std::vector<ImageChar*> *imageCharVecPointer = this->mpDocument->getImageCharsVecPointer();

	// create cluster for each instance:
	CharCluster *pCluster=NULL;
	for (int i=0; i<nSamples; ++i) {
		pCluster = mpClusteringResult->addEmptyCluster(this->mpDocument);
		pCluster->addChar( (*imageCharVecPointer)[i] );
	}


	std::cout << "Starting merging process..." << std::endl;
//	watch.start();
	std::vector<float> minValVec;
	std::vector<int> nClustsVec;
	// while nr of clusters not reached -> merge two nearest clusters
//	while (mClusterVec.size() > pParams->nClusters) {
	while (mpClusteringResult->nClusters() > pParams->nClusters) {
		// find min element of distance matrix:
		minValVec.push_back(mMinDist);
//		nClustsVec.push_back(mClusterVec.size());
		nClustsVec.push_back(mpClusteringResult->nClusters());
//		std::cout << "nr of clusters is " << mClusterVec.size() << std::endl;
//		std::cout << "min dist is " << mMinDist << " on index " << mMinIndex << std::endl;
		watch.start();
		updateClusterLabels();
		std::cout << "updated cluster labels, time = " << watch.stop(false) << std::endl;

		watch.start();
		findMinDist();
		std::cout << "found min dist, time = " << watch.stop(false) << std::endl;
		std::cout << "new nr of clusters is " << mpClusteringResult->nClusters() << std::endl;
	} // end while
#endif

	pDocument->clearAllPreprocessing();
	pDocument->clearFeatures();
	pClusteringResult->computePrototypeFeatures();
	pClusteringResult->updatePrototypes(true);

	return;
} // end startClustering()

void startClusteringOnClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
	// 3rd step: agglomerative merging of cluster prototypes:
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
} // end of startClusteringOnClustering

#if 0
void AgglomerativeClustering::setFeatureDistanceTypeFunctionPointer(const FeatureDistanceType distType)
{
	ublas::matrix<float> covMat;
	ublas::vector<float> avgVec;

	switch (distType) {
		case EUCLIDEAN_DIST:
			ImageChar::psInvCovMat = 0x0;
			CharCluster::sfpImageCharDistance = imageCharDistEucl;
//			std::cout << "Euclidean distance was set!" << std::endl;
			printf("address: 0x%x\n", CharCluster::sfpImageCharDistance);
			break;
		case MAHALANOBIS_DIST:
			// generate covariance matrix:
			OpenCV::computeCovarianceMatrix<float>(mpFeatureCollection->dataMatrixRef(), covMat, avgVec, ROWWISE_ALIGNMENT, true);
			OpenCV::computeInverseMatrix<float>(covMat, mInvCovMat);
			ImageChar::psInvCovMat = &mInvCovMat;
			CharCluster::sfpImageCharDistance = imageCharDistMahalonobis;
//			std::cout << "Mahalonobis distance was set!" << std::endl;
			printf("address: 0x%x\n", CharCluster::sfpImageCharDistance);
			break;
		default:
			throw Exception("Unknown distance type in AgglomerativeClustering::setFeatureDistanceTypeFunctionPointer()");
			break;
	}
	return;
}
#endif

void AgglomerativeClustering::setClusterDistanceTypeFunctionPointer(const ClusterDistanceType distType)
{
	// set distance type function pointer:
	mfpDistance = 0x0;
	switch (distType) {
#if 0
		case MEAN_DIST:
			std::cout << "setting mean distance" << std::endl;
			mfpDistance = clusterDistMean;
			break;
		case WEIGHTED_MEAN_DIST:
			std::cout << "setting weighted mean distance" << std::endl;
			mfpDistance = clusterDistWeightedMean;
			break;
#endif
		case AVG_DIST:
			std::cout << "setting avg distance" << std::endl;
			mfpDistance = clusterDistAvg;
			break;
		case MIN_DIST:
			std::cout << "setting min distance" << std::endl;
			mfpDistance = clusterDistMin;
			break;
		case MAX_DIST:
			std::cout << "setting max distance" << std::endl;
			mfpDistance = clusterDistMax;
			break;
#if 0
		case AVG_DIRECT_DIST:
			std::cout << "setting avg direect distance" << std::endl;
			mfpDistance = clusterDistAvgDirect;
			break;
#endif
		default:
			throw Exception("Unknown distance type in AgglomerativeClustering::setClusterDistanceTypeFunctionPointer()");
			break;
	} // end switch
	return;
}

void AgglomerativeClustering::initDistanceMatrix()
{
	const int nSamples = mpDocument->nParsedImages();
//	std::cout << "nSample is: " << nSamples << std::endl;
	mDistMat.resize( nSamples, nSamples );
	mDistMatOrig.resize( nSamples, nSamples );
	mMinDist = (float)1e32;

	#pragma omp parallel for
    for (int i = 0; i < nSamples; ++ i) {
		#pragma omp parallel for
        for (int j = 0; j <= i; ++ j) {
        	if (i==j) {
        		mDistMat(i,j) = 0;
        		mDistMatOrig(i,j) = 0;
        		continue;
        	}

//        	std::cout << "computing distance for (i,j)="<< i << ", " <<j<<std::endl;

    		ImageChar *pIi = mpDocument->getImageChar(i);
    		ImageChar *pIj = mpDocument->getImageChar(j);
    		PreprocessingResults *pi = &pIi->mPreprocessingResults;
    		PreprocessingResults *pj = &pIj->mPreprocessingResults;

    		double dist = -1;
#if 1
    		dist = mpDocument->computeDistance(i, j);
#else
    		if (pi->isWhiteSpace && pj->isWhiteSpace) {
    			dist = 0;
    		}
    		else if ( (!pi->isWhiteSpace && pj->isWhiteSpace)
    				||(pi->isWhiteSpace && !pj->isWhiteSpace) ) {
    			dist = 9999;
    		}
    		else {
				dist = mpDocument->computeDistance(i, j);
    		}
#endif

    		if (dist < mMinDist) {
    			mMinDist = dist;
				mMinIndex.set(i,j);
			}

			mDistMat(i, j) = dist;
			mDistMat(j, i) = dist; // make symmetric

			// make a copy to origninal distance matrix (needed?):
    		mDistMatOrig(i,j) = dist;
    		mDistMatOrig(j,i) = dist;
        } // end for j
        std::cout << "finished computing distance matrix row " << i << std::endl;
	} // end for i

    // test: write distance matrix for evaluation with matlab:
//    std::string base = "C:/projekte/impact/matlab_foo/c_prog_out/";
//    std::string fn = "distMat.txt";
//    writeTxtFile(mDistMat, base + fn);
    ///////////////////////

//    std::cout << mDistMat << std::endl;
	return;
} // end initDistanceMatrix

void AgglomerativeClustering::findMinDist()
{
	mMinDist = mDistMat(1, 0);
	mMinIndex.set(1, 0);

    for (int i = 1; i < mDistMat.size1(); ++i) {
        for (int j = 0; j < i; ++ j) {
        		if ( mDistMat(i,j) < mMinDist ) {
        			mMinDist = mDistMat(i,j);
        			mMinIndex.set(i,j);
        		}
        } // end for j
	} // end for i

	return;
} // end finMinDist

void AgglomerativeClustering::updateClusterLabels()
{
#if 1
	const int r = mMinIndex(0);
	const int c = mMinIndex(1);
//	std::cout << "r = " << r << ", c = " << c << std::endl;
//	std::cout << mDistMat.size1() << ", " << mDistMat.size2() << std::endl;
	if (c >= r) {
		throw Exception("Column index is greater or equal than row index in AgglomerativeClustering::updateDistanceMatrixAndClusterVec - should never happen!!");
	}

	// find clusters for index c and r:
	StopWatch watch;
//	watch.start();
	CharClustIt cClustIt = mpClusteringResult->getClusterItFromIndex(c);
	CharClustIt rClustIt = mpClusteringResult->getClusterItFromIndex(r);
//	std::cout << "getting clusters: " << watch.stop(false) << std::endl;

	// merge r'th cluster into c'th cluster:
//	watch.start();
	(*cClustIt)->appendCluster(*(*rClustIt));
	mpClusteringResult->removeCluster(rClustIt);
//	std::cout << "merging clusters: " << watch.stop(false) << std::endl;

	// erase row and column r from distance matrix:
	watch.start(); // FIXME: this step is very time consuming --> fix, but how???
	watch.start();
	mDistMatOrig = mDistMat;
	watch.stop();
	watch.start();
	removeMatrixRow(mDistMat, r);
	watch.stop();
	watch.start();
	removeMatrixColumn(mDistMat, r);
	watch.stop();
	std::cout << "removing rows and copying distmat: " << watch.stop(false) << std::endl;

//	std::cout << "done!" << std::endl;
	// update row c from distance matrix:
	float dist=-1;
//	watch.start();
	for (int j=0; j < c; ++j) { // (c,j)
//		newDist = (*mfpDistance)(*(*mpClusteringResult)[c], *(*mpClusteringResult)[j], mDistMatOrig);
		dist = (mDistMatOrig(c,j)+mDistMatOrig(r,j))/2;

		mDistMat(c, j) = mDistMat(j, c) = dist;
	}
//	std::cout << "col update: " << watch.stop(false) << std::endl;
//	watch.start();
	// update column c from distance matrix:
	for (int i=0; i < c; ++i) { // (i,c)
//		dist = (*mfpDistance)(*(*mpClusteringResult)[i], *(*mpClusteringResult)[c], mDistMatOrig);
		dist = (mDistMatOrig(i,c)+mDistMatOrig(i,r))/2;

		mDistMat(i,c) = mDistMat(c, i) = dist;
	}
//	std::cout << "row update: " << watch.stop(false) << std::endl;
#endif
	return;
} // end updateDistanceMatrixAndClusterVec

} // end of namespace impact_tr4
