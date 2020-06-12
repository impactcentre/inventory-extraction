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
#ifndef FRXMLCHARDATAREADER_HPP_
#define FRXMLCHARDATAREADER_HPP_

#include "xml/xml.h"
#include "xml/FineReaderXMLOutputHandler.hpp"

#include "CharDataReaderI.hpp"

namespace impact_tr4
{

class FRXMLCharDataReader : public CharDataReaderI
{
public:
	FRXMLCharDataReader() : CharDataReaderI() {};
	virtual ~FRXMLCharDataReader() {}

//	void parseFiles(Document *pDocument, std::vector<ImageChar> &imageCharVec, std::vector<std::string> &charVec) const;
	void parseSingleImageFile(const std::string fileName, const std::string xmlFileName, GrayImage<> *pImage, std::vector<ImageChar> &imageCharVec, int imageId = 0) const;
	const std::string fileExtension() const { return std::string("xml"); }
};

} // end of namespace impact_tr4

#endif /*FRXMLCHARDATAREADER_HPP_*/
