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
#include "ColorMap.hpp"

namespace impact_tr4 {

void ColorMap::createJetMap()
{
	unsigned char uc=0;
	while (uc<32){
		ucRed[uc]=128+4*uc;
		ucGreen[uc]=0;
		ucBlue[uc++]=0;
    }
	while (uc<96){
		ucRed[uc]=255;
		ucGreen[uc]=(uc-32)*4;
		ucBlue[uc++]=0;
	}
	while (uc<160){
		ucRed[uc]=(160-uc)*4-1;
		ucGreen[uc]=255;
		ucBlue[uc]=(uc-96)*4;
		uc++;
	}
	while (uc<224){
		ucRed[uc]=0;
		ucGreen[uc]=(224-uc)*4-1;
		ucBlue[uc++]=255;
	}
	while(uc>0){
		ucRed[uc]=0;
		ucGreen[uc]=0;
		ucBlue[uc]=127+(256-uc)*4;
		uc++;
	}
}

} // end of namespace impact_tr4

