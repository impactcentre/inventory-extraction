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
#include "StopWatch.hpp"

namespace impact_tr4  {

#if defined(LINUX_OS)
	void StopWatch::start(  )
	{
		// gettimeofday measures time in microseconds!
		gettimeofday (&_t_start, 0x0);
	}

	double StopWatch::stop( const bool print_time)
	{
		gettimeofday (&_t_end, 0x0);
		_total_time = (_t_end.tv_sec - _t_start.tv_sec) + (_t_end.tv_usec - _t_start.tv_usec) / 1000000.0;
		if (print_time)
			std::cout << "Elapsed time in seconds: " << _total_time << std::endl;

		return _total_time;
	}
#elif defined(WINDOWS_OS)
	void StopWatch::start(  )
	{
		_t_start = GetTickCount(); // GetTickCount returns milliseconds!
	}

	double StopWatch::stop( const bool print_time)
	{
		_t_end = GetTickCount();
		_total_time = double(_t_end - _t_start) / double(1000.0);
//		std::cout << _t_end << " " << _t_start << std::endl;
//		std::cout << "stopwatch ms: " << double(_t_end - _t_start) << std::endl;
		if (print_time)
			std::cout << "Elapsed time in seconds: " << _total_time << std::endl;

		return _total_time;
	}
#else
	PLATFORM_ERROR;
#endif

} // end of namespace impact_tr4


