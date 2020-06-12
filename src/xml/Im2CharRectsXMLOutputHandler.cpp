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
#include "Im2CharRectsXMLOutputHandler.hpp"

namespace impact_tr4 {

Im2CharRectsXMLOutputHandler::Im2CharRectsXMLOutputHandler(/*GrayImage* pImage*/) //: mpImage(pImage)
{
//	if (mpImage==0x0)
//		std::cout << "Warning: Image pointer is 0x0 in Im2CharRectsXMLOutputHandler constructor!" << std::endl;
	actualLine = -1;
} // end of default constructor

bool Im2CharRectsXMLOutputHandler::startElement( const QString &namespaceURI,
												 const QString &localName,
												 const QString &qName,
												 const QXmlAttributes &attributes )
{
//	std::cout << "startElement()" << "\n";
//	std::cout << "namespaceURI = " << namespaceURI.toStdString() << std::endl;
//	std::cout << "localName = " << localName.toStdString() << std::endl;
//	std::cout << "qName = " << qName.toStdString() << std::endl;
//	std::cout << "attributes length = " << attributes.length() << std::endl;

	bool returnVal = true;
	// if current element is charParams --> parse it in corresponding function
	if (qName == "Page") {
		returnVal = parsePageElement(attributes);
	}
	else if (qName == "Paragraph") {
		parseParagraphElement(attributes);
	}
	else if (qName == "Line") {
		parseLineElement(attributes);
	}
	else if (qName == "Char") {
		returnVal =  parseCharElement(attributes);
	}

    currentText.clear();
    return returnVal;
} // end of startElement


bool Im2CharRectsXMLOutputHandler::endElement(const QString &namespaceURI, const QString &localName,
                const QString &qName)
{
	if ( qName == "Char" ) {
		QString trimmedText = currentText.trimmed();
//		std::cout << trimmedText.toStdString() << std::endl;
		mParsedData.parsedChars.push_back(trimmedText.toStdString());

//		int len = trimmedText.length();
//		if (len == 1)
//			current_char = currentText.at(0).toAscii();
//		else if (len == 0)
//			current_char = ' ';
//
//		mParsedData.parsedChars.push_back(current_char);
//		std::cout << "current char = " << current_char << std::endl;
	}

	return true;
} // end of endElement

bool Im2CharRectsXMLOutputHandler::parsePageElement(const QXmlAttributes &attributes)
{
//	QString widthStr = attributes.value("width");
//	QString heightStr = attributes.value("height");
//
//	int width = widthStr.toInt();
//	int height = heightStr.toInt();
//
//	mParsedData.pageSize = Vector2i(height, width);
////	mParsedData.offset = Vector2i(mpImage->rows(), mpImage->columns()) - Vector2i(height, width);
//
////	mParsedData.offset = Vector2i(offHeight, offWidth); // ???????????????????
////	mParsedData.offset = Vector2i(offWidth, offHeight);
//	mParsedData.offset = Vector2i(-4, -3); // ??????????????????? WHY FIX OFFSET???
//
//	std::cout << "offset is = " << mParsedData.offset << std::endl;

	return true;
} // end parse

bool Im2CharRectsXMLOutputHandler::parseParagraphElement(const QXmlAttributes &attributes)
{
	// do nothing here...
	return true;
}

bool Im2CharRectsXMLOutputHandler::parseLineElement(const QXmlAttributes &attributes)
{
	actualLine = attributes.value("Number").toInt();
	return true;
}

bool Im2CharRectsXMLOutputHandler::parseCharElement( const QXmlAttributes &attributes )
{
	// parse l, t, r, b values of bbox:
	int l, t, r, b;

	QString lStr = attributes.value("Left");
	QString tStr = attributes.value("Top");
	QString rStr = attributes.value("Right");
	QString bStr = attributes.value("Bottom");

	if ( lStr.isEmpty() || tStr.isEmpty() || rStr.isEmpty() || bStr.isEmpty() ) {
		errorStr = QObject::tr("Warning: bounding box not correctly specified for charParams element in FineReader XML-output!");
		return false;
	}

	l = lStr.toInt(); t = tStr.toInt(); r = rStr.toInt(); b = bStr.toInt();
//	std::cout << l << ", " << t << ", " << r << ", " << b << std::endl;

	// create bounding box with this values:
	BoundingBox bbox(l,t,r-l,b-t);

//	std::cout << bbox << std::endl;
	// add bbox to the list of bboxes:
	mParsedData.parsedBBoxes.push_back(bbox);
	return true;
} // end of parseCharParams

} // end of namespace impact_tr4

