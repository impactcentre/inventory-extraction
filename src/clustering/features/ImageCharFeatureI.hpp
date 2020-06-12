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
#ifndef IMAGECHARFEATUREI_HPP_
#define IMAGECHARFEATUREI_HPP_

#include "../DataStructs.hpp"

namespace impact_tr4
{

class ImageCharFeatureI;

/** An interface for specific image characteristics.
 * The class basically consists of two pure virtual functions.
 * The function calculateOutputVector(...) calculates specific image characteristics
 * and appends the resulting output vector at the row and column index
 * of the given matrix. For the creation of the output matrix, the size of the
 * output vector for the image characteristic has also to be known, which is
 * accomplished by the vectorSize() function.
 * */
class ImageCharFeatureI
{
public:
	virtual ~ImageCharFeatureI() {}
	/** Returns the size of the output vector. Has to be implemented by subclasses. */
	virtual const int vectorSize() const = 0;
	/** */
	virtual void calculateOutputVector(const GrayImage<> &image, const int rowIndex, const int colIndex, ublas::matrix<float>& outMatrix) = 0;
};

} // end of namespace impact_tr4

#endif /*IMAGECHARFEATUREI_HPP_*/
