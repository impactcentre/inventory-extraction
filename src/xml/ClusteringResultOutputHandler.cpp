/*
 * ClusteringResultOutputHandler.cpp
 *
 *  Created on: 10.08.2009
 *      
 */


#include "ClusteringResultOutputHandler.hpp"

namespace impact_tr4 {

ClusteringResultOutputHandler::ClusteringResultOutputHandler(Document *pDocument, ClusteringResult *pClusteringResult)
: mpDocument(pDocument), mpClusteringResult(pClusteringResult)
{
	mImpactClusteringElementOpen = false;
	mImpactClusterElementOpen = false;
}

bool ClusteringResultOutputHandler::startElement( const QString &namespaceURI,
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
//	std::cout << qName.toStdString() << std::endl;
	// if current element is charParams --> parse it in corresponding function
	if (qName == "clustering") {
		tmpClusteringResult.deleteClustering();
		mpDocument->clearDocument();
		returnVal = this->parseClusteringElement(attributes);
	}
	else if (qName == "inputImage") {
		returnVal = this->parseInputImageElement(attributes);
	}
	else if (qName == "cluster") {
		returnVal = this->parseClusterElement(attributes);
	}
	else if (qName == "charImage") {
		returnVal = this->parseCharImageElement(attributes);
	}

    currentText.clear();
    return returnVal;
} // end of startElement


bool ClusteringResultOutputHandler::endElement(const QString &namespaceURI, const QString &localName,
                const QString &qName)
{
	if ( qName == "clustering" ) {
		mImpactClusteringElementOpen = false;
		return endClusteringElement();
	}
	else if ( qName == "cluster" ) {
		mImpactClusterElementOpen = false;
	}

	return true;
} // end of endElement

bool ClusteringResultOutputHandler::parseClusteringElement(const QXmlAttributes &attributes)
{
	mImpactClusteringElementOpen = true;
	mImpactClusterElementOpen = false;
	mParsedInputImageFilenames.clear();

	QString segmentationInputTypeStr = attributes.value("segmentationInputType");
	if (segmentationInputTypeStr.isEmpty()) {
		errorStr = QObject::tr("No segmentationInputType specified in clustering element!");
		return false;
	}
	if (segmentationInputTypeStr.compare("abbyyXML")==0) {
		mParsedParsingMethod = FINEREADER_XML;
	}
	else if (segmentationInputTypeStr.compare("im2CharRectsXML")==0) {
		mParsedParsingMethod = IM2CHARRECTS_XML;
	}
	else if (segmentationInputTypeStr.compare("datFiles")==0) {
		mParsedParsingMethod = DAT_FILES;
//		errorStr = QObject::tr("reading clustering element: dat files not supported yet!");
//		return false;
	}
	else {
		errorStr = QObject::tr("Unknown segmentationInputType specified in clustering element!");
		return false;
	}

	return true;
}

bool ClusteringResultOutputHandler::parseInputImageElement(const QXmlAttributes &attributes)
{
	if (mImpactClusteringElementOpen) {
		QString urlStr = attributes.value("url");
//		std::cout << "Input image parsed: " << urlStr.toStdString() << std::endl;
		if (urlStr.isEmpty()) {
			errorStr = QObject::tr("No url specified in inputImage element!");
			return false;
		}
		mParsedInputImageFilenames.push_back(urlStr.toStdString());

		return true;
	} // end if (mImpactClusteringElementOpen)
	errorStr = "No clustering element opened for inputImage element!";
	return false;
}

bool ClusteringResultOutputHandler::parseClusterElement(const QXmlAttributes &attributes)
{
	if (mImpactClusteringElementOpen) {
//		std::cout << "parsing cluster element" << std::endl;
		mImpactClusterElementOpen = true;
		CharCluster *pCluster = tmpClusteringResult.addEmptyCluster(mpDocument);
		QString unicodeLabelTxt = attributes.value("unicodeLabel");
		QString infoTextStr = attributes.value("infoText");

//		std::cout << "parsed unicode str and info text = " << unicodeLabelTxt.toStdString() << ", " << infoTextStr.toStdString() << std::endl;

		pCluster->setUnicodeLabel(unicodeLabelTxt.toStdString());
		pCluster->setInfoText(infoTextStr.toStdString());
		return true;
	} // end if (mImpactClusteringElementOpen)
	errorStr = "No clustering element opened for cluster element!";
	return false;
}

bool ClusteringResultOutputHandler::parseCharImageElement(const QXmlAttributes &attributes)
{
	if (mImpactClusteringElementOpen && mImpactClusterElementOpen) {
//		std::cout << "parsing image char element" << std::endl;
		QString charIdStr = attributes.value("charId");
		QString imageIdStr = attributes.value("imageId");
		QString hposStr = attributes.value("HPOS");
		QString vposStr = attributes.value("VPOS");
		QString widthStr = attributes.value("WIDTH");
		QString heightStr = attributes.value("HEIGHT");
		if (charIdStr.isEmpty() || imageIdStr.isEmpty()||hposStr.isEmpty()||vposStr.isEmpty()||widthStr.isEmpty()||heightStr.isEmpty()) {
			errorStr = QObject::tr("Either charId, imageId, HPOS, VPOS, WIDTH or HEIGHT not specified in charImage element!");
			return false;
		}
		int id = charIdStr.toInt();
		int image_id = imageIdStr.toInt();
		int x = hposStr.toInt();
		int y = vposStr.toInt();
		int width = widthStr.toInt();
		int height = heightStr.toInt();
//		std::cout << image_id << ", " << x << ", " << y << ", " << width << ", " << height << std::endl;
		BBox bbox(x, y, width, height);
		// create ImageChar object with parsed bounding box and image_id;
		// the image pointer is set to 0x0 because it is set later, when clustering element is closed!
		ImageChar tmpImageChar(bbox, 0x0, image_id, id);
		// add it to document:
		ImageChar *imCharPointerInDoc = mpDocument->addImageChar(tmpImageChar);
//		std::cout << imCharPointerInDoc->bBox << std::endl;
		// add ImageChar pointer to actual cluster (i.e. the last cluster in list):
		tmpClusteringResult.back()->addChar(imCharPointerInDoc);
//		std::cout << *imCharPointerInDoc << std::endl;//should be the same as next one!
//		std::cout << *tmpClusteringResult[tmpClusteringResult.nClusters()-1]->charList().back() << std::endl;

		return true;
	} // end if (mImpactClusteringElementOpen)
	errorStr = "No clustering and cluster element opened for charImage element!";
	return false;
}

bool ClusteringResultOutputHandler::endClusteringElement()
{
//	std::cout << "ending clustering element" << std::endl;
	// set document files:
//	mpDocument->clearDocument();
	mpDocument->setDocumentFileNames(mParsedInputImageFilenames);
	// open corresponding filesand update image pointers:
	mpDocument->openDocumentImages();
	mpDocument->setSegmentationResultParsingMethod(mParsedParsingMethod);
	mpDocument->updateImagePointers();

	// set temp-clustering result to parsed one:
//	tmpClusteringResult.print();
	*mpClusteringResult = tmpClusteringResult;
	mpClusteringResult->updatePrototypes();
	mpClusteringResult->computePrototypeFeatures();

//	mpDocument->printImageChars();
//	mpClusteringResult->print();

	return true;
}

} // end of namespace impact_tr4
