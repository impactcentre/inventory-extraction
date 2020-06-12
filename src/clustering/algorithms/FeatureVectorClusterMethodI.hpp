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
#ifndef FEATUREVECTORCLUSTERMETHODI_HPP_
#define FEATUREVECTORCLUSTERMETHODI_HPP_

// global includes:
#include <iostream>

// project includes:
#include "core_headers.h"
#include "image_headers.h"

// local includes:
#include "../CharCluster.hpp"
#include "../CharFeatureCollection.hpp"
#include "../ClusteringResult.hpp"

namespace impact_tr4 {

/**
 * Interface for clustering methods using feature vectors.
 * @deprecated This class should not be used anymore. Use the more generic ClusterMethodI instead!
 */
class FeatureVectorClusterMethodI
{
public:
	FeatureVectorClusterMethodI(CharFeatureCollection* pFeatureCollection, ClusteringResult *pClusteringResult)
		{ setFeatureCollection(pFeatureCollection); mpClusteringResult = pClusteringResult; }
	virtual ~FeatureVectorClusterMethodI() {  }

	void setFeatureCollection( CharFeatureCollection* pFeatureCollection)
		{ mpFeatureCollection = pFeatureCollection; }
	CharFeatureCollection *featuresCollectionPointer()
		{ return mpFeatureCollection; }

#if 0
	void clearClusterVec() { mClusterVec.clear(); }
	void addCluster(const CharCluster& clust) { mClusterVec.push_back(clust); }
	CharCluster* clusterPointer(const int i) { return &mClusterVec[i]; }
	const std::vector<CharCluster>& clusterVecRef() const { return mClusterVec; }
	ClusteringResult* clusteringResultPointer() { return &mClusteringResult; }
#endif

	const ClusterMethodParameters* parameters() const { return mpParams; }
	void setParameters(const ClusterMethodParameters* pParams)
		{ mpParams = pParams; }
	/** Starts the clustering process. Has to be implemented by subclasses. */
	virtual void startClustering() = 0;

protected:
	CharFeatureCollection* mpFeatureCollection; // the given feature data
	const ClusterMethodParameters* mpParams; // the given parameters

	//std::vector<CharCluster> mClusterVec; // the resulting vector of clusters
	ClusteringResult *mpClusteringResult;
}; // end FeatureVectorClusterMethodI

} // end of namespace impact_tr4

#endif /*FEATUREVECTORCLUSTERMETHODI_HPP_*/
