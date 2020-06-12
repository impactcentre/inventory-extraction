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
#ifndef BINARYIMAGEMOMENTS_HPP_
#define BINARYIMAGEMOMENTS_HPP_

// system includes:
#include <iostream>
#include <vector>

// project includes:
#include "core/Index.hpp"

// local includes:

namespace impact_tr4
{


/**
 * This class contains functions for computing image moments of binary images.
 * A binary image here is represented by its object indices i.e. a vector, containing
 * all indices of the binary image, that are non-zero.
 * Note, that the object indices are stored in a (row, column) fashion, i.e.
 * the y-coordinates are stored before the x-coordinates! This can be confusing,
 * as image moments are computed in x-first fashion, meaning that for example
 * the function rawMoment10 computes the first order moment in x-direction, i.e.
 * in column-direction concerning object indices!
 * */
class BinaryImageMoments
{
public:
	BinaryImageMoments();
	virtual ~BinaryImageMoments();
	// RAW MOMENTS:
	static float_t raw00(const std::vector<Index> &objectIndices)
		{ return (float_t)(objectIndices.size()); }
	static float_t raw10(const std::vector<Index> &objectIndices);
	static float_t raw01(const std::vector<Index> &objectIndices);
	/** Computes the raw moment of order p+q. */
	static float_t raw(const std::vector<Index> &objectIndices, const int p, const int q);

	/** Returns the centroid of the binary object. */
	static Vector2D<float_t> centroid(const std::vector<Index> &objectIndices);

	// CENTRAL MOMENTS:
	/** Computes the central moment of order p+q. */
	static float_t central(const std::vector<Index> &objectIndices, const int p, const int q);
	static float_t central00(const std::vector<Index> &objectIndices)
		{ return raw00(objectIndices); }
	static float_t central10(const std::vector<Index> &objectIndices)
		{ return 0.0f; }
	static float_t central01(const std::vector<Index> &objectIndices)
		{ return 0.0f; }
	/** Computes the normalized central moment of order p+q. */
	static float_t normCentral(const std::vector<Index> &objectIndices, const int p, const int q);
	static float_t normCentral00(const std::vector<Index> &objectIndices)
		{ return 1.0f; }
	static float_t normCentral10(const std::vector<Index> &objectIndices)
		{ return 0.0f; }
	static float_t normCentral01(const std::vector<Index> &objectIndices)
		{ return 0.0f; }

	// HU MOMENTS:
	/** Computes the hu moments 1-4 and stores them in the vector momentsVec. */
	static void hu1To4(const std::vector<Index> &objectIndices, std::vector<float_t> &momentsVec);

	/** Computes the AMI moments 1-4 and stores them in the vector momentsVec. */
	static void ami1To4(const std::vector<Index> &objectIndices, std::vector<float_t> &momentsVec);
};

typedef BinaryImageMoments BinImMom;

} // end of namespace impact_tr4
#endif /*BINARYIMAGEMOMENTS_HPP_*/
