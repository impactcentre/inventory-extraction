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
#ifndef UBLAS_UTIL_H_
#define UBLAS_UTIL_H_

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

/** Removes the row from the matrix at index r. */
template <class T>
void removeMatrixRow(ublas::matrix<T> &m, const int r);

/** Removes the column from the matrix at index c. */
template <class T>
void removeMatrixColumn(ublas::matrix<T> &m, const int c);

/** Removes the rc'th row and column from the specified symmetric matrix.
 * @todo FIXME: resizing of symmetric boost matrices preserving values seems not
 * to work, thus this function is buggy!
 * */
template <class T>
void removeSymmMatrixRowAndColumn(ublas::symmetric_matrix<T>& m, const int rc);

/** Writes the specified ublas-matrix to the textfile file with filename fn. */
template <class T>
void writeTxtFile(ublas::matrix<T> &m, const std::string& fn);

/** Writes the specified ublas-vector to the textfile file with filename fn. */
template <class T>
void writeTxtFile(ublas::vector<T> &v, const std::string& fn);

// TEMPLATE IMPLEMENTATIONS:

template <class T>
void writeTxtFile(ublas::matrix<T> &m, const std::string& fn)
{
	std::ofstream myfile(fn.c_str());
	for (int i=0; i<m.size1(); ++i) {
		for (int j=0; j<m.size2(); ++j) {
			myfile << m(i,j) << " ";
		}
		myfile << "\n";
	}
//	myfile << m; // write as ublas matrix (cannot be read by matlab...)
	myfile.close();
	return;
}

template <class T>
void writeTxtFile(ublas::vector<T> &v, const std::string& fn)
{
	std::ofstream myfile(fn.c_str());
	for (int i=0; i<v.size(); ++i) {
		myfile << v[i] << " ";
	}
//	myfile << v; // write as ublas vector (cannot be read by matlab...)
	myfile.close();
	return;
}

template <class T>
void removeMatrixRow(ublas::matrix<T> &m, const int r)
{
	Assert(!ARG_CHECK || r < m.size1(), IndexOutOfBoundsException("Incorrect row index in removeMatrixRow!"));

	for (int i=r+1; i<m.size1(); ++i) {
		for (int j=0; j<m.size2(); ++j) {
			m(i-1,j) = m(i,j);
		}
	}
	// resize matrix preserving existing values:
	m.resize(m.size1()-1, m.size2(), true);

	return;
}

template <class T>
void removeMatrixColumn(ublas::matrix<T> &m, const int c)
{
	Assert(!ARG_CHECK || c < m.size2(), IndexOutOfBoundsException("Incorrect column index in removeMatrixRow!"));

	for (int i=0; i<m.size1(); ++i) {
		for (int j=c+1; j<m.size2(); ++j) {
			m(i,j-1) = m(i,j);
		}
	}
	// resize matrix preserving existing values:
	m.resize(m.size1(), m.size2()-1, true);

	return;
}

template <class T>
void removeSymmMatrixRowAndColumn(ublas::symmetric_matrix<T, ublas::lower>& m, const int rc)
{
//	Assert(!ARG_CHECK || rc < m.size1(), IndexOutOfBoundsException("Incorrect column index in removeMatrixRow!"));

	// remove row:
	for (int i=rc+1; i<m.size1(); ++i) {
		for (int j=0; j<=i; ++j) {
			m(i-1,j) = m(i,j);
		}
	}
	// remove column:
	for (int i=0; i<m.size1(); ++i) {
		for (int j=rc+1; j<=i; ++j) {
			m(i,j-1) = m(i,j);
		}
	}
	// resize matrix preserving existing values:
	const int oldsize = m.size1();
	std::cout << m.size1() << ", " << m.size2() << std::endl;
	m.resize(oldsize-1, true);
	std::cout << m.size1() << ", " << m.size2() << std::endl;
	std::cout << m(0, 3) << std::endl;
	std::cout << m << std::endl;

	return;
}

#endif /*UBLAS_UTIL_H_*/
