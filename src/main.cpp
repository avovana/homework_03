#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

#include "allocator.h"
#include "forward_list.h"

template<typename T>
constexpr auto factorial(T n) -> decltype(n)
{
    return n ? n * factorial(n - 1) : 1;
}

static_assert(factorial(9) == 9*8*7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(8) ==   8*7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(7) ==     7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(6) ==       6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(5) ==         5*4*3*2*1, "factorial failed!");
static_assert(factorial(4) ==           4*3*2*1, "factorial failed!");
static_assert(factorial(3) ==             3*2*1, "factorial failed!");
static_assert(factorial(2) ==               2*1, "factorial failed!");
static_assert(factorial(1) ==                 1, "factorial failed!");
static_assert(factorial(0) ==                 1, "factorial failed!");

int main()
{
    try
    {
        auto make_factorial_value = [i = 0] () mutable
        {
            auto f = factorial(i);
            auto value = std::make_pair(i,f);
            ++i;
            return value;
        };
        
        std::map<int, int> map1;
        
        std::generate_n( std::inserter(map1, std::begin(map1))
                       , 10
                       , make_factorial_value
                       );

        std::map<int, int, std::less<int>, Allocator<std::pair<const int, int>, 10>> map2;

        std::generate_n( std::inserter(map2, std::begin(map2))
                       , 10
                       , make_factorial_value
                       );    
        
        for(const auto& p : map2)
            std::cout << p.first << " " << p.second << '\n';
    
        ForwardList<int> list1;

		for (size_t i = 0; i < 10; ++i)
			list1.append(i);

        ForwardList<int, Allocator<int, 10>> list2;
        
        for(size_t i = 0; i < 10; ++i)
            list2.append(i);
        
        for(const auto& it : list2)
            std::cout << it << '\n';
    }
    catch(std::bad_alloc &)
    {
        std::cout << "Not enough memory to create container!" << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << "Error! " << e.what() << std::endl;
    }
}
