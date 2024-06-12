#pragma once

#include <array>

namespace vs19 
{
  /**
   * @brief array of N element intiallized to 0
  */
  template<auto N> std::array<double, N> gArr{};

  /**
   * @brief global variable depending of the type
  */
  template<auto N> constexpr decltype(N) dval = N;

  /**
   * @brief variable template
   * @tparam T precision type
  */
  template<typename T = long double> constexpr T gPi = { 3.14159265358979 };

  /**
   * @brief Compute factorial uisng compile-time feature
  */
  template<int N>
  constexpr int factorial = factorial<N - 1>*N;
} // End of namespace
