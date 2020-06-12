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
#include "CharDataReaderI.hpp"

#include "clustering/Document.hpp"
#include "util/RandomNumber.hpp"

namespace impact_tr4
{

CharDataReaderI::CharDataReaderI()
{
}

/*virtual*/ CharDataReaderI::~CharDataReaderI()
{
} // end destructor

void CharDataReaderI::parseFiles(Document *pDocument, std::vector<ImageChar> &imageCharVec) const
{
	imageCharVec.clear();

	ParsingParameters pars = pDocument->parsingParameters();
	const ParseSubsetType pType = pars.parsingType;
	const SubsetGenerationType genType = pars.generationType;
	const ParsingMethodType mType = pars.parsingMethod;

	if (mType == UNKNOWN_PARSING_METHOD) {
		throw Exception("An invalid parsing method type was given!");
	}
	if (pType == UNKNOWN_PARSESUBSET_TYPE) {
		throw Exception("An invalid parsing subset type was given!");
	}
	if (genType == UNKNOWN_GENERATION_TYPE) {
		throw Exception("An invalid parsing subset generation type was given!");
	}

	// load document images if not done yet:
	if (!pDocument->isDocumentImagesLoaded()) {
		throw NoDataException("No input documents opened for parsing!");

//		std::cout << "Opening document images for parsing, parsing type is: " << pars.parsingMethod << std::endl;
//		pDocument->openDocumentImages();
	}
	// create XML filenames:
	std::string fileExtension = "";
	if (mType == FINEREADER_XML || mType == IM2CHARRECTS_XML) {
		fileExtension = "xml";
	}
	else if (mType == DAT_FILES) {
		fileExtension = "dat";
	}

	std::cout << "file extension = " << fileExtension << std::endl;
	pDocument->createSegmentationResultFileNames(fileExtension, pars.filenamePrefix,  pars.filenameSuffix);
//	std::cout << "created segmentation result filenames" << std::endl;

	for (int i=0; i<pDocument->nFiles(); ++i) {
		std::cout << "Now parsing image file " << i << std::endl;

		// 1st: parse single image file, store result in tmp-vectors:
		std::vector<ImageChar> tmpImageCharVec;
		this->parseSingleImageFile(pDocument->fileName(i), pDocument->segmentationResultFileName(i), pDocument->imagePointer(i), tmpImageCharVec, i);

		// 2nd: sort out susp. / non-susp. if needed, store result in tmp-vectors:
		std::vector<ImageChar> tmpImageCharVec2;
		for (int j=0; j<tmpImageCharVec.size(); ++j) {
			if ((pType == PARSE_ALL) || (pType == PARSE_ONLY_NON_SUSP && !tmpImageCharVec[j].suspicious) || (pType == PARSE_ONLY_SUSP && tmpImageCharVec[j].suspicious)) {
				tmpImageCharVec2.push_back(tmpImageCharVec[j]);
			}
		} // end for all parsed chars j

		// 3rd: generate subset of parsed chars according to generation type:
		tmpImageCharVec.clear();

		double percentage = double(pars.subsetPerc) / 100.0f;
		int nCharsToPars = ceil(tmpImageCharVec2.size() * percentage);
		assert(nCharsToPars<=tmpImageCharVec2.size());
		std::cout << "Parsing " << pars.subsetPerc << "% of " << tmpImageCharVec2.size() << " nr of chars = " << nCharsToPars << std::endl;
		if (genType == PARSE_RANDOMLY) { // take nCharsToPars nr of characters randomly from page
			std::vector<int> rand_tuple = RandomNumber::random_unique_integer_tuple(tmpImageCharVec2.size());
			for (int k=0; k<nCharsToPars; k++) {
				imageCharVec.push_back(tmpImageCharVec2[rand_tuple[k]]);
				imageCharVec[imageCharVec.size()-1].id = k;
			}
		}
		else if (genType == PARSE_FIRST) { // take nCharsToPars nr of characters first from page
			for (int k=0; k<nCharsToPars; k++) {
				imageCharVec.push_back(tmpImageCharVec2[k]);
				imageCharVec[imageCharVec.size()-1].id = k;
			}
		}
		else if (genType == PARSE_LAST) {
			for (int l=0; l<nCharsToPars; l++) {
				int k = tmpImageCharVec2.size()-l-1;
//				std::cout << "k = " << k << std::endl;
//				std::cout << "(tmpImageCharVec2.size()-nCharsToPars) = " << (tmpImageCharVec2.size()-nCharsToPars) << std::endl;
				imageCharVec.push_back(tmpImageCharVec2[k]);
				imageCharVec[imageCharVec.size()-1].id = k;
			}
		}
	} // end for all files i

	return;
}

} // end of namespace impact_tr4
