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
#ifndef IMAGEINTEGRALINVARIANTFEATURE_HPP_
#define IMAGEINTEGRALINVARIANTFEATURE_HPP_

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

namespace impact_tr4
{
//using namespace boost::numeric::ublas;

class ImageIntegralInvariantFeature : public ImageCharFeatureI
{
public:
	ImageIntegralInvariantFeature(Kernel<>& mask, const int histogramType = 0) /*: mMask(mask)*/
		{
		mMask = mask;
		mHistogramType = histogramType;
		}
	virtual ~ImageIntegralInvariantFeature() {}

	const int vectorSize() const
		{ return mMask.countNonZero(); }
	void calculateOutputVector(const GrayImage<> &image, const int rowIndex, const int colIndex, ublas::matrix<float>& outMatrix);
private:
	Kernel<> mMask;
	int mHistogramType;
};

} // end of namespace impact_tr4

#endif /*IMAGEINTEGRALINVARIANTFEATURE_HPP_*/
