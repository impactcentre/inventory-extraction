/*
 * DatFileDataReader.hpp
 *
 *  Created on: 04.02.2010
 *      
 */

#ifndef DATFILEDATAREADER_HPP_
#define DATFILEDATAREADER_HPP_

#include "CharDataReaderI.hpp"

namespace impact_tr4 {

class DatFileDataReader : public CharDataReaderI
{
public:
	DatFileDataReader();
	virtual ~DatFileDataReader() {}

//	void parseFiles(Document *pDocument, std::vector<ImageChar> &imageCharVec, std::vector<std::string> &charVec) const;
	void parseSingleImageFile(const std::string fileName, const std::string resultFileName, GrayImage<> *pImage, std::vector<ImageChar> &imageCharVec, int imageId = 0) const;
	const std::string fileExtension() const { return std::string("dat"); }

}; // end of class DatFileDataReader.hpp

} // end of namespace impact_tr4


#endif /* DATFILEDATAREADER_HPP_ */
