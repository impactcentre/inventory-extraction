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
#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#if defined(_WIN32) || defined(WIN32)
	#include <direct.h>
	#include <windows.h>
#else
	#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include <QImage>

#include "core/core.h"
#include "core/Exception.hpp"
#include "util/RandomNumber.hpp"

//#include "core_headers.h"
//#include "image_headers.h"
//#include "clustering/Cluster.hpp"

namespace impact_tr4 {

//template <class T> class GrayImage;
class CharCluster;
class ClusteringResult;

inline double round(double x) { return floor(x + 0.5); }

/// Permutes the input matrix randomly rowwise, use non-empty random_tuple entry to use this permutation instead of new random one!
template <class T>
void permuteBoostMatrixRandomlyRowwise(const ublas::matrix<T> &mat, ublas::matrix<T> &permutedMatrix, std::vector<int> &random_tuple)
{
	permutedMatrix.resize(mat.size1(), mat.size2());
	// only create new random permutation if input was empty!
	if (random_tuple.empty())
		random_tuple = RandomNumber::random_unique_integer_tuple(mat.size1());

	for (int i=0; i<mat.size1(); ++i) {
//		std::cout << "random_tuple[i] = " << random_tuple[i] << std::endl;
		for (int j=0; j<mat.size2(); ++j) {
			permutedMatrix(i, j) = mat(random_tuple[i],j);
		}
	}
	return;
}

/// Permutes the input vector randomly columnwise, use non-empty random_tuple entry to use this permutation instead of new random one!
template <class T>
void permuteBoostVectorRandomlyRowwise(const ublas::vector<T> &vec, ublas::vector<T> &permutedVector, std::vector<int> &random_tuple)
{
	permutedVector.resize(vec.size());
	// only create new random permutation if input was empty!
	if (random_tuple.empty())
		random_tuple = RandomNumber::random_unique_integer_tuple(vec.size());
	for (int i=0; i<vec.size(); ++i) {
//		std::cout << "random_tuple[i] = " << random_tuple[i] << std::endl;
		permutedVector(i) = vec(random_tuple[i]);
	}
	return;
}

template <class T>
void printVector(const std::vector<T> &vec)
{
	for (int i=0; i<vec.size(); ++i) {
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
	return;
}

template <class T>
const T sumVec(const ublas::vector<T>& vec)
{
	T sum = (T)(0);
	for (int i=0; i<vec.size(); ++i)
		sum += vec[i];
	return sum;
}

template <class T>
const T sumMat(const ublas::matrix<T>& mat)
{
	T sum = (T)(0);
	for (int i=0; i<mat.size1(); ++i)
		for (int j=0; j<mat.size2(); ++j)
		sum += mat(i,j);
	return sum;
}


std::string num2Str(double number);

/**
 * Replaces all occurences of character c1 in str with character c2.
 * @param [in,out] The input string
 * @param [in] The character to replace
 * @param [in] The character to fill in.
 * @return The number of replacements that have been made.
 */
int replaceAllChars(std::string& str, const char c1, const char c2);

/**
 * Same functionality as replaceAllChars but for wide-strings.
 * */
int replaceAllCharsW(std::wstring& str, const wchar_t c1, const wchar_t c2);

/** Returns 1, if the file with given filename exists, 0 otherwise. */
inline int fileExists( const std::string& fileName ) {
	struct stat buffer ;
	if ( stat( fileName.c_str(), &buffer ) ) return 0;
	return 1;
}

/**
 * 'Regularizes' the given path, i.e. replaces all backslashes with slashes
 * and removes trailing slashes.
 * @param[in,out] path The path to regularize.
 * */
inline void convertToRegularPath(std::string& path)
{
	// 1st: replace all backslashes of path with slashes
	replaceAllChars(path, '\\', '/');
	// 2nd: remove possibly trailing '/':
	if (path[path.size()-1] == '/')
		path = path.substr(0, path.size()-1);

	return;
} // end makeRegularPath

/** Extracts the path of a filename. E.g.: path(c:/workspace/files/filename.txt) = c:/workspace/files/ */
std::string pathName(const std::string& input);
/** Extracts the base filename out of a given path (without extension)
 * E.g.: baseFileName(c:/workspace/files/filename.txt) = filename */
std::string baseFileName(const std::string& input);

/** Extracts the base filename (i.e. filename without extension) including the path. */
std::string baseFileNameWithPath(const std::string& input);

/** Removes leading and trailing whitespace from the string str. */
std::string& trim( std::string& str);

inline bool isDir(const std::string& path)
{
	std::string pathR = path;
	convertToRegularPath(pathR);

	struct stat statBuffer;
	return (stat(pathR.c_str(), &statBuffer) >= 0 && // make sure it exists
	statBuffer.st_mode & S_IFDIR); // and it's not a file
} // end isDir

/**
 * Create the given path including eventually non-existant sub-directories.
 * */
int createPath(const std::string& path) ;

void convertGrayscaleToJet(const QImage& srcGrayscaleImage, QImage &dstJetImage);

void writeLibSVMOutputFile(const std::vector<unsigned int> &labels, const ublas::matrix<float> &featureMat, const std::string filename);

} // end of namespace impact_tr4


#endif /*UTIL_H_*/
