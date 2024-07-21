
#pragma once

// C++ include
#include <iostream>
#include <optional>
#include <charconv>    // from_char

#include <string_view> // ...
//#include <type_traits>  std::decay

// Base Types package 
#include "vs19_MinMod.hpp"

namespace vsc19 
{
  /**
   * @brief Convert string to int if possible
   * @param aStrv string to convert
   * @return integer value
  */
  inline std::optional<int> toInt( std::string_view aStrv)
  {
    int val {}; // init to 0

    // Return a struct 'from_char_results' value and error code
    // Using structured binding to retrieve return result with 'if initialization'
    if( auto [ptr, ec] = std::from_chars( aStrv.data(), aStrv.data() + aStrv.size(), val); ec != std::errc{})
    {
      return std::nullopt;
    }
    return val;
  }

  /**
   * @brief operator '+' on string_view
   *
   * @param aStr1 a first string
   * @param aStr2 a second string
   * @return sum of both string (concatenate)
 */
  inline std::string operator+ ( std::string_view aStr1, std::string_view aStr2)
  {
    return std::string(aStr1) + std::string(aStr2);
  }

  // Want to test when passing string_view as args, 
  // Usage
  //   string_view str1{"Hi"};
  //   string_view str2 {"Guys!"};
  //   auto xy = concat(str1,str2)

  /**
   * @brief Use auto as return type because it does not extends lifetime of the string object.
   * @tparam T type
   * @param x arg
   * @param y arg
   * @return sum of both args
   *
   * Important to notice returning T instead of 'auto',
   * may result in a string_view return type, cause a runtime error
   * because it does not extends lifetime of the string object.
   * Its better to let compiler deduce type by returning 'auto'.
   * @Usage
   *   string_view str1{"Hi"};
   *   string_view str2 {"Guys!"};
   *   auto xy = concat(str1,str2);  return type
  */
  template<typename T> // let compiler deduce type
  auto concat( const T& x, const T& y)
  {
    return x + y;
  }

  /**
   * @brief prevent returning a reference
   * @tparam T1
   * @tparam T2
   * @tparam RT
   * @param aT1
   * @param aT2
   * @return
  */
  template<typename T1, typename T2,
    typename RT = std::decay_t<decltype(true ? T1() : T2())>> // ensure that no reference can be returned
    RT max( T1 aT1, T2 aT2)
  {
    return aT1 < aT2 ? T1 : T2;
  }

  /**
   * @brief Testing template mechanism with keyword auto 
   * 
   * @Usage printValue<42>(); not sure yet
  */
  template<auto N> void printNValue() { std::cout << "Value is: " << N << std::endl; }

  /**
   * @brief Define variable template (since C++14)
   * @tparam T variable type
  */
  template<typename T = std::string>
  T myGlobObj{ "GblMsg" };

  /**
   * @brief C++17 (constexpr) implies inline and definition
  */
  struct Dstruct
  {
    static constexpr int n = 7; // implies inline and definition
  };

  /**
   * @brief Ensure that any output of any argument is extended by a space (can be used with fold expression)
   * @tparam T Argument type
  */
  template<typename T>
  class AddSpace
  {
    private:
      const T& m_refStr; // refer to argument passed in constructor
    public:
      AddSpace( const T& aStr) : m_refStr(aStr) {}

      friend std::ostream operator << ( std::ostream& os, AddSpace<T> aSpace)
      {
        return os << aSpace.m_refStr << ' '; // output passed argument and a space
      }
  };

   /**
   * @brief Sum arguments of the parameter pack
   * @tparam ...T Argument type
   * @param ...args Parameter pack
   * @return Sum of the args (fold expression)
  */
  template<typename... T>
  auto sumAll( T... args)
  {
    //(arg1+arg2)+arg3 ... 
    return (... + args); 
  }

  /**
   * @brief Mapping a function to a range to modify it
   * @tparam F functor or callable or lambda
   * @tparam R range
   * @param aFunc function that modify
   * @param aRng  range arg
   * @return range with nmodified value
  */
  template<typename F, typename R>
  R mapf( F&& aFunc, R aRng)
  {
    std::transform( std::begin(aRng), std::end(aRng),
      std::begin(aRng),
      std::forward<F>(aFunc));

    return aRng;
  }

  // fold expression (see N. Josuttis chap 14 C++17 The Complete Guide) template function 

// just testing variadic template 
template<typename ... Params> // parameter pack
struct jbTpl 
{
   std::tuple<Params...> m_tpl; // pack expansion
};

// pattern creation
template<typename ... Params>
struct jbTpl1 {
   std::tuple<std::string, Params...> m_tplPatrn;
};

template<typename ... Params>
struct jbTpl2 {
   std::tuple<std::unique_ptr<Params> ...> m_tpluniqPtr; // intantiate for each arg in the pack
};
#if 0
  // NOTE
// error generated when compiling because ::fabs is not constexpr in C++20
// C++23 introduce a vrsion which is constexpr
// we are calling a constexpr type but it use a non-constexpr ::fabs
// "expression cannot evaluate to constant"
// return by value
  constexpr auto computeSlopeLimiter(double dU1, double dU2)
  {
    return vs19::MinMod<double>{}(dU1, dU2);
    //return w_minMod(dU1, dU2);
  }
#endif // 0
} // End of namespace
