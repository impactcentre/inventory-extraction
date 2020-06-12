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
#include "util.h"
#include "clustering/CharCluster.hpp"
#include "clustering/ClusteringResult.hpp"
#include "util/ColorMap.hpp"

#include <iostream>
#include <fstream>

namespace impact_tr4 {

void writeLibSVMOutputFile(const std::vector<unsigned int> &labels, const ublas::matrix<float> &featureMat, const std::string filename)
{
	if (labels.size() != featureMat.size1()) {
		throw Exception("Nr of labels must be the same as number of features in data matrix!");
	}

	std::ofstream outfile(filename.c_str());
	for (int i=0; i<featureMat.size1(); ++i) { // for all rows of matrix
		outfile << labels[i];
		for (int j=0; j<featureMat.size2(); ++j) {
			outfile << " " << j+1 << ":" << featureMat(i,j);
		}
		outfile << "\n";
	}

	return;
}

std::string num2Str(double number)
{
	std::stringstream ss;
	std::string str;
	ss << number;
	ss >> str;
	return str;
}

int replaceAllChars(std::string& str, const char c1, const char c2)
{
	int count = 0;
	for (int i=0; i<str.size(); ++i) {
		if (str[i] == c1) {
			str[i] = c2;
			++count;
		}
	}
	return count;
} // end replace_chars

int replaceAllCharsW(std::wstring& str, const wchar_t c1, const wchar_t c2)
{
	int count = 0;
	for (int i=0; i<str.size(); ++i) {
		if (str[i] == c1) {
			str[i] = c2;
			++count;
		}
	}
	return count;
} // end replace_chars_w

int createPath(const std::string& path) {
	int count = 0;
	std::string pathT = path;
	std::string subStr;
	convertToRegularPath(pathT);
//	std::cout << pathT << std::endl;

    subStr = "";
	int fpos = -1;
	bool firstFlag = true;
	do {
		fpos = pathT.find('/', 0);
		if (fpos != -1) {
			subStr.append( pathT.substr(0, fpos+1) );
			pathT = pathT.substr(fpos+1, pathT.size()-fpos-1);
		}
		else {
			subStr.append(pathT);
		}
//		std::cout << subStr << ", " << pathT << std::endl;
		if (firstFlag && subStr[subStr.size()-2]==':') {
			;
		}
		else if (!subStr.empty()) {
			if (!isDir(subStr)) { // if dir not already there -> create!
#if defined(_WIN32) || defined(WIN32)
				BOOL success = CreateDirectory(subStr.c_str(), NULL);
				if (success)
					count++;
				else {
					// do error handling here...
					throw Exception("There was an error during path creation in createPath(...)!");
				}
#else // put unix code for dir-creation here
				// TODO
				std::cout << "DIRECTORY CREATION NOT DONE IN UNIX YET!!" << std::endl;
				throw Exception("DIRECTORY CREATION NOT DONE IN UNIX YET!!");
#endif
			}
		}
		firstFlag = false;
	} while (fpos != -1);
	return count;
} // end of createPath

std::string pathName(const std::string& input)
{
	std::string tmp = input;
	std::string res;
	convertToRegularPath(tmp);
	int index1 = tmp.find_last_of("/");
	if (index1 < tmp.size()) {
		res = tmp.substr(0, index1+1);
	}
	return res;
}

std::string baseFileName(const std::string& input)
{
	std::string tmp = input;
	std::string res;
	convertToRegularPath(tmp);
	int index1 = tmp.find_last_of("/");
	int index2 = tmp.find_last_of(".");
	if (index2 >= tmp.size()) index2 = tmp.size()-1;
	if (index1 < tmp.size()) {
		res = tmp.substr(index1+1, index2-index1-1);
	}
	return res;
}

std::string baseFileNameWithPath(const std::string& input)
{
	std::string tmp = input;
	std::string res;
	convertToRegularPath(tmp);
	int index = tmp.find_last_of(".");
	if (index >= tmp.size()) index = tmp.size()-1;
	res = tmp.substr(0, index);
	return res;
}

std::string& trim( std::string& str)
{
    // Trim Both leading and trailing spaces
    size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
    size_t endpos = str.find_last_not_of(" \t"); // Find the first character position from reverse af

    // if all spaces or empty return an empty string
    if(( std::string::npos == startpos ) || ( std::string::npos == endpos))
    {
        str = "";
    }
    else
        str = str.substr( startpos, endpos-startpos+1 );

    return  str;
}

void convertGrayscaleToJet(const QImage& srcGrayscaleImage, QImage &dstJetImage)
{
	if (!srcGrayscaleImage.isGrayscale()) {
		throw Exception("convertGrayscaleToJet: This function is only supported for grayscale images!");
		std::cerr << "convertGrayscaleToJet: This function is only supported for grayscale images!" << std::endl;
		return;
	}

	ColorMap jetMap;
	jetMap.createJetMap();
	QRgb rgbValue;
	int grayValue;

	const int width = srcGrayscaleImage.size().width();
	const int height = srcGrayscaleImage.size().height();

	dstJetImage = QImage(width, height, QImage::Format_RGB32);

	for (int i=0; i<width; ++i) {
		for (int j=0; j<height; ++j) {
			grayValue = qGray(srcGrayscaleImage.pixel(i,j));
			rgbValue = qRgb(jetMap.red(grayValue), jetMap.green(grayValue), jetMap.blue(grayValue));
			dstJetImage.setPixel(i,j,rgbValue);
		}
	} // end for i

	return;
} // end convertGrayscaleToJet



} // end of namespace impact_tr4
