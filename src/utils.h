#pragma once

#include <type_traits>

constexpr auto factorial(auto number) -> decltype(number)
{
  static_assert(std::is_integral<decltype(number)>::value, "Argument of factorial function must be integer type.");
  if(0 > number)
    throw std::invalid_argument("Argument of factorial function must be positive integer.");

  return number ? number * factorial(number-1) : 1;
}