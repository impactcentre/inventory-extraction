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
#include "FineReaderXMLOutputHandler.hpp"



namespace impact_tr4 {

FineReaderXMLOutputHandler::FineReaderXMLOutputHandler(/*GrayImage* pImage*/) //: mpImage(pImage)
{
//	if (mpImage==0x0)
//		std::cout << "Warning: Image pointer is 0x0 in FineReaderXMLOutputHandler constructor!" << std::endl;
} // end of default constructor

bool FineReaderXMLOutputHandler::startElement(	const QString &namespaceURI,
												const QString &localName,
												const QString &qName,
												const QXmlAttributes &attributes)
{

//	std::cout << "startElement()" << "\n";
//	std::cout << "namespaceURI = " << namespaceURI.toStdString() << std::endl;
//	std::cout << "localName = " << localName.toStdString() << std::endl;
//	std::cout << "qName = " << qName.toStdString() << std::endl;
//	std::cout << "attributes length = " << attributes.length() << std::endl;

	bool returnVal = true;
	// if current element is charParams --> parse it in corresponding function
	if (qName == "page") {
		returnVal = parsePageElement(attributes);
	}
	else if (qName == "charParams") {
		returnVal =  parseCharParamsElement(attributes);
	}

    currentText.clear();
    return returnVal;
} // end of startElement


bool FineReaderXMLOutputHandler::endElement(const QString &namespaceURI, const QString &localName,
                const QString &qName)
{
	if ( qName == "charParams" ) {
		QString trimmedText = currentText.trimmed();
//		std::cout << trimmedText.toStdString() << std::endl;
		parsedData.parsedChars.push_back(trimmedText.toStdString());

//		int len = trimmedText.length();
//		if (len == 1)
//			current_char = currentText.at(0).toAscii();
//		else if (len == 0)
//			current_char = ' ';
//		else
//			throw Exception("No Character specified in characeter element in Im2CharRectsXMLOutputHandler!");
//
//		parsedData.parsedChars.push_back(current_char);
//		std::cout << "current char = " << current_char << std::endl;
	}

	return true;
} // end of endElement

bool FineReaderXMLOutputHandler::parsePageElement(const QXmlAttributes &attributes)
{
	QString widthStr = attributes.value("width");
	QString heightStr = attributes.value("height");

	int width = widthStr.toInt();
	int height = heightStr.toInt();

	parsedData.pageSize = Vector2i(height, width);
//	parsedData.offset = Vector2i(mpImage->rows(), mpImage->columns()) - Vector2i(height, width);

//	parsedData.offset = Vector2i(offHeight, offWidth); // ???????????????????
//	parsedData.offset = Vector2i(offWidth, offHeight);
	parsedData.offset = Vector2i(-4, -3); // ??????????????????? WHY FIX OFFSET???

	std::cout << "offset is = " << parsedData.offset << std::endl;

	return true;
} // end parse

bool FineReaderXMLOutputHandler::parseCharParamsElement(const QXmlAttributes &attributes)
{
	// parse l, t, r, b values of bbox:
	int l, t, r, b;

	QString lStr = attributes.value("l");
	QString tStr = attributes.value("t");
	QString rStr = attributes.value("r");
	QString bStr = attributes.value("b");

	if (lStr.isEmpty() || tStr.isEmpty() || rStr.isEmpty() || bStr.isEmpty()) {
		errorStr = QObject::tr("Warning: bounding box not correctly specified for charParams element in FineReader XML-output!");
		return false;
	}

	l = lStr.toInt(); t = tStr.toInt(); r = rStr.toInt(); b = bStr.toInt();
//	std::cout << l << ", " << t << ", " << r << ", " << b << std::endl;

	// create bounding box with this values:
	BoundingBox bbox(l,t,r-l,b-t);

	// parse suspicious flag:
	QString isSuspStr = attributes.value("suspicious");
//	std::cout << "isSuspStr = " << isSuspStr.toStdString() << std::endl;
	bool isSusp = false;
	if ( !isSuspStr.isEmpty() && isSuspStr.compare("true")==0 ) {
		isSusp = true;
	}

//	std::cout << bbox << std::endl;
	// add bbox to the list of bboxes, if not empty:
	if (!bbox.empty()) {
		parsedData.parsedBBoxes.push_back(bbox);
		parsedData.isSuspicious.push_back(isSusp);
	}
	return true;
} // end of parseCharParams

} // end of namespace impact_tr4

