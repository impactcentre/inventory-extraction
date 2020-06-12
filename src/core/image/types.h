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
#ifndef TYPES_H_
#define TYPES_H_

namespace impact_tr4 {

enum ThreshType {
	/** Normal binary thresholding. */
	THRESH_BINARY = CV_THRESH_BINARY,
	/** Inverted binary thresholding. */
	THRESH_BINARY_INV = CV_THRESH_BINARY_INV,
	/** Truncated thresholding - sets all values above threshold to specified value. */
	THRESH_TRUNC = CV_THRESH_TRUNC,
	/** Truncate to zero thresholding - sets all values above threshold to zero. */
	THRESH_TOZERO = CV_THRESH_TOZERO,
	/** Truncate to zero thresholding inverted. */
	THRESH_TOZERO_INV = CV_THRESH_TOZERO_INV
};

enum AdaptiveThreshType {

};

} // end of namespace impact_tr4

#endif /*TYPES_H_*/
