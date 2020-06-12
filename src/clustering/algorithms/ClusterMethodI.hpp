/*
 * ClusterMethodI.hpp
 *
 *  Created on: 27.01.2010
 *      
 */

#ifndef CLUSTERMETHODI_HPP_
#define CLUSTERMETHODI_HPP_

#include <QThread>
#include <QMessageBox>

#include <iostream>
#include <vector>

#include "core_headers.h"

namespace impact_tr4 {

class Document;
class ClusteringResult;
class ClusterMethodParameters;

/**
 * Generic abstract class interface for all clustering methods.
 */
class ClusterMethodI : public QThread
{
	Q_OBJECT
public:
	ClusterMethodI() {};
	virtual ~ClusterMethodI() // not necessary I think...
		{ mpDocument = 0x0; mpClusteringResult = 0x0; mpLabelVector = 0x0; };

	ClusterMethodI(Document *pDocument, ClusteringResult *pClusteringResult, const ClusterMethodParameters* pParameters)
		: mpDocument(pDocument), mpClusteringResult(pClusteringResult), mpParameters(pParameters)
		{};

//	ClusterMethodI(	const std::vector<int>& sampleIndices,
//					Document *pDocument,
//					std::vector<int> *pLabelVector)
//		{
//		mSampleIndices = sampleIndices;
//		mpDocument = pDocument;
//		mpLabelVector = pLabelVector;
//		};

	std::vector<int> getLabelVector() const { return  *mpLabelVector; }
	std::vector<int> getSampleIndices() const { return mSampleIndices; }

	/**
	 * Pure virtual method to start the clustering process with a generic set of parameters
	 * specified.
	 * @param[in] pParameters	Pointer to the parameters for the clustering process stored
	 *							in the generic class ClusterMethodParameters
	 */
	virtual void startClustering(const ClusterMethodParameters* pParameters, Document *pDocument, ClusteringResult *pClusteringResult) = 0;

	void run() {
		try {
			this->startClustering(mpParameters, mpDocument, mpClusteringResult);
			emit clusteringFinished(true);
		}
		catch (Exception e) {
			std::cerr << "An exception has occured in ClusterMethodI::run() " << e.what() << std::endl;
			QMessageBox::critical(NULL,"An error has occured while clustering!", e.what() );
			emit clusteringFinished(false);
		}
	}
signals:
	void clusteringFinished(bool success);

protected:
	Document *mpDocument;
	ClusteringResult *mpClusteringResult;
	const ClusterMethodParameters *mpParameters;

	std::vector<int> mSampleIndices;
	std::vector<int> *mpLabelVector;
//	double (Document::*mfpDistance)(const int i, const int j) const;
}; // end of class ClusterMethodI.hpp

} // end of namespace impact_tr4


#endif /* CLUSTERMETHODI_HPP_ */
