#include <map>
#include <algorithm>
#include <iterator>

#include "allocator.h"
#include "utils.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(tests)

BOOST_AUTO_TEST_CASE(factorial_function)
{
  BOOST_STATIC_ASSERT(1 == factorial(0));
  BOOST_STATIC_ASSERT(1 == factorial(1));
  BOOST_STATIC_ASSERT(2 == factorial(2));
  BOOST_STATIC_ASSERT(6 == factorial(3));
  BOOST_STATIC_ASSERT(24 == factorial(4));
  BOOST_STATIC_ASSERT(120 == factorial(5));
  BOOST_STATIC_ASSERT(720 == factorial(6));
  BOOST_STATIC_ASSERT(5040 == factorial(7));
  BOOST_STATIC_ASSERT(40320 == factorial(8));
  BOOST_STATIC_ASSERT(362880 == factorial(9));

  BOOST_CHECK_THROW(factorial(-1), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(map_custom_allocator)
{
	auto make_factorial_value = [i = 0] () mutable
	{
		auto f = factorial(i);
		auto value = std::make_pair(i,f);
		++i;
		return value;
	};
	
	std::map<int, int, std::less<int>, Allocator<std::pair<const int, int>, 10>> map;

	BOOST_CHECK_NO_THROW(
						std::generate_n( std::inserter(map, std::begin(map))
									, 10
									, make_factorial_value
									)
						);
}

BOOST_AUTO_TEST_CASE(map_custom_allocator_exceeded_size)
{
	auto make_factorial_value = [i = 0] () mutable
	{
		auto f = factorial(i);
		auto value = std::make_pair(i,f);
		++i;
		return value;
	};
	
	const int size = 10;
	const int sizeExceeded = size + 1;
	
	std::map<int, int, std::less<int>, Allocator<std::pair<const int, int>, size>> map;

	BOOST_CHECK_THROW(
						std::generate_n( std::inserter(map, std::begin(map))
										, sizeExceeded
										, make_factorial_value
										)
						, std::bad_alloc()
						);
}

BOOST_AUTO_TEST_SUITE_END()