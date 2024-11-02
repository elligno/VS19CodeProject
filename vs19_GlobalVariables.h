#pragma once

// C++ includes
#include <array>
#include <ranges>

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
   * @brief Compute factorial uisng compile-time feature
  */
  template<int N>
  constexpr int factorial = factorial<N - 1>*N;

  /**
   * @brief Contraining alias template
  */
  template<std::ranges::range T>
  using ValueType = std::ranges::range_value_t<T>;

  /**
   * @brief Contraining variable template
  */

} // End of namespace
