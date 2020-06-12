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
#ifndef KMEANS_HPP_
#define KMEANS_HPP_

#include <iostream>

#include "core_headers.h"
#include "image_headers.h"

#include "ClusterMethodI.hpp"
#include "../DataStructs.hpp"
#include "../CharCluster.hpp"

namespace impact_tr4 {

class KMeans : public ClusterMethodI
{
public:
	KMeans() : ClusterMethodI() {};
	KMeans(Document *pDocument, ClusteringResult *pClusteringResult, const ClusterMethodParameters* pParameters)
	: ClusterMethodI(pDocument, pClusteringResult, pParameters)
		{
		}

	/** Starts the k-means clustering process. */
	void startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult);

private:
	// PRIVATE MEMBERS:

}; // end class KMeans

} // end of namespace impact_tr4

#endif /*KMEANS_HPP_*/
