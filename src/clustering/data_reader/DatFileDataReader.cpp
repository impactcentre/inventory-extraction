/*
 * DatFileDataReader.cpp
 *
 *  Created on: 04.02.2010
 *      
 */


#include "DatFileDataReader.hpp"

#include "clustering/Document.hpp"

namespace impact_tr4 {

DatFileDataReader::DatFileDataReader()
{

}

#if 0
void DatFileDataReader::parseFiles(Document *pDocument, std::vector<ImageChar> &imageCharVec, std::vector<std::string> &charVec) const
{
	imageCharVec.clear(); charVec.clear();
	// load document images if not done yet:
	if (!pDocument->isDocumentImagesLoaded()) {
		std::cout << "opening document images for dat files!" << std::endl;
		pDocument->openDocumentImages();
	}
	// create XML filenames:
	pDocument->createSegmentationResultFileNames("dat", pDocument->parsingParameters().filenamePrefix,  pDocument->parsingParameters().filenameSuffix);
//	pDocument->createSegmentationResultFileNames("dat");
//	std::cout << "created segmentation result filenames" << std::endl;

	const ParseSubsetType pType = pDocument->parsingParameters().parsingType;

	for (int i=0; i<pDocument->nFiles(); ++i) {
		std::cout << "parsing image file " << i << std::endl;
		this->parseSingleImageFile(pDocument->fileName(i), pDocument->segmentationResultFileName(i), pDocument->imagePointer(i), imageCharVec, charVec, i);
	}

	return;
} // end parseXMLFiles
#endif

void DatFileDataReader::parseSingleImageFile(const std::string fileName, const std::string resultFileName, GrayImage<> *pImage, std::vector<ImageChar> &imageCharVec, const int imageId /*= 0*/) const
{
	std::cout << "parsing dat file: " << resultFileName << std::endl;
//	GrayImage<int> image(resultFileName);

#if 1
	std::ifstream datFile(resultFileName.c_str(), std::ios::in | std::ios::binary);
	unsigned int width, height;

	datFile.read((char*)&width, sizeof(unsigned int));
	datFile.read((char*)&height, sizeof(unsigned int));
	std::cout << "image, width = " << pImage->width() << ", height = " << pImage->height() << std::endl;
	std::cout << "read dat file, width = " << width << ", height = " << height << std::endl;
	unsigned int size = width * height;
	std::vector<unsigned int> imData(size);
	datFile.read((char*)&imData[0], sizeof(unsigned int)*size);

	const int maxBoxes = 50000;
	std::vector<int> maxX(maxBoxes, -1);
	std::vector<int> maxY(maxBoxes, -1);
	std::vector<int> minX(maxBoxes, width+1);
	std::vector<int> minY(maxBoxes, height+1);

	int i=0;
	for (int x=0; x<width; ++x) {
		for (int y=0; y<height; ++y) {
			i = y*width + x;
			if (imData[i]!=0) {
//				std::cout << imData[i] << std::endl;
				int valInd = imData[i]-1;
				if (x > maxX[valInd]) maxX[valInd] = x;
				if (y > maxY[valInd]) maxY[valInd] = y;
				if (x < minX[valInd]) minX[valInd] = x;
				if (y < minY[valInd]) minY[valInd] = y;
			}
		} // end for y
	} // end for x
#if 1
	int nParsedBoxes = 0;
	for (int i=0; i<maxBoxes; ++i) {
		if (maxX[i]!=-1) {
			BoundingBox bbox(minX[i], minY[i], maxX[i]-minX[i]+1, maxY[i]-minY[i]+1);
			ImageChar imChar(bbox, pImage, imageId, imageCharVec.size());
			imageCharVec.push_back(imChar);
			nParsedBoxes++;
		}
	}
	std::cout << "nParsedBoxes = " << nParsedBoxes << std::endl;
#endif

#endif

     return;
} // end parseSingleImageFile


} // end of namespace impact_tr4
