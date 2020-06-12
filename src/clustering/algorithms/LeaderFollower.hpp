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
#ifndef LEADERFOLLOWER_HPP_
#define LEADERFOLLOWER_HPP_

#include <iostream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include "core_headers.h"
#include "image_headers.h"

#include "ClusterMethodI.hpp"

#include "../DataStructs.hpp"
#include "../CharCluster.hpp"

namespace impact_tr4 {

class LeaderFollower : public ClusterMethodI
{
public:
	LeaderFollower() : ClusterMethodI() {};
	LeaderFollower(Document *pDocument, ClusteringResult *pClusteringResult, const ClusterMethodParameters* pParameters)
	: ClusterMethodI(pDocument, pClusteringResult, pParameters)
		{
		}
//	LeaderFollower(const std::vector<int>& sampleIndices,
//			Document *pDocument,
//			std::vector<int> *pLabelVector)
//	: ClusterMethodI(sampleIndices, pDocument, pLabelVector)
//		{ };
	virtual ~LeaderFollower() {}

	/** Starts the leader-follower clustering process. */
	void startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult);
private:
	// PRIVATE MEMBERS:

};

} // end of namespace impact_tr4

#endif /*LEADERFOLLOWER_HPP_*/
