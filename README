################################################################################
# 
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################


README file for the Inventory-Extraction-Tool Prototype v0.2
-----------------------------------------------------------------------------------------

The library uses CMake as makefile generator. To compile the tool, download and 
install CMake (a cross-platform, open-source build system) from www.cmake.org.
A CMake version >= 2.4 is needed.

The program has been successfully compiled on Windows using Visual Studio 2008
and Ubuntu-Linux with gcc version 4.4.5. However, it should be easily compilable on other 
platforms and / or compilers too, since only platform independent libraries have been used.
Note that compilation was also successfull for Windows using MinGW with gcc version 4.4.1
but the OpenCV library seems to be broken using this compiler and thus this configuration is 
not recommended!
Compilation on an Ubuntu Linux system however has proven to be the most unproblematic configuration, 
since all libraries needed can be obtained using the apt-tool and are found by the CMake configuration 
file automatically in most cases.

The following libraries are required to compile the project:

	- OpenCV, version >= 2.2.0 (opencv.willowgarage.com)
	- Boost, version >= 1.40 (www.boost.org)
	- Qt, version >= 4.6 (qt.nokia.com)

Additionally OpenMP is used for multicore-parallelization. Using OpenMP can be turned off using CMake by
setting the cached variable "USE_OPENMP" to off (using either ccmake or cmake-gui).
	
The main CMake configuration file is located at 'bin/CMakeLists.txt'. Edit this file, to configure specific  
library paths etc. 
On Unix systems change the CMAKE_BUILD_TYPE variable to 'Debug' or 'Release' to switch
between build specific compiler flags. 
On Windows, debug and release compiler flags are handled by Visual Studio itself so the content of this
variable has no effect.

To compile the tool using cmake, just step into the main directory and type:
 > cmake .
to generate build files using standard configuration values or type
 > cmake-gui .
to start the GUI interface of CMake. There you can edit specific variables to customize your 
configuration. Then press the 'Configure' button first to configure the project and press 'Generate' 
afterwards to create build files. 

After the build files have been generated, type
 > make inventory_extraction_tool
to compile the program.

The resulting executable 'inventory_extraction_tool' is located in the 'bin' folder. Note, that
CMake also supports to generate Eclipse project files.

On Windows systems one can use the GUI version of cmake (cmake-gui.exe) to generate solution files
for Visual Studio. Just open the solution afterwards and compile target 'inventory_extraction_tool' 
using either the 'Release' of 'Debug' configuration.



















 