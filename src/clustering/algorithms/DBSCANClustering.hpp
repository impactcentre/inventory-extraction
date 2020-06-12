/*
 * DBSCAN.hpp
 *
 *  Created on: 11.02.2010
 *      
 */

#ifndef DBSCAN_HPP_
#define DBSCAN_HPP_

#include <set>

#include "core_headers.h"
#include "image_headers.h"

#include "util/std_util.h"
#include "util/ublas_util.h"

#include "ClusterMethodI.hpp"

#include "../DataStructs.hpp"
#include "../CharCluster.hpp"


namespace impact_tr4 {

class DBSCANClustering : public ClusterMethodI
{
public:
	DBSCANClustering() : ClusterMethodI() {};
	DBSCANClustering(Document *pDocument, ClusteringResult *pClusteringResult, const ClusterMethodParameters* pParameters)
	: ClusterMethodI(pDocument, pClusteringResult, pParameters)
		{
		}
	~DBSCANClustering() {};

	/** Starts the DBSCAN clustering process. */
	void startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult);
private:
	void getNeighbors(const int i, const double eps, std::vector<int> &neighbors);
	void expandCluster(const int i, const std::vector<int> &i_neighbors, const int actClust, const double eps, const int minPts, std::vector<bool> &visited, std::vector<int> &labels);

}; // end of class DBSCAN.hpp

} // end of namespace impact_tr4


#endif /* DBSCAN_HPP_ */
