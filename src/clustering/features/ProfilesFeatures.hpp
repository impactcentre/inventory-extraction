/*
 * ProfilesFeatures.hpp
 *
 *  Created on: 12.02.2010
 *      
 */

#ifndef PROFILESFEATURES_HPP_
#define PROFILESFEATURES_HPP_

// system includes:
#include <iostream>

// boost includes:
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

// project includes:
#include "core_headers.h"
#include "image_headers.h"

// local includes:
#include "../DataStructs.hpp"
#include "ImageCharFeatureI.hpp"

//using namespace boost::numeric::ublas;

namespace impact_tr4
{

class ProfilesFeatures : public ImageCharFeatureI
{
public:
	ProfilesFeatures(const int nHorizontalBlocks, const int nVerticalBlocks, bool useHorizontalOuter=true, bool useVerticalOuter=true, bool useHorizontalInner=true, bool useVerticalInner=true);
	virtual ~ProfilesFeatures();

	const int vectorSize() const;

	void calculateOutputVector(const GrayImage<> &image, const int rowIndex, const int colIndex, ublas::matrix<float>& outMatrix);

private:
	int mNHorizontalBlocks;
	int mNVerticalBlocks;
	bool mUseHorizontalOuter;
	bool mUseHorizontalInner;
	bool mUseVerticalOuter;
	bool mUseVerticalInner;
};

}

#endif /* PROFILESFEATURES_HPP_ */
