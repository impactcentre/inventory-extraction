/*
 * FeatureExtractor.h
 *
 *  Created on: 12.02.2010
 *      
 */

#ifndef FEATUREEXTRACTOR_H_
#define FEATUREEXTRACTOR_H_

// project includes:
#include "core_headers.h"
#include "image_headers.h"

namespace impact_tr4
{

class FeatureExtractor
{
public:
	FeatureExtractor(GrayImage<> *pImage);
	virtual ~FeatureExtractor();

private:
	GrayImage<> *mpImage;

};

}

#endif /* FEATUREEXTRACTOR_H_ */
