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
#include "FineReaderOperations.hpp"

namespace impact_tr4 {

/*static*/ void
FineReaderOperations::
parseText(IImageDocument* pImageDoc, ILayout* pLayout, FRData& frdata, const bool convertCoordinates/*=true*/)
{
	// get blocks:
	CSafePtr<IBlocksCollection> pBlocks = 0x0;
	CheckResult( pLayout->get_Blocks( &pBlocks ) );

	// get nr. of blocks:
	long blocksCount = 0;
	CheckResult(  pBlocks->get_Count( &blocksCount ) );

	int textBlockIndex = 0;
	// for all blocks
	for( int i = 0; i < blocksCount; i++ ) {
		// get current block:
		CSafePtr<IBlock> pBlock = 0x0;
		CheckResult( pBlocks->Item( i, &pBlock ) );
		// get block type:
		BlockTypeEnum blockType;
		CheckResult( pBlock->get_Type( &blockType ) );
		// only process textblocks:
		if (blockType == BT_Text) {
			// create FRTextBlock object with current textblock index:
			FRTextBlock frtextblock(textBlockIndex);
			++textBlockIndex;

			// get block region:
			CSafePtr<IRegion> pBlockRegion = 0x0;
			CheckResult( pBlock->get_Region(&pBlockRegion) );
			// get rectangle in region:
			long nrRects = -1;
			CheckResult( pBlockRegion->get_Count(&nrRects) );
			// get bounds of block region rectangle:
			long t=0, b=0, l=0, r=0;
			CheckResult( pBlockRegion->get_Top(0, &t) );
			CheckResult( pBlockRegion->get_Bottom(0, &b) );
			CheckResult( pBlockRegion->get_Left(0, &l) );
			CheckResult( pBlockRegion->get_Right(0, &r) );
			if (convertCoordinates) {
				pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &l, &t);
				pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &r, &b);
			}

//			std::cout << "l=" << l << ", t=" << t << ", r=" << r << ", b=" << b << std::endl;
//			std::cout << "-----------------------------------------------" << std::endl;
			// set bounds to frtextblock object:
			frtextblock.setBounds(l, t, r, b);

			// get textblock properties:
			CSafePtr<ITextBlockProperties> pTextBlockProperties = 0x0;
			CheckResult( pBlock->get_TextBlockProperties( &pTextBlockProperties ) );
			// get text:
			CSafePtr<IText> pText = 0x0;
			CheckResult( pTextBlockProperties->get_Text( &pText ) );
			// get paragraphs:
			CSafePtr<IParagraphs> pParagraphs = 0x0;
			CheckResult( pText->get_Paragraphs( &pParagraphs ) );
			// get nr. of paragraphs:
			long paragraphsCount;
			CheckResult( pParagraphs->get_Count( &paragraphsCount ) );
//			std::cout << "paragraphsCount = " << paragraphsCount << std::endl;

			// For all paragraphs j in block:
			for( long j = 0; j < paragraphsCount; j++ ) {
				// Create FRParagraph object with current paragraph index j:
				FRParagraph frparagraph(j);
				// get j'th paragraph:
				CSafePtr<IParagraph> pParagraph = 0x0;
				CheckResult( pParagraphs->Item( j, &pParagraph ) );
				// get text of paragraph:
				CBstr text;
				CheckResult( pParagraph->get_Text(&text) );
				// set text of paragraph to frparagraph object:
				std::wstring text_w(text);
				frparagraph.setText(text_w);
				// get bounds of paragraph:
				long t=-1, b=-1, l=-1, r=-1;
				CheckResult( pParagraph->get_Top(&t) );
				CheckResult( pParagraph->get_Bottom(&b) );
				CheckResult( pParagraph->get_Left(&l) );
				CheckResult( pParagraph->get_Right(&r) );
				if (convertCoordinates) {
					pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &l, &t);
					pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &r, &b);
				}
//				std::cout << "l=" << l << ", t=" << t << ", r=" << r << ", b=" << b << std::endl;
//				std::cout << "-----------------------------------------------" << std::endl;
				// set bounds of paragraph to frparagraph object:
				frparagraph.setBounds(l, t, r, b);

				// get lines of current paragraph:
				CSafePtr<IParagraphLines> pParLines = 0x0;
				CheckResult( pParagraph->get_Lines(&pParLines) );
				// get nr. of lines:
				long nLines=-1;
				CheckResult( pParLines->get_Count(&nLines) );
		//		std::cout << "lines: " << std::endl;

				// for all lines k in paragraph:
				for ( long k = 0; k < nLines; k++ ) {
					// create new FRLine object with current line index k:
					FRLine frline(k);
					// get k'th line:
					CSafePtr<IParagraphLine> pParLine = 0x0;
					CheckResult( pParLines->Item(k, &pParLine) );
					// get bounds of line:
					long l=-1,t=-1,r=-1,b=-1;
					CheckResult( pParLine->get_Left(&l) );
					CheckResult( pParLine->get_Top(&t) );
					CheckResult( pParLine->get_Right(&r) );
					CheckResult( pParLine->get_Bottom(&b) );
					if (convertCoordinates) {
						pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &l, &t);
						pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &r, &b);
					}
//					std::cout << "l=" << l << ", t=" << t << ", r=" << r << ", b=" << b << std::endl;
//					std::cout << "text of line " << m+1 << ":" << std::endl;
					// set bounds to frline object:
					frline.setBounds(l, t, r, b);

					// get text of line:
					std::wstring line_text;
					long firstCharIndex=-1, charactersCount=-1;
					CheckResult( pParLine->get_FirstCharIndex(&firstCharIndex) );
					CheckResult( pParLine->get_CharactersCount(&charactersCount) );
//					std::cout << firstCharIndex << ", " << charactersCount << std::endl;
					line_text = text_w.substr(firstCharIndex, charactersCount);
//					_cwprintf(L"-%s-\n", line_text.c_str());
					// set text to frline object:
					frline.setText(line_text);
					// add this line to the current frparagraph object:
					frparagraph.pushBackLine(frline);
				} // end for all lines m
//				std::cout << "-----------------------------------------------" << std::endl;

				// get paragraph length, i.e. the number of chars in it:
				long paragraphLength = -1;
				CheckResult( pParagraph->get_Length( &paragraphLength ) );

				// test outputs:
//				std::cout << "the number of lines is: " << nLines
//							<< " =(?) " << frparagraph.nLines() << std::endl;
//				std::cout << "the paragraph length is: " << paragraphLength
//						<< " =(?) " << frparagraph.nCharacters() << std::endl;

				// for all characters k in paragraph:
				for( long k = 0; k < paragraphLength; k++ ) {
					// create FRcharacter object with current char index k:
					FRCharacter frcharacter(k);

					// get character parameters:
					CSafePtr<ICharParams> pCharParams = 0x0;
					CheckResult( g_FREngine->CreateCharParams(&pCharParams) );
					CheckResult( pParagraph->GetCharParams( k, pCharParams ) );

					// get bounds of character:
					long t=-1, b=-1, l=-1, r=-1;
					CheckResult( pCharParams->get_Top(&t) );
					CheckResult( pCharParams->get_Bottom(&b) );
					CheckResult( pCharParams->get_Left(&l) );
					CheckResult( pCharParams->get_Right(&r) );
					if (convertCoordinates) {
						pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &l, &t);
						pImageDoc->ConvertCoordinates(IT_Deskewed, IT_Base, &r, &b);
					}
//					std::cout << "l=" << l << ", t=" << t << ", r=" << r << ", b=" << b << std::endl;
					// set bounds to frcharacter object:
					frcharacter.setBounds(l,t,r,b);

					// get variants: (i.e. possible characters):
					CSafePtr<ICharacterRecognitionVariants> pCharacterRecognitionsVariants = 0x0;
					CheckResult( pCharParams->GetCharacterRecognitionVariants(&pCharacterRecognitionsVariants) );

					// if there are variants:
					if (pCharacterRecognitionsVariants != 0x0) {
						// get nr. of variants:
						long nVariants = -1;
						CheckResult( pCharacterRecognitionsVariants->get_Count(&nVariants) );
						// for all variants v:
						for (int v=0; v<nVariants; ++v) {
							// get current variant and extract character and confidence information:
							CSafePtr<ICharacterRecognitionVariant> pCharacterRecognitionsVariant = 0x0;
							CheckResult( pCharacterRecognitionsVariants->Item(v, &pCharacterRecognitionsVariant) );
							CBstr character;
							CheckResult( pCharacterRecognitionsVariant->get_Character(&character) );
							long confidence = 0;
							CheckResult( pCharacterRecognitionsVariant->get_CharConfidence(&confidence) );
							// store confidence/character pair:
							std::pair<long, wchar_t> variant(confidence, character[0]);
							frcharacter.pushBackVariant(variant);

//							std::cout << character.Length() << std::endl;
//							std::wcout << L"Char variant " << v+1 << "/" << nVariants
//										<< L": " << variant.second
//										<< ", confidence: " << variant.first << std::endl;
						} // end for all variants v
					} // end if variants exist

#if 0
					// parse some extended attribs:
					// get extended attributes:
					CSafePtr<IExtendedRecAttributes> pERecAttributes = 0x0;
					CheckResult( pCharParams->get_ExtendedRecAttributes(&pERecAttributes) );

					long wordsCount=0, wordsFromDictionary=0;
					if( pERecAttributes != 0 ) {
						VARIANT_BOOL isWordStart, isWordNormal, isWordFromDic;
						pERecAttributes->get_IsWordStart( &isWordStart );
						pERecAttributes->get_IsWordNormal( &isWordNormal );
						pERecAttributes->get_IsWordFromDictionary( &isWordFromDic );

						if( isWordStart != VARIANT_FALSE ) {
							wordsCount++;
							if( isWordNormal != VARIANT_FALSE &&
									isWordFromDic != VARIANT_FALSE )
							{
							wordsFromDictionary++;
							}
						}
					}
//						std::cout << "extended attributes:" << std::endl;
//						std::cout << "word count = " << wordsCount << std::endl;
//						std::cout << "words from dic = " << wordsFromDictionary << std::endl;
#endif
					// determine the line, where this character belongs to:
					int lineIndex = frparagraph.determineLineIndex(k);
//					std::cout << "The line index is: " << lineIndex << std::endl;
					// add current character to this line:
					frparagraph.getLine(lineIndex).pushBackCharacter(frcharacter);
				} // end for paragraph length k
				// add current paragraph to textblock:
				frtextblock.pushBackParagraph(frparagraph);
			} // end for all paragraphs j
			frdata.pushBackTextBlock(frtextblock);
		} // end if this is a text block
	} // end for all blocks i

//	std::cout << "----------------------------------------------------------" << std::endl;
//	std::cout << "----------------------------------------------------------" << std::endl;

//	frdata.print();

	return;
} // end parseText()

/*static*/ int
FineReaderOperations::
parseImage(const std::string fileName, FRData& frdata)
{
	try {
		frdata.clearTextBlocks();

		// create the PrepareImageMode object (optional)
		CSafePtr<IPrepareImageMode> pImageMode = 0x0;
		CheckResult( g_FREngine->CreatePrepareImageMode(&pImageMode) );

		// create a user-implemented object of the ImagePasswordCallBack type (optional)
		IImagePasswordCallback* pImagePasswordCallback = 0x0;
		// Set up the prepare image mode
			// do sth.

		// open the image
		std::wstring fileNameW(fileName.length(),L' ');
		std::copy(fileName.begin(), fileName.end(), fileNameW.begin());
		CBstr imageFilePath(fileNameW.c_str());

		// create image doc:
		CSafePtr<IImageDocument> pImageDoc = 0x0;
		CheckResult( g_FREngine->PrepareAndOpenImage(imageFilePath,
										pImageMode,
										pImagePasswordCallback,
										0x0, // pass 0x0 if a file other than pdf is opened
										&pImageDoc) );

		// create a layout object
		CSafePtr<ILayout> pLayout = 0x0;
		CheckResult( g_FREngine->CreateLayout(&pLayout) );

		// create the page processing parameters object (optional)
		CSafePtr<IPageProcessingParams> pParams = 0x0;
		CheckResult( g_FREngine->CreatePageProcessingParams(&pParams) );
		// set up the page processing parameters
			// do sth.

		// call the AnalyzeAndRecoginzePage method of the Engine object AndRecognizePage
		CheckResult( g_FREngine->AnalyzeAndRecognizePage(pImageDoc,
											pParams,
											pLayout,
											0x0) ); // pass 0x0 if file other than pdf is opened


		FineReaderOperations::parseText(pImageDoc, pLayout, frdata); // parse image and store information in frdata
	} // end try
	catch (CAbbyyException ce) {
		std::wcout << "AbbyyException occurred in FineReaderOperations::parseImage(...) function:" << std::endl;
		std::wcout << ce.Description() << std::endl;
	}
	catch (Exception e) {
		std::wcout << "Exception occurred in FineReaderOperations::parseImage(...) function:" << std::endl;
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::wcout << "Unknown exception occurred in FineReaderOperations::parseImage(...) function:" << std::endl;
		exit(1);
	}

	// EXPORTING:
#if 0
	// create the xport parameters object (optional)
	IXMLExportParams* pExportParams = 0x0;
	g_FREngine->CreateXMLExportParams(&pExportParams);
	// set up the export parameters
	pExportParams->put_WriteCharAttributes( true ); // write char attribs to xml file
	pExportParams->put_WriteExtendedCharAttributes( true ); // write "extended" char attribs to xml file
	// call the ExportPage method of the Engine object
	CBstr outputFilePath = L"fr_test\\example_flexible_out.xml";
	g_FREngine->ExportPage(FEF_XML,
							outputFilePath,
							pImageDoc,
							pLayout,
							pExportParams);

	pExportParams->Release();
	pImageDoc->Release();
#endif

	return 0;
} // end of parseImage



} // end of namespace impact_tr4
