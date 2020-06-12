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
#ifndef FINEREADEROPERATIONS_HPP_
#define FINEREADEROPERATIONS_HPP_

#include "FineReaderData.hpp"

#include "external/finereader/Tools/AbbyyException.h"
#include "external/finereader/Tools/BstrWrap.h"
#include "external/finereader/Tools/SafePtr.h"
#include "external/finereader/FREngineLoader.h"

#include "FREngine.h"

#define _UNICODE

namespace impact_tr4 {

class FRData;

class FineReaderOperations
{
public:
	/** Loads the FineReader engine.
	 * This is necessary for all subsequent operations of this class.
	 * */
	static void loadEngine()
	{
		try {
			LoadFREngine(); // load the fr engine
			std::cout << "FineReader engine loaded!" << std::endl;
		}
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
	} // end loadEngine()

	/** Unloads the FineReader Engine. */
	static void unloadEngine()
	{
		try {
			// unload the fr engine, note that it is important,
			// that all finereader-engine related pointers have to be released beforehand,
			// elsewise a CAbbyyException is thrown!
			UnloadFREngine();
			std::cout << "FineReader engine unloaded!" << std::endl;
		}
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
	} // end unloadEngine()

	/** Parses the image with the given filename using the FineReader
	 * Engine and stores the data into the given FRData object.
	 * @param [in] fileName The filename for the image to parse.
	 * @param [out] data The FRData object to store the results of the parsing process.
	 * */
	static int parseImage(const std::string fileName, FRData& frdata);

private:
	static void parseText(IImageDocument* pImageDoc, ILayout* pLayout, FRData& frdata, const bool convertCoordinates=true);

}; // end class FineReaderOperations

} // end of namespace impact_tr4

#endif /*FINEREADEROPERATIONS_HPP_*/
