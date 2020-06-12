/*
 * ColorMap.hpp
 *
 *  Created on: 20.07.2010
 *      
 */

#ifndef COLORMAP_HPP_
#define COLORMAP_HPP_

#include "core/core.h"

namespace impact_tr4 {

class ColorMap
{
public:
	uint8 red(unsigned char ucAt){return ucRed[ucAt];}
	uint8 green(unsigned char ucAt){return ucGreen[ucAt];}
	uint8 blue(unsigned char ucAt){return ucBlue[ucAt];}
	uint8 red(int iAt){return ucRed[iAt];}
	uint8 green(int iAt){return ucGreen[iAt];}
	uint8 blue(int iAt){return ucBlue[iAt];}
	uint8 redSec(int iAt){return ucRed[(unsigned char)iAt];}
	uint8 greenSec(int iAt){return ucGreen[(unsigned char)iAt];}
	uint8 blueSec(int iAt){return ucBlue[(unsigned char)iAt];}

	void createJetMap();
private:
	uint8 ucRed[256], ucGreen[256], ucBlue[256];
};

}

#endif /* COLORMAP_HPP_ */
