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
#ifndef STOPWATCH_HPP_
#define STOPWATCH_HPP_

/**
 * @file StopWatch.hpp
 * 
 **/

// INCLUDEFILES:
//	Standard Library:
#include <iostream>

#include "platform.h"
#include "util.h"

#if defined(LINUX_OS)
#	include <sys/time.h>
#	include <sys/times.h>
#elif defined(WINDOWS_OS)
#	include <windows.h>
#else
	PLATFORM_ERROR;
#endif

namespace impact_tr4  {

// 	struct timeval;
	typedef struct timeval time;

	class StopWatch;

	/** A Stopwatch with millisecond accuracy.
	 *  Use the start/stop functions to measure time.
	*/
	class StopWatch
	{
	// VARIABLES:
	private:
		#if defined(LINUX_OS)
			time _t_start, _t_end;
			double _total_time;
		#elif defined(WINDOWS_OS)
			DWORD _t_start, _t_end;
			double _total_time;
		#else
			PLATFORM_ERROR;
		#endif
	// METHODS:
	public:
		/** Constructor. Automatically starts the StopWatch. */
		StopWatch() { reset(); start(); }

// 		~StopWatch() {}

		/** Resets the StopWatch to zero. */
		void reset() { _total_time = 0; }

		/** Starts the StopWatch. */
		void start();
		/** Adds the time elapsed since the last call to start()
			to the total time and returns the total time. In addition
			the total time is printed to the command line if print_time is true. */
		double stop(const bool print_time=true);

		/** Returns the milliseconds passed since system start. */
		static long getMsTickCount()
		{
		#if defined(LINUX_OS)
		    tms tm;
		    return times(&tm);
		#elif defined(WINDOWS_OS)
			return GetTickCount();
		#else
			PLATFORM_ERROR;
		#endif
		} // end getMsTickCount
	}; // end class StopWatchMs


} // end of namespace impact_tr4

#endif /*STOPWATCH_HPP_*/
