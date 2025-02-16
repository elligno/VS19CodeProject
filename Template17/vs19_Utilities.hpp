
#pragma once

// C++ includes
#include <fstream> // ifstream
#include <sstream>
#include <iostream>
#include <optional>
#include <charconv>    // from_char
// STL includes
#include <algorithm>
#include <vector>
#include <concepts>    // C++20 
#include <string_view> // C++17
#include <filesystem>  // C++17
//#include <type_traits>  std::decay
// Base Types package 
//#include "vs19_MinMod.hpp"
#include "../vs19_AppConstant.hpp"

namespace vsc19 
{
  /**
   * @brief Convert string to int if possible
   * @param aStrv string to convert
   * @return integer value
  */
  inline std::optional<int> toInt( std::string_view aStrv)
  {
    // NOTE string_view cheap to copy, pass-by-value
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
   * @brief Printing values with separator
   * 
   * @Usage printValue<'-'>(); as an alternative
  */
  template<auto Sep = ' ', typename First, typename... Args>
  void printAutoSep( const First& aFirst, Args... aArgs) 
  {
    std::cout << aFirst;

    // lambda or anonymous function
    auto addSpace = [] ( const Args& arg) 
    {
      std::cout << Sep << arg; 
    };

    // fold expression as return (operator <<)
    ( ...,addSpace(aArgs));
    
    std::cout << '\n';
  }
 
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
   * @brief Ensure that any output of any argument is extended by a space (can be used with fold expression).
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

  // +++++++++++++++++++++++++++++++++++++++++++++++++++
  // fold expression (see N. Josuttis chap 14 
  //   " C++17 The Complete Guide) template function
  // +++++++++++++++++++++++++++++++++++++++++++++++++++
    
#if 0
// Using fold expression
// Note this could be done in a more comppact form
template<typename First, typename... Args>
void printWithSpace( const First& aFirstElem, const Args&... arg) 
{
  std::cout << aFirstElem; // print first element
  // NOTE by default lambda return objects by value. This mean
  // create an unnecessary copy of the arg. The way to avoid that
  // is to explcitly declare the return type of the lambda as to be 
  // const auto& or decltype(auto).
  addSpaceBefore = [] ( const Args& aArg) -> decltype(auto)
  {
    std:: cout << " "; // add a space before print arg
    return aArg; // return arg to be print 
  };

  // operator << apply to fold expression
  (std::cout << ... << addSpaceBefore(arg)) << '\n';
}
#endif

// NOTE by default lambda return objects by value. This mean
   // create an unnecessary copy of the arg. The way to avoid that
   // is to explcitly declare the return type of the lambda as to be 
   // const auto& or decltype(auto).
template<typename First, typename ...Args>
void printAddspace10( const First& aFirstElem, const Args& ...args) 
{
  std::cout << aFirstElem;
  // avoid unnecessary copy of 'aArg' since lambda return objects by value (default)
  (std::cout << ... << [] (const Args& aArg) -> decltype(auto) 
  {
     std::cout << ' ';
     return aArg;
  }(args)) << '\n';
}

/**
 * @brief Comparison of two characters.
 * @tparam T character type
 * @param a a character
 * @param b a character to compare with
 * @return succeed if both char sequence are identical
*/
	template<typename T, std::size_t N, std::size_t M> // could we use auto instead of size_t?
		//typename = std::enable_if_t<std::is_same_v<T, char>>>
	requires std::is_same_v<T, char>
		std::optional<int> id17( const T(&a)[N], const T(&b)[M])  // arguument deduction (decay??) N. Josuttis "C++17 The Complete Guide" 
	{                                                           // instead of using T* not sure about it (size must be equal)
		int i {};   // initialization (default ctor is called initialization)

	//	int m = 1;
		while (a[i] != '\0' || b[i] != '\0') {  // until the end of the word
			if (a[i] != b[i]) {                   // check character equality
				//m = 0; break;                     ... original code
				return std::nullopt;
			}
			++i;
		}//while-loop

		//return std::make_optional<int>(std::nullopt);
		return std::make_optional(1);
	}

// using another feature of cC++17 is auto keyword in template parameter
// another example using auto as template parameter
//template<auto sep=" ", typename First, template ...Args>
//void printAddspace10() {}

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

/**
   * @brief Check type parameters are the same.
   * @Usage 
   *   to be completed
  */
template<typename T, typename... TN>
struct isHomegeneous {
  // using fold expression (C++17 new features)
  static constexpr bool value = (std::is_same_v<T,TN> && ...);
};

/** @brief Read value from a stream
 *  @return type
 */
template <typename T> T read(std::istringstream &is) {
  T t;
  is >> t;
  return t;
}

/** @brief Parse a line and return values as tuple
 *  @return let compiler deduce return type
 */
template <typename... Args> auto parse(std::istringstream &is) {
  return std::make_tuple(read<Args>(is)...);
}

/** @brief Read file format "full result" for each simulation time
 * Use utility function to parse line (return a tuple with
 * value in reverse order tuple index 4->0 instead of 0->4)
 * Return a vector of values corresponding at simulation time.
 */
template <typename... Args> // let compiler deduce return type
auto extractLineValuesTmplt( std::ifstream* afileStream, std::string&& aFirstLine) {
  std::string w_line2Read{std::move(aFirstLine)};
  std::vector<std::tuple<Args...>> w_vecoftpl;
  w_vecoftpl.reserve(vsc19::EMCNEILNbSections::value); // hard coded!!!
  do { // read full result file format (wave simulator storing result)
    // replace sequence of white space by a single one (Qt version)
    //auto w_lineTrimmed = w_line2Read.simplified().toStdString();
    // using STL algorithm
    // does it returns something (make white unique)
    // an iterator to new end (logical end), need to test this
    std::string w_lineTrimmed{ w_line2Read.begin(),    // begin
    std::unique(w_line2Read.begin(),w_line2Read.end())}; // logical end
     
    // step to next white space one after one
    std::istringstream w_iis{w_lineTrimmed};

    // parse a line with format X|H|A|...
    auto w_tplResult = parse<Args...>(w_iis);
    w_vecoftpl.push_back(std::move(w_tplResult)); // not sure we are moving something!! No, see
                                                  // Scott Meyer's book item #25

    // maybe i should clear the string (just to make sure)
    w_line2Read.clear(); // ready to read the next line
  } while (std::getline(*afileStream,w_line2Read) && !w_line2Read.empty());
  // Qt version 
  //} while (afileStream.readLineInto(&w_line2Read) && !w_line2Read.isEmpty());

  return w_vecoftpl;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++
//                C++20 concepts
// ++++++++++++++++++++++++++++++++++++++++++++++++

// Reference
//  https://johnfarrier.com/breaking-down-c20-callable-concepts/

template <std::invocable F>
void call_if_callable(F func) {
    std::cout << "Callable!n";
    func();
}

// shall be same as the function above (C++20)
void callIfCallable( std::invocable auto func)
{
  std::cout << "Callable!n";
  func();
}

// Template function that requires a RegularInvocable
template <std::regular_invocable<double> T>
double computeAndPrint(T&& callable, double value) {
    double result = callable(value);
    std::cout << "Result: " << result << "n";
    return result;
}

// Using the function that accepts a std::regular_invocable
  //  computeAndPrint(squareRoot, 16.0);  Valid, as squareRoot is side-effect free and consistent

    // Lambda example
 //   auto lambda = [](double x) { return x * x; };
 //   computeAndPrint(lambda, 4.0); // Also valid for the same reasons

// Template function that requires an Invocable
template <std::invocable<int> T>
void invokeWithArg(T&& callable) {
    callable(42);
}

// Function template using std::invocable
template<std::invocable F>
void myFunction(F func) {
    std::cout << "Invoking function...n";
    func(); // Invoke the callable
}

// Lambda function
    // auto printMessage = []() {
    //     std::cout << "Hello, C++20 Concepts!n";
    // };

    // Pass the lambda to the function template
   // myFunction(printMessage);

    // Function pointer
   // void (*funcPtr)() = []() { 
   //     std::cout << "Function pointer called.n"; 
    //};

    // Pass the function pointer to the function template
    //myFunction(funcPtr);

    // Uncommenting the following line will cause a compile-time error
    // myFunction(5); // Error: '5' does not satisfy std::invocable
} // End of namespace vsc19