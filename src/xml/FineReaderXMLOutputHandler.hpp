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
#ifndef FINEREADERXMLOUTPUTHANDLER_HPP_
#define FINEREADERXMLOUTPUTHANDLER_HPP_

#include <QXmlDefaultHandler>
#include <QMessageBox>

#include "core/core.h"
#include "core/Vector2D.hpp"
#include "core/image/GrayImage.hpp"
#include "core/image/BoundingBox.hpp"


namespace impact_tr4 {

/**
 * Struct for parsed data from the FineReader XML-output.
 * */
struct SimpleFineReaderXMLData
{
	Vector2i pageSize;
	Vector2i offset;
	std::vector<BoundingBox> parsedBBoxes;
	std::vector<std::string> parsedChars;
	std::vector<bool> isSuspicious;
}; // end of struct SimpleFineReaderXMLData

/**
 * An XML Handler class for the XML-output files of the Abby-FineReader.
 * This class is designed to handle the output data from the Abby-Finereader OCR enginge, that
 * is written into XML files. See http://www.abbyy.com/FineReader_xml/FineReader6-schema-v1.xml for
 * the complete XML-Schema file for the output files.
 * */
class FineReaderXMLOutputHandler : public QXmlDefaultHandler
{
public:
	/** Default constructor. */
	FineReaderXMLOutputHandler(/*GrayImage* pImage*/);
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
        QMessageBox::information(0x0, QObject::tr("FineReaderXMLOutputHandler"),
                                 QObject::tr("Parse error at line %1, column %2:\n"
                                             "%3")
                                 .arg(exception.lineNumber())
                                 .arg(exception.columnNumber())
                                 .arg(exception.message()));
        return false;

    } // end of fatalError
    /** Returns the current error string. */
    QString errorString() const { return errorStr; }

    /** Clears the parsed data. */
    void clearData()
    	{
    	parsedData.pageSize.set(0,0); parsedData.pageSize.set(0,0);
    	parsedData.parsedBBoxes.clear(); parsedData.parsedChars.clear();
    	}
    SimpleFineReaderXMLData* getFineReaderData() { return &parsedData; }
private:
	// PRIVATE FUNCTIONS:
	/** Parses a page element of the XML-file with the given attributes */
	bool parsePageElement(const QXmlAttributes &attributes);
	/** Parses a charParams element of the XML-file with the given attributes */
	bool parseCharParamsElement(const QXmlAttributes &attributes);

	// PRIVATE MEMBERS:
    QString currentText;
    QString errorStr;

//    GrayImage* mpImage;
    SimpleFineReaderXMLData parsedData;
//    std::vector<char> mParsedChars;
 };

} // end of namespace impact_tr4

#endif /*FINEREADERXMLOUTPUTHANDLER_HPP_*/
