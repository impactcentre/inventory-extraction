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
#ifndef IMAGEMOMENTS_HPP_
#define IMAGEMOMENTS_HPP_

// system includes:
#include <iostream>
#include <vector>

// project includes:
#include "core_headers.h"
#include "GrayImage.hpp"

//#include "image_headers.h"

// local includes:

namespace impact_tr4
{


/** Struct for moments up to order 3 - Wrap corresponding OpenCv struct. */
typedef struct Moments : public CvMoments { } Moments;
/** Struct for hu moments - Wrap corresponding OpenCv struct. */
typedef struct HuMoments : public CvHuMoments { } HuMoments;
/** Struct for affine invariant moments. */
typedef struct AmiMoments
{
	AmiMoments() : ami1(0),ami2(0),ami3(0),ami4(0) { }
	double ami1;
	double ami2;
	double ami3;
	double ami4;
	static const int size = 4;
} AmiMoments;

/** Struct for basic image moments. */
typedef struct BasicMoments
{
	BasicMoments() : orientation(0),eccentricity(0) { }
	double orientation;
	double eccentricity;
	static const int size = 2;
} BasicMoments;

class ImageMoments
{
public:
	template <class T>
	static void moments(const GrayImage<T>& image, Moments &moments, const bool binary = true)
	{
		cvMoments(image.mpImage, &moments, binary);
		return;
	}

	static void huMoments(const Moments& moments, HuMoments &huMoments)
	{
		cvGetHuMoments((Moments*)&moments, &huMoments);
		return;
	}
	static void amiMoments(const Moments& moments, AmiMoments &amiMoments);
	static void basicMoments(const Moments& moments, BasicMoments &basicMoments);

	// functions utilizing moments:
	static void centerOfMass(const Moments& moments, Vector2D<double> &centerOfMass, const bool binary = true)
	{
		centerOfMass(0) = moments.m10 / moments.m00;
		centerOfMass(1) = moments.m01 / moments.m00;
		return;
	}

	static void computeCenterOfMassIndex(const Moments& moments, Index &centerOfMassIndex, const bool binary = true)
	{
		Vector2D<double> centerOfMass;
		ImageMoments::centerOfMass(moments, centerOfMass, binary);
		centerOfMassIndex(0) = (int)round(centerOfMass(1));
		centerOfMassIndex(1) = (int)round(centerOfMass(0));
		return;
	}

	template <class T>
	static void centerOfMass(const GrayImage<T>& image, Vector2D<double> &centerOfMass, const bool binary = true)
	{
		Moments moments;
		ImageMoments::moments<T>(image, moments, binary);
		ImageMoments::centerOfMass(moments, centerOfMass, binary);
		return;
	}

	template <class T>
	static void centerOfMassIndex(const GrayImage<T>& image, Index &centerOfMassIndex, const bool binary = true)
	{
		Vector2D<double> centerOfMass;
		ImageMoments::centerOfMass<T>(image, centerOfMass, binary);
		centerOfMassIndex(0) = (int)round(centerOfMass(1));
		centerOfMassIndex(1) = (int)round(centerOfMass(0));
		return;
	}
};

} // end of namespace impact_tr4

#endif /*IMAGEMOMENTS_HPP_*/
