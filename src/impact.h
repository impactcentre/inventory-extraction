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
#ifndef IMPACT_H_
#define IMPACT_H_

/**
 * @mainpage Impact - WP Text Recognition, Subproject 4
 * 
 * @section introduction Introduction
 * This class library was written for subproject for text recognition of the IMPACT project.
 * It aims to provide ... 
 * 
 * @section coding_style Coding Style
 * 
 * The coding style used inside this project may not always be consistent but in general we adhere from 
 * the rules proposed on the following webpage: http://geosoft.no/development/cppstyle.html
 * 
 * @section dependencies Dependencies
 * 
 * The library is based upon the following libraries:
 * 
 * - Qt 4: Graphical user interface, XML module.
 * - OpenCV: Used for image processing.
 * - Boost: UBLAS module for basic linear algebra.
 * 
 * @section modules Modules
 * @subsection core_module Core Module
 * 
 * The core module provides core functionality such as the definition of custom exceptions,
 * types and fixed size vector classes (up to now: 2D-, 3D-vectors). Furthermore, the
 * subfolder image contains classes for image manipulation using Intels OpenCV library. 
 * 
 * @subsection gui_module GUI Module
 * 
 * The GUI module is intended to provide some basic graphical user interfaces to test
 * the output of this library. In a later stage of the development a more sophisticated
 * GUI, that covers the whole functionality, is intended to be created. All GUI's are
 * created using Qt 4 and the highgui module of Intels OpenCV library. 
 * 
 * @subsection xml_module XML Module
 * 
 * In the XML module, classes for the manipulation of XML files are to be created, 
 * mainly to read XML-output files from Abbyy's FineReader and write custom XML-output 
 * files.
 * For now, all classes are based upon the XML module of Qt 4. In a later development stage,
 * a change to the Xerces-C++ (see http://xerces.apache.org/xerces-c/) library 
 * may be considered.
 */

#endif /*IMPACT_H_*/
