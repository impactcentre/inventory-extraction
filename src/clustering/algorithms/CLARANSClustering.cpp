/*
 * CLARANSClustering.cpp
 *
 *  Created on: 11.02.2010
 *      
 */

#include "CLARANSClustering.hpp"

#include <cstdlib>
#include <ctime>
#include <time.h>

#include "clustering/ClusteringResult.hpp"
#include "clustering/Document.hpp"

namespace impact_tr4
{


void CLARANSClustering::startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult)
{
	mpDocument = pDocument;
	mpClusteringResult = pClusteringResult;

	this->mpDocument->setDistanceType(pParameters->dataType);

	// retrieve parameters:
	const CLARANSParameters *pParams = (const CLARANSParameters*)(pParameters);

	std::cout << "CLARANS parameters: numClust = " << pParams->numClust << ", numLocal = " << pParams->numLocal << ", maxNeighbor = " << pParams->maxNeighbor << std::endl;

	mpClusteringResult->deleteClustering(); // delete probably old clustering result
	const int nSamples = this->mpDocument->nParsedImages();
	const int nClusts = pParams->numClust;
	if (nSamples < 2) {
		throw NoDataException("No data found for clustering!");
	}
	if (pParameters->dataType == FEATURES_BASED && this->mpDocument->nFeatures() < 2) {
		throw NoDataException("No features found for clustering!");
	}
	// get pointer to image chars:
	std::vector<ImageChar*> *imageCharVecPointer = this->mpDocument->getImageCharsVecPointer();

	// INITIALIZATION:
//	srand(time(NULL)); // FIXME!!!!!!!!!!!!!
	double minCost = 1e32f; minCost = 1e32f;
	double actCost = 0.0f; double newCost = 0.0f;
	std::vector<int> currentNode;
	getRandomNode(nSamples, nClusts, currentNode);
	actCost = getNodeCost(nSamples, currentNode);
	std::cout << "Starting node: "; printNode(currentNode);
	// LOOP:
	int j = 1; j=1;
	std::vector<int> neighborNode;
	getRandomNeighbor(nSamples, currentNode, neighborNode); // determine a random neighbor node
	newCost = getNodeCost(nSamples, neighborNode);

	std::cout << "Finished CLARANS clustering!" << std::endl;
	return;
} // end startClustering(...)

void CLARANSClustering::getRandomNode(const int n, const int k, std::vector<int> &node)
{
	node.clear();
	for (int i=0; i<n; ++i) {
		int randIndex = rand() % n;
		std::vector<int>::iterator it = std::find(node.begin(), node.end(), randIndex);
		if (it == node.end()) { // of not present yet, add index to list
			node.push_back(randIndex);
		}
	}
	return;
} // end getRandomNode

void CLARANSClustering::getRandomNeighbor(const int n, const std::vector<int> &inputNode, std::vector<int> &neighborNode)
{
	int index = rand() % inputNode.size();
	neighborNode = inputNode;

	do {
		int newNode = rand() % n;
		if (inputNode[index] != newNode) {
			neighborNode[index] = newNode;
			break;
		}
	} while(true);
	return;
}

void CLARANSClustering::printNode(const std::vector<int> &node)
{
	for (int i=0; i<node.size(); ++i) {
		std::cout << node[i] << " ";
	}
	std::cout << std::endl;
	return;
}

double CLARANSClustering::getNodeCost(const int n, const std::vector<int> &node)
{
//	for (int i=0; i<)

	return 0;
}



} // end of namespace impact_tr4
