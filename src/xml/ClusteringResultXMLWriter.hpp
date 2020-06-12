/*
 * ClusteringResultXMLWriter.hpp
 *
 *  Created on: 06.08.2009
 *      
 */

#ifndef CLUSTERINGRESULTXMLWRITER_HPP_
#define CLUSTERINGRESULTXMLWRITER_HPP_

#include <QXmlStreamWriter>

#include "clustering/Document.hpp"
#include "clustering/ClusteringResult.hpp"

namespace impact_tr4 {

class ClusteringResultXMLWriter : public QXmlStreamWriter
{
public:
	ClusteringResultXMLWriter(Document *pDocument, ClusteringResult *pClusteringResult);
    bool writeFile(QIODevice *device);

private:
	Document *mpDocument;
	ClusteringResult *mpClusteringResult;
};

} // end of namespace impact_tr4


#endif /* CLUSTERINGRESULTXMLWRITER_HPP_ */
