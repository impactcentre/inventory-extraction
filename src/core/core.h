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
#ifndef CORE_HPP_
#define CORE_HPP_

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif

// STD includes
#include <iostream>
#include <ostream>
#include <fstream>

// Boost includes
#include <boost/numeric/ublas/vector.hpp>
// Project includes
#include "float_types.hpp"

//namespace ublas = boost::numeric::ublas;

typedef unsigned char uchar;
typedef unsigned char uint8;
typedef char int8;
typedef unsigned short int uint16;
typedef short int int16;
typedef int int32;

/*! The namespace for the impact project */
namespace impact_tr4 {

}

namespace impact_tr4 {
	/*! Abbreviation for the ublas module of boost */
	namespace ublas = boost::numeric::ublas;
//#ifdef DEBUG
//	static std::ofstream my_out;
////	my_out.rdbuf(my_out.rdbuf());
////	(std::cout).rdbuf(&my_out);
//#endif
//	static std::ofstream logout("logfile.txt");

	// Define the size type to be the one from std
	//typedef std::size_t size_t;

//	typedef unsigned char uchar;
//	typedef unsigned char uint8;
//	typedef char int8;
//	typedef unsigned short int uint16;
//	typedef short int int16;
//	typedef int int32;
}



#endif /*CORE_HPP_*/
