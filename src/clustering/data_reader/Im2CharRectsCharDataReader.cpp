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
#include "Im2CharRectsCharDataReader.hpp"
#include "clustering/Document.hpp"

namespace impact_tr4
{

#if 0
void Im2CharRectsCharDataReader::parseFiles(Document *pDocument, std::vector<ImageChar> &imageCharVec, std::vector<std::string> &charVec) const
{
	imageCharVec.clear(); charVec.clear();

	// load document images if not done yet:
	if (!pDocument->isDocumentImagesLoaded()) {
		pDocument->openDocumentImages();
	}
	// create XML filenames:
//	pDocument->createSegmentationResultFileNames();
	pDocument->createSegmentationResultFileNames("xml", pDocument->parsingParameters().filenamePrefix,  pDocument->parsingParameters().filenameSuffix);

	// create the finereader xml output handler object need for parsing the files
	Im2CharRectsXMLOutputHandler *pXMLHandler = new Im2CharRectsXMLOutputHandler();
	// create simple reader and set the content/error handler
    QXmlSimpleReader reader;
    reader.setContentHandler(pXMLHandler);
    reader.setErrorHandler(pXMLHandler);

    int parsedCharsCount = 0;
    for (int i=0; i < pDocument->nFiles(); ++i) {
    	const std::string& xmlFileName = pDocument->segmentationResultFileName(i);
    	// create QFile object
        QFile file( QString::fromStdString(xmlFileName) );
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
        	throw Exception("Error openening xml-file in FRXMLCharDataReader::parseXMLFiles!");
        }
        // create xml input source from file object
        QXmlInputSource xmlInputSource(&file);
        // parse the file using the simple reader
        reader.parse(xmlInputSource);
        std::cout << "Successfully parsed xml data of file " << xmlFileName << std::endl;
    	// store results
        Im2CharRectsData* pData = pXMLHandler->parsedData();
        GrayImage<>* pImage = pDocument->imagePointer(i);
        for (int j=0; j<pData->parsedBBoxes.size(); ++j) {
        	ImageChar imChar(pData->parsedBBoxes[j], pImage, i, parsedCharsCount);
        	imChar.text = pData->parsedChars[j];
        	++parsedCharsCount;
        	imageCharVec.push_back(imChar);
        	charVec.push_back(pData->parsedChars[j]);

        } // end for j
        pXMLHandler->clearData();
    } // end for all xml files
    std::cout << "Successfully parsed " << pDocument->nFiles() << " xml files!" << std::endl;
    delete pXMLHandler;

	return;
}
#endif

void Im2CharRectsCharDataReader::parseSingleImageFile(const std::string fileName, const std::string resultFileName, GrayImage<> *pImage, std::vector<ImageChar> &imageCharVec, const int imageId /*= 0*/) const
{
	// create the finereader xml output handler object need for parsing the files
	Im2CharRectsXMLOutputHandler *pXMLHandler = new Im2CharRectsXMLOutputHandler();
	// create simple reader and set the content/error handler
    QXmlSimpleReader reader;
    reader.setContentHandler(pXMLHandler);
    reader.setErrorHandler(pXMLHandler);

	// create QFile object
    QFile file( QString::fromStdString(resultFileName) );
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
     	throw Exception("Error openening xml-file in FRXMLCharDataReader::parseXMLFiles!");
     }
     // create xml input source from file object
     QXmlInputSource xmlInputSource(&file);
     // parse the file using the simple reader
     reader.parse(xmlInputSource);
     std::cout << "Successfully parsed xml data of file " << resultFileName << std::endl;
 	// store results
     Im2CharRectsData* pData = pXMLHandler->parsedData();
//     imageCharVec.clear();
     for (int j=0; j<pData->parsedBBoxes.size(); ++j) {
     	ImageChar imChar(pData->parsedBBoxes[j], pImage, imageId, imageCharVec.size());
     	imChar.text = pData->parsedChars[j];
     	imageCharVec.push_back(imChar);
     } // end for j
     pXMLHandler->clearData();
     delete pXMLHandler;

     return;
} // end parseSingleImageFile

} // end of namespace impact_tr4
