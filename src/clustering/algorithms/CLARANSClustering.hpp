/*
 * CLARANSClustering.h
 *
 *  Created on: 11.02.2010
 *      
 */

#ifndef CLARANSCLUSTERING_H_
#define CLARANSCLUSTERING_H_

#include <set>

#include "core_headers.h"
#include "image_headers.h"

#include "util/std_util.h"
#include "util/ublas_util.h"

#include "ClusterMethodI.hpp"

#include "../DataStructs.hpp"
#include "../CharCluster.hpp"

namespace impact_tr4
{

class CLARANSClustering : public ClusterMethodI
{
public:
	CLARANSClustering() : ClusterMethodI() {};
	CLARANSClustering(Document *pDocument, ClusteringResult *pClusteringResult, const ClusterMethodParameters* pParameters)
	: ClusterMethodI(pDocument, pClusteringResult, pParameters)
		{
		}
	virtual ~CLARANSClustering() {}

	void startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult);

private:
	void getRandomNode(const int n, const int k, std::vector<int> &node);
	void getRandomNeighbor(const int n, const std::vector<int> &inputNode, std::vector<int> &neighborNode);
	double getNodeCost(const int n, const std::vector<int> &node);
	void printNode(const std::vector<int> &node);

};

}

#endif /* CLARANSCLUSTERING_H_ */
