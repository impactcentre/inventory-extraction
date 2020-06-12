/*
 * ClusteringThread.hpp
 *
 *  Created on: 16.11.2010
 *      
 */

#ifndef CLUSTERINGTHREAD_HPP_
#define CLUSTERINGTHREAD_HPP_

#include <qthread.h>

namespace impact_tr4 {

class ClusteringThread: public QThread {
public:
	ClusteringThread();
	virtual ~ClusteringThread();

	void run();
};

}

#endif /* CLUSTERINGTHREAD_HPP_ */
