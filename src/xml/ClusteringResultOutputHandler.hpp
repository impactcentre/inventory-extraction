/*
 * ClusteringResultOutputHandler.hpp
 *
 *  Created on: 10.08.2009
 *      
 */

#ifndef CLUSTERINGRESULTOUTPUTHANDLER_HPP_
#define CLUSTERINGRESULTOUTPUTHANDLER_HPP_

#include <QXmlDefaultHandler>
#include <QMessageBox>

#include "core/core.h"
#include "core/Vector2D.hpp"
#include "core/image/GrayImage.hpp"
#include "core/image/BoundingBox.hpp"

#include "clustering/Document.hpp"
#include "clustering/ClusteringResult.hpp"

namespace impact_tr4 {

class ClusteringResultOutputHandler : public QXmlDefaultHandler
{
public:
	/** Default constructor. */
	ClusteringResultOutputHandler(Document *pDocument, ClusteringResult *pClusteringResult);
	/** Event for the start of an XML-tag. */
    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    /** Event for the end of an XML-tag. */
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    /** Event for ingoing characters. */
    bool characters(const QString &str)
    {
	// add incoming characters to current text and return true
    currentText += str;
    return true;
    }
    /** Event for a fatal error, that occured during handling the XMl-file. */
    bool fatalError(const QXmlParseException &exception)
    {
        QMessageBox::critical(0x0, QObject::tr("ClusteringResultOutputHandler"),
                                 QObject::tr("Parse error at line %1, column %2:\n"
                                             "%3")
                                 .arg(exception.lineNumber())
                                 .arg(exception.columnNumber())
                                 .arg(exception.message()));
        return false;

    } // end of fatalError
    /** Returns the current error string. */
    QString errorString() const { return errorStr; }

private:

	bool parseClusteringElement(const QXmlAttributes &attributes);
	bool parseInputImageElement(const QXmlAttributes &attributes);
	bool parseClusterElement(const QXmlAttributes &attributes);
	bool parseCharImageElement(const QXmlAttributes &attributes);
	bool endClusteringElement();

	// PRIVATE MEMBERS:
    QString currentText;
    QString errorStr;

    bool mImpactClusteringElementOpen;
    bool mImpactClusterElementOpen;
    ParsingMethodType mParsedParsingMethod;
    std::vector<std::string> mParsedInputImageFilenames;
//    std::vector<std::vector<ImageChar> > mParsedClusters;

    Document *mpDocument;
    ClusteringResult *mpClusteringResult;
    ClusteringResult tmpClusteringResult;
}; // end class ClusteringResultOutputHandler



} // end of namespace impact_tr4


#endif /* CLUSTERINGRESULTOUTPUTHANDLER_HPP_ */
