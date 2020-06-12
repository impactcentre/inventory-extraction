/*
 * DBSCAN.cpp
 *
 *  Created on: 11.02.2010
 *      
 */


#include "DBSCANClustering.hpp"

#include <set>

#include "clustering/ClusteringResult.hpp"
#include "clustering/Document.hpp"

namespace impact_tr4 {

void DBSCANClustering::startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
	mpDocument = pDocument;
	mpClusteringResult = pClusteringResult;

	this->mpDocument->setDistanceType(pParameters->dataType);

	if (pParameters->dataType == FEATURES_BASED) {
		pDocument->computeFeatures();
	}
	else {
		pDocument->preprocessAllGlyphs();
	}

	// retrieve parameters:
	const DBSCANParameters *pParams = (const DBSCANParameters*)(pParameters);

	std::cout << "DBSCAN parameters: minPts = " << pParams->minPts << ", eps = " << pParams->eps << std::endl;

//	mpClusteringResult->deleteClustering(); // delete probably old clustering result
	const int nSamples = this->mpDocument->nParsedImages();
	if (nSamples < 2) {
		throw NoDataException("No data found for clustering!");
	}
	if (pParameters->dataType == FEATURES_BASED && this->mpDocument->nFeatures() < 2) {
		throw NoDataException("No features found for clustering!");
	}

	// create vector for visited points:
	std::vector<bool> visited(nSamples, false);
	std::vector<int> labels(nSamples, -2);

	int actClust = 0;
	for (int i=0; i<nSamples; ++i) {
		std::cout << "processing sample " << i << " of " << nSamples << std::endl;
		if (!visited[i]) { // for all unvisited points
			visited[i] = true;
			std::vector<int> i_neighbors;
			getNeighbors(i, pParams->eps, i_neighbors);
			const int nNeighbors = i_neighbors.size();
			std::cout << "nr of neighbors(1) = " << nNeighbors << std::endl;
			if (nNeighbors < pParams->minPts) {
				labels[i] = 0; // mark this point as noise, i.e. -1
			}
			else {
				++actClust;
				expandCluster(i, i_neighbors, actClust, pParams->eps, pParams->minPts, visited, labels);
			}
		} // end if not visited
	} // for all unvisited points �

	std::cout << "nr of clusters (without noise points) = " << actClust+1 << std::endl;

	std::cout << "label vector = " << std::endl;
	for (int i=0; i<nSamples; ++i) {
		std::cout << labels[i] << " ";
		if ((i+1)%20==0) {
			std::cout << std::endl;
		}
	}
//	std::cout << std::endl << "visited vector = " << std::endl;
//	for (int i=0; i<nSamples; ++i) {
//		std::cout << visited[i] << " ";
//		if ((i+1)%20==0) {
//			std::cout << std::endl;
//		}
//	}

	mpClusteringResult->createClusteringResultFromLabelVector(labels, mpDocument);

	pDocument->clearAllPreprocessing();
	pDocument->clearFeatures();
	pClusteringResult->computePrototypeFeatures();
	pClusteringResult->updatePrototypes(true);

	std::cout << "Finished DBSCAN clustering!" << std::endl;

	return;
}

void DBSCANClustering::getNeighbors(const int i, const double eps, std::vector<int> &neighbors)
{
	const int nSamples = mpDocument->nParsedImages();
	double dist = 0.0f;
	neighbors.clear();
	for (int j=0; j<nSamples; ++j) {
		if (i!=j) {
			dist = mpDocument->computeDistance(i, j);
//			std::cout << "eps = " << eps << "dist = " << dist << std::endl;
			if (dist < eps) {
				neighbors.push_back(j);
			}
		}
	}

	return;
} // end getNeighbors

#if 1
void DBSCANClustering::expandCluster(const int i, const std::vector<int> &i_neighbors, const int actClust, const double eps, const int minPts, std::vector<bool> &visited, std::vector<int> &labels)
{
	labels[i] = actClust;
	std::vector<int> neighbors = i_neighbors;

	std::cout << "expanding cluster!" << std::endl;
	for (int j=0; j<neighbors.size(); ++j) {
		const int index = neighbors[j];
		if (!visited[index]) { // if point not visited yet
			visited[index] = true;
			std::vector<int> index_neighbors;
			getNeighbors(index, eps, index_neighbors);
			const int nNeighbors = index_neighbors.size();
			std::cout << "nr of neighbors(2) = " << nNeighbors << std::endl;
			if (nNeighbors >= minPts) {
				// add unique elements of neighborhood to neighbors-vector:
				for (int k=0; k<index_neighbors.size(); ++k) {
					std::vector<int>::iterator it = std::find(neighbors.begin(), neighbors.end(), index_neighbors[k]);
					if (it==neighbors.end()) { // if element does not exits in list yet, add it
						neighbors.push_back(index_neighbors[k]);
					}
				}
				std::cout << "j = " << j << ", new neighbors size = " << neighbors.size() << std::endl;
			}
		}
		if (labels[index] < 1) { // if point not member of any cluster yet, add it to actClust
			labels[index] = actClust;
		}
	} // for all neighbors of i

	return;
}
#endif

} // end of namespace impact_tr4
