/*
 * RandomNumber.cpp
 *
 *  Created on: 30.09.2010
 *      
 */

#include "RandomNumber.hpp"

namespace impact_tr4 {

	/*static*/ boost::mt19937 RandomNumber::random_generator;

	/*static*/ int RandomNumber::random_integer(const int from/*=1*/, const int to/*=100*/)
	{
		boost::uniform_int<> dist(from, to);
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen(RandomNumber::random_generator, dist);
		return gen();
	}

	/*static*/ std::vector<int> RandomNumber::random_unique_integer_tuple(const int n)
	{
		std::vector<int> flags(n, 0);
		std::vector<int> tuple;

		for (int i=0; i<n; ++i) {
		while (true) {
			int rand_int = RandomNumber::random_integer(0, n-1);
			if (flags[rand_int]==0) {
//			 	 std::cout << "valid!" << std::endl;
				flags[rand_int]=1;
				tuple.push_back(rand_int);
				break;
		 		}
			} // end while
		} // end for i

		return tuple;
	}



}

