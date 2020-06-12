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
#ifndef FINEREADERDATA_H_
#define FINEREADERDATA_H_

#include <iostream>

#include <windows.h>
#include <direct.h>

#include <QDir>
#include <QFile>

#include "Exception.hpp"
#include "FRCharIterator.hpp"
#include "image/GrayImage.hpp"
#include "image/BoundingBox.hpp"
#include "util/util.h"

namespace impact_tr4 {

// Forward references:
class BoundingBox;

class FRCharacter;
class FRLine;
class FRParagraph;
class FRTextBlock;
class FRData;
class FRCharIterator;

/**
 * Class for FineReader Character data.
 */
class FRCharacter
{
public:
	// CONSTRUCTORS
	FRCharacter(const int index) { mIndex = index; }

	// ACCESS
	const int index() const { return mIndex; }

	BoundingBox boundingBox() const { return mBBox; }
	void setBoundingBox(const BoundingBox& bBox) { mBBox = bBox; }
	void setBounds(const int left, const int top, const int right, const int bottom)
	{
		BoundingBox bBox(left, top, right-left, bottom-top);
		mBBox = bBox;
	}

	wchar_t character() const
		{ return (mVariants.empty()) ? 0 : mVariants[0].second; }

	int characterConfidence() const
		{ return (mVariants.empty()) ? -1 : mVariants[0].first; }

//	void setCharacter(const wchar_t character) { mCharacter = character; }

	void pushBackVariant(const std::pair<int, wchar_t> variant)
		{ mVariants.push_back(variant); }
	void popBackVariant() { mVariants.pop_back(); }
	void clearVariants() { mVariants.clear(); }
	const int nVariants() const { return mVariants.size(); }
	std::pair<int, wchar_t>& getVariant(const int i) { return mVariants[i]; }

	// VALIDATION

	// METHODS
	void print()
	{
		std::cout << "Character, x="<<mBBox.x()<<", y="<<mBBox.y()<<", width="<<mBBox.width()<<", height="<<mBBox.height()<<"\n";
		std::cout << "Variants (char/confidence):\n";
		for (int i=0; i<nVariants(); ++i) {
			std::wcout << (getVariant(i).second) << L"/" << getVariant(i).first << L"\n";
		}
		return;
	}

private:
	// MEMBERS:
	BoundingBox mBBox;
//	wchar_t mCharacter;
	std::vector<std::pair<int, wchar_t> > mVariants; // the list of character variants
	int mIndex;
}; // end of class FRCharacter

/**
 * Class for FineReader Line data.
 */
class FRLine
{
public:
	// CONSTRUCTORS
	FRLine(const int index) { mIndex = index; }

	// ACCESS
	const int index() const { return mIndex; }

	BoundingBox boundingBox() const { return mBBox; }
	void setBoundingBox(const BoundingBox& bBox) { mBBox = bBox; }
	void setBounds(const int left, const int top, const int right, const int bottom)
	{
		BoundingBox bBox(left, top, right-left, bottom-top);
		mBBox = bBox;
	}

	const std::wstring& text() const { return mText; }
	const int textSize() const { return mText.size(); }
	void setText(const std::wstring& text) { mText = text; }
	wchar_t character(const int pos) const { return mText[pos]; }
	std::wstring subString(const int pos, const int nChars)
		{ return mText.substr(pos, nChars); }

	void pushBackCharacter(const FRCharacter& character)
		{ mCharacters.push_back(character); }
	void popBackCharacter() { mCharacters.pop_back(); }
	void clearCharacters() { mCharacters.clear(); }
	const int nCharacters() const { return mCharacters.size(); }
	FRCharacter& getCharacter(const int i) { return mCharacters[i]; }

	// VALIDATION

	// METHODS
	void print()
	{
		std::cout << "Line, x="<<mBBox.x()<<", y="<<mBBox.y()<<", width="<<mBBox.width()<<", height="<<mBBox.height()<<"\n";
		_cwprintf(L"Text: %s\n", mText.c_str());
		std::cout << "Characters:\n";
		for (int i=0; i<nCharacters(); ++i) {
			std::cout << "Character " << i << ":\n";
			mCharacters[i].print();
		}
		return;
	}

private:
	// MEMBERS:
	BoundingBox mBBox;
	std::wstring mText;
	std::vector<FRCharacter> mCharacters;
	int mIndex;
}; // end of class FRLine

/**
 * Class for FineReader Paragraph data.
 */
class FRParagraph
{
public:
	// CONSTRUCTORS
	FRParagraph(const int index) { mIndex = index; }

	// ACCESS
	const int index() const { return mIndex; }

	BoundingBox boundingBox() const { return mBBox; }
	void setBoundingBox(const BoundingBox& bBox) { mBBox = bBox; }
	void setBounds(const int left, const int top, const int right, const int bottom)
	{
		BoundingBox bBox(left, top, right-left, bottom-top);
		mBBox = bBox;
	}

	const std::wstring& text() const { return mText; }
	void setText(const std::wstring& text) { mText = text; }
	wchar_t character(const int pos) const { return mText[pos]; }
	std::wstring subString(const int pos, const int nChars)
		{ return mText.substr(pos, nChars); }
	int nCharacters() const { return mText.size(); }

	int nLines() const { return mLines.size(); }
	void pushBackLine(const FRLine& line) { mLines.push_back(line); }
	void popBackLine() { mLines.pop_back(); }
	void clearLines() { mLines.clear(); }
	FRLine& getLine(const int i) { return mLines[i]; }

	// VALIDATION

	// METHODS
	void print()
	{
		std::cout << "Paragraph, x="<<mBBox.x()<<", y="<<mBBox.y()<<", width="<<mBBox.width()<<", height="<<mBBox.height()<<"\n";
		_cwprintf(L"Text: %s\n", mText.c_str());
		std::cout << "Lines:\n";
		for (int i=0; i<nLines(); ++i) {
			std::cout << "Line " << i << ":\n";
			mLines[i].print();
		}
		return;
	}


	/**
	 * Determines the line index, to which this paragraph character index
	 * belongs to.
	 * */
	int determineLineIndex(const int charIndex)
	{
		int count = 0, i = 0, nCharsInLine = 0;
		const int nLines = mLines.size();
		for (i=0; i<nLines; ++i) {
			nCharsInLine = mLines[i].textSize();
			if (charIndex >= count && charIndex < (count+nCharsInLine) ) {
				break;
			}
			count += nCharsInLine;
		} // end for all line i
		if (i==nLines)
			throw Exception("Could not determine line number of this character index!");

		return i;
	} // end of determineLine

private:
	// MEMBERS:
	BoundingBox mBBox;
	std::wstring mText;

	std::vector<FRLine> mLines;
	int mIndex;
}; // end of class FRParagraph

/**
 * Class for FineReader Text Block data.
 */
class FRTextBlock
{
public:
	// CONSTRUCTORS
	FRTextBlock(const int index) { mIndex = index; }

	// ACCESS
	const int index() const { return mIndex; }

	BoundingBox boundingBox() const { return mBBox; }
	void setBoundingBox(const BoundingBox& bBox) { mBBox = bBox; }
	void setBounds(const int left, const int top, const int right, const int bottom)
	{
		BoundingBox bBox(left, top, right-left, bottom-top);
		mBBox = bBox;
	}

	void pushBackParagraph(const FRParagraph& par) { mParagraphs.push_back(par); }
	void popBackParagraph() { mParagraphs.pop_back(); }
	void clearParagraphs() { mParagraphs.clear(); }
	int nParagraphs() const { return mParagraphs.size(); }
	FRParagraph& getParagraph(const int i) { return mParagraphs[i]; }

	int nCharacters() const
	{
		int count = 0;
		for (int i=0; i<mParagraphs.size(); ++i) {
			count += mParagraphs[i].nCharacters();
		}
		return count;
	}

	int nLines() const
	{
		int count = 0;
		for (int i=0; i<mParagraphs.size(); ++i) {
			count += mParagraphs[i].nLines();
		}
		return count;
	}

	// VALIDATION

	// METHODS
	void print()
	{
		std::cout << "TextBlock, x="<<mBBox.x()<<", y="<<mBBox.y()<<", width="<<mBBox.width()<<", height="<<mBBox.height()<<"\n";
		std::cout << "Paragraphs:\n";
		for (int i=0; i<nParagraphs(); ++i) {
			std::cout << "Paragraph " << i << ":\n";
			mParagraphs[i].print();
		}
		return;
	}

	// OPERATORS

private:
	// MEMBERS:
	BoundingBox mBBox;

	std::vector<FRParagraph> mParagraphs;
	int mIndex;
}; // end of class FRTextBlock

/**
 * Class for FineReader Data.
 */
class FRData
{
public:
	// CONSTRUCTORS
	FRData() { }

	// ACCESS
	void pushBackTextBlock(const FRTextBlock& textBlock)
		{ mTextBlocks.push_back(textBlock); }
	void popBackTextBlock() { mTextBlocks.pop_back(); }
	void clearTextBlocks() { mTextBlocks.clear(); }
	int nTextBlocks() const { return mTextBlocks.size(); }
	FRTextBlock& getTextBlock(const int i) { return mTextBlocks[i]; }

	int nCharacters() const
	{
		int count = 0;
		for (int i=0; i<mTextBlocks.size(); ++i) {
			count += mTextBlocks[i].nCharacters();
		}
		return count;
	}

	int nLines() const
	{
		int count = 0;
		for (int i=0; i<mTextBlocks.size(); ++i) {
			count += mTextBlocks[i].nLines();
		}
		return count;
	}

	// VALIDATION

	// METHODS
	void print()
	{
		std::cout << "Data\n\n";
		std::cout << "TextBlocks:\n";
		for (int i=0; i<nTextBlocks(); ++i) {
			std::cout << "TextBlock " << i << ":\n";
			mTextBlocks[i].print();
		}
		return;
	} // end print()
	template <class T>
	void exportImages(GrayImage<T>& referenceImage, const std::string baseDirName, const std::string suffix = ".png");

	/** Return iterator pointing to first character of data. */
	FRCharIterator begin();
	/** Returns iterator pointing to last characeter of data.
	 * Note that *end() is no member of the data! */
	FRCharIterator end();

	// OPERATORS

private:
	// MEMBERS:
	std::vector<FRTextBlock> mTextBlocks;
}; // end of class FRData

template <class T>
void FRData::exportImages(GrayImage<T>& referenceImage, const std::string baseDirName, const std::string suffix /*= ".png"*/)
{
    // Create the path for the base dir:
#if 1
	createPath(baseDirName); // create the base directory if not present

	std::string baseDirNameR = baseDirName;
	convertToRegularPath(baseDirNameR);
	std::string dirName = baseDirNameR;

	std::stringstream ss;
	std::string nr;
	std::cout << "nr of textblocks: " << this->nTextBlocks() << std::endl;
	bool success = false;
	for (int i=0; i< this->nTextBlocks(); ++i) {
		ss << i; ss >> nr; ss.clear();
		std::string tbDirN = baseDirNameR + "/textblock_" + nr;
		if (!isDir(tbDirN.c_str()))
		if (!CreateDirectory(tbDirN.c_str(), NULL)) {
			std::cout << "Something wrong here!" << std::endl;
			exit(1);
		}

		FRTextBlock textblock = this->getTextBlock(i);
		std::cout << textblock.boundingBox() << std::endl;
		BoundingBox bbox = textblock.boundingBox();

		bbox.saveImage( referenceImage, tbDirN + "/" + nr + suffix );
		std::cout << "nr of paragraphs: " << textblock.nParagraphs() << std::endl;
		for (int j=0; j<textblock.nParagraphs(); ++j) {
			ss << j; ss >> nr; ss.clear();
			std::string pDirN = tbDirN + "/paragraph_" + nr;
			if (!isDir(pDirN.c_str()))
			if (!CreateDirectory(pDirN.c_str(), NULL)) {
				std::cout << "Something wrong here!" << std::endl;
				exit(1);
			}

			FRParagraph paragraph = textblock.getParagraph(j);
//			mkdir(prefix+"paragraph_"+nr);
			paragraph.boundingBox().saveImage( referenceImage, pDirN+"/"+nr+suffix );
			std::cout << "nr of lines: " << paragraph.nLines() << std::endl;
			for (int k=0; k<paragraph.nLines(); ++k) {
				ss << k; ss >> nr; ss.clear();
				std::string lDirN = pDirN + "/line_" + nr;
				if (!isDir(lDirN.c_str()))
				if (!CreateDirectory(lDirN.c_str(), NULL)) {
					std::cout << "Something wrong here!" << std::endl;
					exit(1);
				}

				FRLine line = paragraph.getLine(k);
//				mkdir(prefix+"line_"+nr);
				line.boundingBox().saveImage( referenceImage, lDirN+"/"+nr+suffix );
				std::cout << "nr of characters: " << line.nCharacters() << std::endl;
				for (int l=0; l<line.nCharacters(); ++l) {
					ss << l; ss >> nr; ss.clear();
					std::string cFileN = lDirN + "/char_" + nr;
//					if (!CreateDirectory(cDirN.c_str(), NULL)) {
//						std::cout << "Something wrong here!" << std::endl;
//						exit(1);
//					}
//					std::cout << "nr = " << nr << std::endl;
					FRCharacter character = line.getCharacter(l);
					character.boundingBox().saveImage( referenceImage, cFileN+suffix );
				}
			}
		}

	}
#endif
	return;
} // end exportImages

} // end of namespace impact_tr4

#endif /*FINEREADERDATASTRUCTS_H_*/
