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
#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PLATFORM_ERROR printf("Platform could not be verified\n"); exit(-1);

// defines for windows platform:
#if ( defined(WINDOWS) || defined(_WIN32) || defined(WIN32) )

// tell windows, that it should not define the macros min/max: (our project has it's own min/max functions!)
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#undef min
#undef max
// include windows.h, needed for windows-native types etc.:
#include <Windows.h>
#include <direct.h>
// #include <winbase.h> // for Sleep function?
#define WINDOWS_OS
// set platform name to windows:
#ifndef PLATFORM_NAME
#define PLATFORM_NAME "Windows"

//typedef unsigned int uint;

#endif

#endif // end of defines for windows platform

// defines for linux platform:
#if defined(linux) || defined(__linux__)

#define LINUX_OS
// set platform name to linux:
#ifndef PLATFORM_NAME
#define PLATFORM_NAME "Linux"
#endif

#endif // end of defines for linux platform

#ifdef __cplusplus
} // end extern "C"
#endif

#endif /*PLATFORM_H_*/
