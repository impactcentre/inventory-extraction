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
#include "BinaryImageMoments.hpp"

#include <iostream>

namespace impact_tr4
{

/*static*/ float_t
BinaryImageMoments::raw10(const std::vector<Index> &objectIndices)
{
	float_t moment10 = 0.0f;
	for (int k=0; k<objectIndices.size(); ++k) {
		moment10 += objectIndices[k](1); // sum up x-indices, i.e. column indices
	} // end for all k
	return moment10;
}

/*static*/ float_t
BinaryImageMoments::raw01(const std::vector<Index> &objectIndices)
{
	float_t moment01 = 0.0f;
	for (int k=0; k<objectIndices.size(); ++k) {
		moment01 += objectIndices[k](0); // sum up y-indices, i.e. row indices
	} // end for all k
	return moment01;
}

/*static*/ float_t
BinaryImageMoments::raw(const std::vector<Index> &objectIndices, const int p, const int q)
{
	float_t moment = 0.0f;
	for (int k=0; k<objectIndices.size(); ++k) {
		moment += pow((float_t)objectIndices[k](1), p) * pow((float_t)objectIndices[k](0), q);
	} // end for all k
	return moment;
}

/*static*/ float_t
BinaryImageMoments::central(const std::vector<Index> &objectIndices, const int p, const int q)
{
	float_t moment = 0.0f;
	const Vector2D<float> centroid = BinaryImageMoments::centroid(objectIndices);
	for (int k=0; k<objectIndices.size(); ++k) {
		moment += pow((float_t)(objectIndices[k](1)-centroid(0)), p)
				* pow((float_t)(objectIndices[k](0)-centroid(1)), q);
	} // end for all k
	return moment;
}

/*static*/ float_t
BinaryImageMoments::normCentral(const std::vector<Index> &objectIndices, const int p, const int q)
{
	const float_t w = (float_t)(p+q)/2.0f + 1.0f;
	const float_t centMom00Pow = pow(central(objectIndices, 0, 0), w);
	float_t moment = central(objectIndices, p, q) / centMom00Pow;
	return moment;
}

/*static*/ Vector2D<float_t>
BinaryImageMoments::centroid(const std::vector<Index> &objectIndices)
{
	Vector2D<float_t> centroid(0,0);
	if (objectIndices.empty()) {
		return centroid;
	}

	float_t moment00 = (float_t)BinaryImageMoments::raw00(objectIndices);
	centroid(0) = raw10(objectIndices) / moment00;
	centroid(1) = raw01(objectIndices) / moment00;
	return centroid;
}

/*static*/ void
BinaryImageMoments::hu1To4(const std::vector<Index> &objectIndices, std::vector<float_t> &momentsVec)
{
	// resize output vector:
	momentsVec.resize(4);

	if (objectIndices.empty()) {
		for (int i=0; i<momentsVec.size(); ++i) momentsVec[i] = 0;
		return;
	}

	// compute the needed normalized central moments:
	const float_t nu20 = normCentral(objectIndices, 2, 0);
	const float_t nu02 = normCentral(objectIndices, 0, 2);
	const float_t nu11 = normCentral(objectIndices, 1, 1);
	const float_t nu30 = normCentral(objectIndices, 3, 0);
	const float_t nu03 = normCentral(objectIndices, 0, 3);
	const float_t nu12 = normCentral(objectIndices, 1, 2);
	const float_t nu21 = normCentral(objectIndices, 2, 1);
	// compute the hu moments:
	momentsVec[0] = nu20 + nu02;
	momentsVec[1] = pow((nu20 - nu02),2) + 4*pow(nu11, 2);
	momentsVec[2] = pow((nu30 - 3*nu12),2) + pow((3*nu21-nu03), 2);
	momentsVec[3] = pow((nu30 + nu12),2) + pow(nu21+nu03, 2);
	return;
}

/*static*/ void
BinaryImageMoments::ami1To4(const std::vector<Index> &objectIndices, std::vector<float_t> &momentsVec)
{
	// resize output vector:
	momentsVec.resize(4);

	if (objectIndices.empty()) {
		for (int i=0; i<momentsVec.size(); ++i) momentsVec[i] = 0;
		return;
	}

	// compute the needed central moments:
	const float_t mu00 = central00(objectIndices);
	const float_t mu20 = central(objectIndices, 2, 0);
	const float_t mu02 = central(objectIndices, 0, 2);
	const float_t mu11 = central(objectIndices, 1, 1);
	const float_t mu30 = central(objectIndices, 3, 0);
	const float_t mu03 = central(objectIndices, 0, 3);
	const float_t mu21 = central(objectIndices, 2, 1);
	const float_t mu12 = central(objectIndices, 1, 2);

	// compute the ami moments:
	float_t quotient = 0.0f;
	const int tempSize = 13;
	float_t t[tempSize]; // temporary array
	for (int i=0; i<tempSize; ++i) t[i] = 0.0f;

	// I1:
	quotient = 1.0f / pow(mu00,4);
	momentsVec[0] = mu20*mu02 - pow(mu11,2);
	momentsVec[0] *= quotient;
	// I2:
	quotient = 1.0f / pow(mu00,10);
	t[0] = pow(mu30, 2)*pow(mu03, 2);
	t[1] = -6*mu30*mu21*mu12*mu03;
	t[2] = 4*mu30*pow(mu12, 3);
	t[3] = 4*mu03*pow(mu21, 3);
	t[4] = -3*pow(mu21,2)*pow(mu12,2);
	momentsVec[1] = t[0] + t[1] + t[2] + t[3] + t[4];
	momentsVec[1] *= quotient;
	// I3:
	quotient = 1.0f / pow(mu00,7);
	t[0] = mu20*(mu21*mu03-pow(mu12,2));
	t[1] = -mu11*(mu30*mu03-mu21*mu12);
	t[2] = mu02*(mu30*mu12-pow(mu21,2));
	momentsVec[2] = t[0] + t[1] + t[2];
	momentsVec[2] *= quotient;
	// I4:
	quotient = 1.0f / pow(mu00,11);
	t[0] = pow(mu20,3)*pow(mu03,2);
	t[1] = -6*pow(mu20, 2)*mu11*mu12*mu03;
	t[2] = -6*pow(mu20, 2)*mu02*mu21*mu03;
	t[3] = 9*pow(mu20, 2)*mu02*pow(mu12,2);
	t[4] = 12*mu20*pow(mu11, 2)*mu21*mu03; //-------
	t[5] = 6*mu20*mu11*mu02*mu30*mu03;
	t[6] = -18*mu20*mu11*mu02*mu21*mu12;
	t[7] = -8*pow(mu11,3)*mu30*mu03;
	t[8] = -6*mu20*pow(mu02,2)*mu30*mu12; // -------
	t[9] = 9*mu20*pow(mu02,2)*pow(mu21,2);
	t[10] = 12*pow(mu11,2)*mu02*mu30*mu12;
	t[11] = -6*mu11*pow(mu02,2)*mu30*mu21;
	t[12] = pow(mu02,3)*pow(mu30,2); // ------------
	// sum up all terms and divide by quotient:
	momentsVec[3] = 0.0f;
	for (int i=0; i<13; ++i) momentsVec[3] += t[i];
	momentsVec[3] *= quotient;

	return;
}

} // end of namespace impact_tr4
