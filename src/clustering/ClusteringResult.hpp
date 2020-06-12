/*
 * ClusteringResult.hpp
 *
 *  Created on: 06.08.2009
 *      
 */

#ifndef CLUSTERINGRESULT_HPP_
#define CLUSTERINGRESULT_HPP_

#include <iostream>
#include <string>

#include "core_headers.h"
#include "CharCluster.hpp"
#include "DataStructs.hpp"
#include "CharFeatureCollection.hpp"

namespace impact_tr4 {

class Document;

typedef std::list<CharCluster*>::iterator CharClustIt;
typedef std::list<CharCluster*>::const_iterator CharClustConstIt;

inline bool compareIntCharClustItPairings(std::pair<int, CharClustIt> p1, std::pair<int, CharClustIt> p2)
{
	return p1.first < p2.first;
}

/// Compares two CharClustIt 's by returning size_of_cluster1 < size_of_cluster2
inline bool compareCharClustItBySize(CharClustIt it1, CharClustIt it2)
{
	return (*it1)->size() < (*it2)->size();
}

class ClusteringResult
{
public:
	/// Default constructor. Constructs an empty clustering
	ClusteringResult();
	/// Copy constructor
	ClusteringResult(const ClusteringResult& source);
	/// Destructor
	~ClusteringResult();

	/// Deletes content of this clustering and results into an empty clustering.
	void deleteClustering();
	/// Creates a clustering result from a given vector of cluster labels corresponding to the cluster index for each sample in the given document
	void createClusteringResultFromLabelVector(const std::vector<int>& labels, Document *pDocument);

	// --- ACCESS METHODS & INQUIRY ---:
	/// Returns the number of clusters of this clustering
	int nClusters() const { return mClusterList.size(); }
	/// Determines, if this clustering is empty
	bool isEmpty() const { return mClusterList.empty(); }

	CharCluster* front() { return mClusterList.front(); }
	CharCluster* back() { return mClusterList.back(); }
	const CharCluster* front() const { return mClusterList.front(); }
	const CharCluster* back() const { return mClusterList.back(); }

	CharClustIt begin() { return mClusterList.begin(); }
	CharClustIt end() { return mClusterList.end(); }
	CharClustConstIt begin() const { return mClusterList.begin(); }
	CharClustConstIt end() const { return mClusterList.end(); }

	/// Returns the cluster iterator for the specified linear index i
	CharClustIt getClusterItFromIndex(const int i);
	/// Returns the linear index of the cluster given by the specified cluster iterator clustIt or -1 of not found
	int getClusterIndex(CharClustIt clustIt);

	CharClustIt getTrashCluster() { return mTrashClusterList.begin(); }

	/// Computes the number of all characters contained in this clustering.
	int nTotalElements() const;

	// --- EDITING METHODS ---:
	/// Adds an empty cluster to the clustering
	CharCluster* addEmptyCluster(Document *pDocument);
	/// Remove cluster with iterator it from clustering
	void removeCluster(const CharClustIt it);
	/// Removes empty clusters from clustering
	void removeEmptyClusters();
	/// Merges cluster with iterator from into cluster with iterator to and updates the prototype for the cluster with iterator to.
	void mergeClusters(const CharClustIt from, const CharClustIt to);
	/// Moves elements with the iterators listed in iterators from cluster with iterator from to cluster with iterator to
	void moveElements(const CharClustIt from, const CharClustIt to, std::vector<ImageCharIt> iterators);
	void removeElements(const CharClustIt from, std::vector<ImageCharIt> iterators);
	/// Updates all prototypes for all clusters of this clustering. If forceAll is false (default), the method checks, if updating the prototype image is necessary
	void updatePrototypes(const bool forceAll=false);
	/// Computes features for all prototypes of all clusters for this clustering
	void computePrototypeFeatures();

	// NEW: trash cluster:
	/// Moves elements from the cluster 'from' with the iterators listed in 'iterators' to the trash cluster
	void moveToTrash(const CharClustIt from, std::vector<ImageCharIt> iterators);
	/// Moves cluster content of cluster 'it' to trash
	void moveClusterToTrash(const CharClustIt it);
	void clearTrash();

	// --- DISTANCE MATRIX BETWEEN PROTOTYPES ---:
	bool hasPrototypeDistanceMatrix() const { return (mDistMat.size1() != 0 && mDistMat.size2() != 0); }
	void computePrototypeDistanceMatrix();
	ublas::matrix<float>& prototypeDistanceMatrix() { return mDistMat; }
	void clearPrototypeDistanceMatrix() { mDistMat.clear(); mIsDistMatUpToDate = true; }
	bool isDistMatUpToDate() const { return mIsDistMatUpToDate; }

	// --- RECLUSTERING CLUSTERING RESULT:
	void reClusterHierarchical(int nClusts);
	void mergeClustersByLabelVector(const std::vector<int>& labels);

	// --- OTHER ---:
	/// prints this cluster result to the console
	void print();

	/// Computes a vector of unsorted cluster iterators
	void computeUnsorted(std::vector<CharClustIt>& unsorted);
	/// Computes a vector of cluster iterators, sorted by the distance to the cluster given by the iterator clustIt
	void computekNNSorting(CharClustIt clustIt, std::vector<CharClustIt>& kNNSorting);
	/// Computes a vector of cluster iterators, sorted by their size (ascending)
	void computeSizeSorting(std::vector<CharClustIt>& sizeSorting);

	/// Sorts the clusters in this clustering corresponding to their distance to the cluster center. This distance is set in the LeaderFollower clustering method!
	void sortAllClustersByDistanceToClusterCenter();

	// --- OPERATORS ---:
	const ClusteringResult& operator=(const ClusteringResult &rhs);

private:
	void addDefaultPrototypeFeatures();

	std::list<CharCluster*> mClusterList; ///< A std::list containing the clusters of this clustering
//	CharCluster mTrashCluster; ///< A cluster of all elements removed from clustering
	std::list<CharCluster*> mTrashClusterList;

	CharFeatureCollection mPrototypeFeatures; ///< determines, which features are computed for cluster prototypes

	ublas::matrix<float> mDistMat; ///< Distance matrix between cluster prototypes
	bool mIsDistMatUpToDate;

}; // end of class ClusteringResult

} // end of namespace impact_tr4


#endif /* CLUSTERINGRESULT_HPP_ */
