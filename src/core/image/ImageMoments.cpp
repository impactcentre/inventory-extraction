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
#include "ImageMoments.hpp"

namespace impact_tr4
{


/*static*/ void
ImageMoments::basicMoments(const Moments& moments, BasicMoments &basicMoments)
{
	const double m20_m02 = moments.mu20-moments.mu02;
	if (m20_m02==0) {
		basicMoments.orientation = 0;
		basicMoments.eccentricity = 0;
		return;
	}
	const double m20_m02_2 = pow(m20_m02, 2);

	basicMoments.orientation = 0.5f*atan((2.0f*moments.mu11)/m20_m02);
	basicMoments.eccentricity = m20_m02_2 + 4.0f*pow(moments.mu11, 2);
	basicMoments.eccentricity /= m20_m02_2;

	return;
}

/*static*/ void
ImageMoments::amiMoments(const Moments& moments, AmiMoments &amiMoments)
{
	// store central moments up to order 3:
	const float_t mu00 = moments.m00;
	if (mu00 == 0) { // if mu00 is zero, set all ami moments to zero
		double *pAmiMoment = &amiMoments.ami1;
		for (int i=0; i<amiMoments.size; ++i) {
			*pAmiMoment = 0; ++pAmiMoment;
		}
		return;
	}
	const float_t mu20 = moments.mu20;
	const float_t mu02 = moments.mu02;
	const float_t mu11 = moments.mu11;
	const float_t mu30 = moments.mu30;
	const float_t mu03 = moments.mu03;
	const float_t mu21 = moments.mu21;
	const float_t mu12 = moments.mu12;

	// compute the ami moments:
	float_t quotient = 0.0f;
	const int tempSize = 13;
	float_t t[tempSize]; // temporary array
	for (int i=0; i<tempSize; ++i) t[i] = 0.0f;

	// I1:
	quotient = 1.0f / pow(mu00,4);
	amiMoments.ami1 = mu20*mu02 - pow(mu11,2);
	amiMoments.ami1 *= quotient;
	// I2:
	quotient = 1.0f / pow(mu00,10);
	t[0] = pow(mu30, 2)*pow(mu03, 2);
	t[1] = -6*mu30*mu21*mu12*mu03;
	t[2] = 4*mu30*pow(mu12, 3);
	t[3] = 4*mu03*pow(mu21, 3);
	t[4] = -3*pow(mu21,2)*pow(mu12,2);
	amiMoments.ami2 = t[0] + t[1] + t[2] + t[3] + t[4];
	amiMoments.ami2 *= quotient;
	// I3:
	quotient = 1.0f / pow(mu00,7);
	t[0] = mu20*(mu21*mu03-pow(mu12,2));
	t[1] = -mu11*(mu30*mu03-mu21*mu12);
	t[2] = mu02*(mu30*mu12-pow(mu21,2));
	amiMoments.ami3 = t[0] + t[1] + t[2];
	amiMoments.ami3 *= quotient;
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
	amiMoments.ami4 = 0.0f;
	for (int i=0; i<13; ++i) amiMoments.ami4 += t[i];
	amiMoments.ami4 *= quotient;

	return;

} // end amiMoments

} // end of namespace impact_tr4
