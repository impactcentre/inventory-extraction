/*
 * RandomNumber.hpp
 *
 *  Created on: 30.09.2010
 *      
 */

#ifndef RANDOMNUMBER_HPP_
#define RANDOMNUMBER_HPP_

#include <vector>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

namespace impact_tr4 {

class RandomNumber {
public:
	static int random_integer(const int from=1, const int to=100);
	static std::vector<int> random_unique_integer_tuple(const int n);

private:
	static boost::mt19937 random_generator;

}; // end class RandomNumber


} // end of namespace impact_tr4


#endif /* RANDOMNUMBER_HPP_ */
