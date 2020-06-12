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
#include "FineReaderData.hpp"
#include <windows.h>
//namespace fs = boost::filesystem;


namespace impact_tr4 {

FRCharIterator FRData::begin()
{
	return(FRCharIterator(this, 0, 0, 0, 0));
}

FRCharIterator FRData::end()
{
	FRTextBlock textblock = this->getTextBlock(this->nTextBlocks()-1);
	FRParagraph paragraph = textblock.getParagraph(textblock.nParagraphs()-1);
	FRLine line = paragraph.getLine(paragraph.nLines()-1);
//	FRCharacter character = line.getCharacter(line.nCharacters()-1);
	return(FRCharIterator(this, this->nTextBlocks(),
								textblock.nParagraphs()-1,
								paragraph.nLines()-1,
								line.nCharacters()-1));

}


} // end of namespace impact_tr4
