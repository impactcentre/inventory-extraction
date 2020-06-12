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
#include "KMeans.hpp"

#include "util/StopWatch.hpp"
#include "clustering/Document.hpp"
#include "clustering/ClusteringResult.hpp"

namespace impact_tr4 {

void KMeans::startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
	mpDocument = pDocument;
	mpClusteringResult = pClusteringResult;

	this->mpDocument->setDistanceType(pParameters->dataType);
	if (pParameters->dataType == DISTANCE_BASED) {
		throw Exception("Distance based clustering not possible for KMeans!");
	}

	std::cout << "computing features..." << std::endl;
	this->mpDocument->computeFeatures();
	std::cout << "finished..." << std::endl;


#if 1 // old code
	const KMeansParameters *pParams = (const KMeansParameters*)(pParameters);

	std::cout << "starting kmeans clustering..." << std::endl;
	std::cout << "nr of clusters: " << pParams->nClusters << std::endl;
	std::cout << "stopping parameters (max its, eps): " << pParams->maxIts << ", " << pParams->eps << std::endl;

    StopWatch watch;
    watch.start();
	ublas::matrix<float>& dataMatrix = this->mpDocument->getCharFeatureCollectionPointer()->dataMatrixRef();
//	std::cout << dataMatrix << std::endl;
//	std::cout << dataMatrix.size1() << ", " << dataMatrix.size2() << std::endl;
	const int nSamples = dataMatrix.size1();
	if (dataMatrix.size1()*dataMatrix.size2() <= 0) {
		throw NoDataException("No features found for clustering with kmeans!");
	}

    CvMat *dataMatrixOCV = OpenCV::cvMatFromBoostMat<float>(dataMatrix);
//    std::cout << "dataMatrixOCV, rows = " << dataMatrixOCV->rows << ", cols = " << dataMatrixOCV->cols << std::endl;
//    std::cout << "computed data matrix opencv!" << std::endl;

    CvMat* clusters = cvCreateMat( nSamples, 1, CV_32SC1 );
    cvKMeans2( dataMatrixOCV, pParams->nClusters, clusters,
    		cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, pParams->maxIts, pParams->eps ) );
//    std::cout << "cvmat rows = " << clusters->rows << ", cols = " << clusters->cols << std::endl;
    std::cout << "finished k-means clustering using opencv!" << std::endl;
    watch.stop();

    // save cluster label vector to cluster result:
    std::vector<int> labels;
    for (int i=0; i<nSamples; ++i) { labels.push_back(clusters->data.i[i]); }
    mpClusteringResult->createClusteringResultFromLabelVector(labels, mpDocument);

    pDocument->clearAllPreprocessing();
    pDocument->clearFeatures();
	pClusteringResult->computePrototypeFeatures();
	pClusteringResult->updatePrototypes(true);

    cvReleaseMat( &clusters );
#endif

} // end startClustering

} // end of namespace impact_tr4
