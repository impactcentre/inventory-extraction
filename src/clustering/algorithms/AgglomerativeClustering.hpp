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
#ifndef AGGLOMERATIVECLUSTERING_HPP_
#define AGGLOMERATIVECLUSTERING_HPP_

#include <iostream>
#include <fstream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

#include <QThread>

#include "core_headers.h"
#include "image_headers.h"

#include "util/std_util.h"
#include "util/ublas_util.h"

#include "ClusterMethodI.hpp"

#include "../DataStructs.hpp"
#include "../CharCluster.hpp"

#include "ext/cluster/cluster.h"

namespace impact_tr4 {

class AgglomerativeClustering  : public ClusterMethodI
{
	Q_OBJECT
public:
	AgglomerativeClustering() : ClusterMethodI() {};

	AgglomerativeClustering(Document *pDocument, ClusteringResult *pClusteringResult, const ClusterMethodParameters* pParameters)
	: ClusterMethodI(pDocument, pClusteringResult, pParameters)
		{
		}

	virtual ~AgglomerativeClustering() {}

	/** Starts the agglomerative clustering process. */
	void startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult);

	void startClusteringOnClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult);

private:
#if 0
	void setFeatureDistanceTypeFunctionPointer(const FeatureDistanceType distType);
#endif

	/** Sets the distance type function pointer depending on given type. */
	void setClusterDistanceTypeFunctionPointer(const ClusterDistanceType distType);

	/** Initializes the distance matrix. */
	void initDistanceMatrix();

	/** Returns the value, containing the minimum distance of the
	 *  distance matrix and sets minIndex to the corresponding index. */
	void findMinDist();

	/** Update the distance matrix and the cluster
	 * vector corresponding to the given merge index. */
	void updateClusterLabels();

	// PRIVATE MEMBERS:
//	ublas::symmetric_matrix<float> mDistMat;
	ublas::matrix<float> mDistMat;
	ublas::matrix<float> mDistMatOrig;
	ublas::matrix<float> mInvCovMat;
	Vector2i mMinIndex;
	float mMinDist;
//	std::vector<int> mLabels;

	// function pointer to distance function
	float (*mfpDistance)(const CharCluster& c1, const CharCluster& c2, const ublas::matrix<float>& distMat);
};

} // end of namespace impact_tr4

#endif /*AGGLOMERATIVECLUSTERING_HPP_*/
