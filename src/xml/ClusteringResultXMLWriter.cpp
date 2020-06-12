/*
 * ClusteringResultXMLWriter.cpp
 *
 *  Created on: 06.08.2009
 *      
 */


#include "ClusteringResultXMLWriter.hpp"

#include "clustering/Document.hpp"
#include "clustering/ClusteringResult.hpp"

namespace impact_tr4 {

ClusteringResultXMLWriter::ClusteringResultXMLWriter(Document *pDocument, ClusteringResult *pClusteringResult)
:mpDocument(pDocument), mpClusteringResult(pClusteringResult)
{
	this->setAutoFormatting(true);
}

bool ClusteringResultXMLWriter::writeFile(QIODevice *device)
{
    setDevice(device);

    writeStartDocument();
    writeDTD("<!DOCTYPE ClusteringResult>");
//    writeStartElement("impact_clustering");
    writeStartElement("clustering");
    writeAttribute("segmentationInputType", QString::fromStdString(type2str(mpDocument->segmentationResultParsingMethod())));

    // Write source image url's:
    for (int i=0; i<mpDocument->nFiles(); ++i) {
    	writeStartElement("inputImage");
    	writeAttribute("url", QString::fromStdString(mpDocument->fileName(i)));
//    	writeAttribute("segmentationResultUrl", QString::fromStdString(mpDocument->segmentationResultFileName(i)));
    	writeAttribute("id", QString::number(i));
    	writeEndElement(); // end element inputImage
    }

//    writeStartElement("clustering");
    // for all clusters:
//    for (int i=0; i<mpClusteringResult->nClusters(); ++i) {
   	for (CharClustConstIt clust_it = mpClusteringResult->begin(); clust_it!= mpClusteringResult->end(); ++clust_it) {
   		CharCluster *pCluster = *clust_it;

    	writeStartElement("cluster");
    	writeAttribute("unicodeLabel", QString::fromStdString(pCluster->unicodeLabelString()));
    	writeAttribute("infoText", QString::fromStdString(pCluster->infoText()));

    	ImageCharConstIt it = pCluster->begin();
		while ( it != pCluster->end() ) {
			writeStartElement("charImage");
			writeAttribute("charId", QString::number((*it)->id));
	    	writeAttribute("imageId", QString::number((*it)->imageID));
	    	writeAttribute("HPOS", QString::number((*it)->bBox.x()));
	    	writeAttribute("VPOS", QString::number((*it)->bBox.y()));
	    	writeAttribute("WIDTH", QString::number((*it)->bBox.width()));
	    	writeAttribute("HEIGHT", QString::number((*it)->bBox.height()));
			writeEndElement(); // end element charImage
			++it;
		} // end while

		writeEndElement(); // end element cluster
    } // end for all clusters

//    writeEndElement(); // end element clustering
    writeEndElement(); // end element clustering

    writeEndDocument();
    std::cout << "Successfully written XML clustering result file!" << std::endl;
    return true;
}

} // end of namespace impact_tr4
