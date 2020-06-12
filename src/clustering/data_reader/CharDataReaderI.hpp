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
#ifndef XMLCHARDATAREADERI_HPP_
#define XMLCHARDATAREADERI_HPP_

// system includes:
#include <iostream>

// project includes:
#include "core_headers.h"
#include "image_headers.h"
//
#include "util/util.h"

// local includes:
#include "../DataStructs.hpp"

namespace impact_tr4
{
class Document;

class CharDataReaderI
{
public:
	CharDataReaderI();
	virtual ~CharDataReaderI();
	/** Parses the XML files specified in the given document and stores the resuling data. Must be implemented by subclass. */
//	virtual void parseFiles(Document *pDocument, std::vector<ImageChar> &imageCharVec, std::vector<std::string> &charVec) const = 0;
	void parseFiles(Document *pDocument, std::vector<ImageChar> &imageCharVec) const;
	virtual void parseSingleImageFile(const std::string fileName, const std::string resultFileName, GrayImage<> *pImage, std::vector<ImageChar> &imageCharVec, const int imageId = 0) const = 0;
	virtual const std::string fileExtension() const = 0;

};

} // end of namespace impact_tr4

#endif /*XMLCHARDATAREADERI_HPP_*/
