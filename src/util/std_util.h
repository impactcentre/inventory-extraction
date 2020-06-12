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
#ifndef STD_UTIL_H_
#define STD_UTIL_H_

#include <iostream>
#include <fstream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "core/core.h"
#include "core/Exception.hpp"

//using namespace boost::numeric::ublas;
using namespace impact_tr4;

/** Writes the specified std-vector to the textfile file with filename fn. */
template <class T>
void writeTxtFile(std::vector<T> &v, const std::string& fn);

template <class T>
void writeTxtFile(std::vector<T> &v, const std::string& fn)
{
	std::ofstream myfile(fn.c_str());
	for (int i=0; i<v.size(); ++i) {
		myfile << v[i] << " ";
	}
	myfile.close();
	return;
}

#endif /*STD_UTIL_H_*/
