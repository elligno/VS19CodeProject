
#pragma once

// C++ includes
#include <iostream>
#include <tuple>
// app include
//#include "vs19_Utilities.hpp"

/**
 * @brief Different version of generic print function for tuple.
 *   
 * C++17 can be implemented by using the fold expression, otherwise
 * you need to use recursive template (C++ standard Josuttis).
 * ... to be completed
*/
namespace vsc19 
{
  /**
   * @brief Helper to print tuple (metaprogramming)
   * @tparam ...Args Parameter pack
  */
  template<int IDX, int MAX, typename... Args>
  struct PRINT_TUPLE
  {
    /**
     * @brief Print tuple elements
     * @tparam IDX tuple element index
     * @tparam MAX maximum number of elements
     * @tparam ...Args Parameter pack
     * @param aStream Stream object
     * @param aTpl2Print tuple to print 
    */
    static void print( std::ostream& aStream, const std::tuple<Args...>& aTpl2Print)
    {
      // stream data to stdout 
      aStream << std::get<IDX>(aTpl2Print) << (IDX + 1 == MAX) ? "" : ",";

      // recursive print (go to next element in the tuple until reach the end)
      // parameter expansion Args... 
      PRINT_TUPLE<IDX + 1, MAX, Args...>::print( aStream, aTpl2Print);
    }
  };

  // Partial specialization to end recursion (IDX=MAX)
  template<int MAX, typename... Args>
  struct PRINT_TUPLE<MAX,MAX,Args...>
  {
    static void print( std::ostream& aStream, const std::tuple<Args...>& aTpl2Print)
    {
      // end of recursion!!! do nothing
    }
  };

  /**
   * @brief Output operator << for tuple (overload and some format).
   *  Important to note that IDX is set to 0, start recursion over all elements
   *  std::get<0> is the first then go to next which is 1 (IDX+1) until IDX=MAX 
  */
  template<typename... Args>
  std::ostream operator<< ( std::ostream& aStream, const std::tuple<Args...>& aTpl2Print)
  {
    aStream << "[";
    PRINT_TUPLE<0, sizeof...(Args), Args...>::print( aStream, aTpl2Print);
    return aStream << "]";
  }

  // =================================================================
  //
  //  C++17 version using fold expression and and compile-time feature
  //
  // =================================================================

  // Variadic template print version 
  // NOTE i think i'm missing the 1 arg function to stop recursion
  //  print(T aVal2Print) for the last element of the parameter pack 

  /**
   * @brief Basic version using variadic template 
   * @tparam First
   * @tparam ...Args
   * @param ...args
  */
  template<typename T, typename... Types>
  void print( T firstArg, Types... args)
  {
    // NOTE
    // 'if' standard all branches is instantiaded, this might result
    // in some error for validity of code, even code intantiated not usefull 
    // (run-time decision). Using 'if constexpr' (compile-time if) only 
    // the instantiated part become relevant and its a compile time decision.
    // Code that might not be usefull is discarded (SFINAE version C++17).
    // 'Concepts' in C++20.   
    //   ... 
    //  print() is call with one argument, recursive call of print becomes 
    //  a discarded statement. Code only available for sizeof... (Args) > 0
    // SFINAE version C++17 (kind of)
    std::cout << firstArg << '\n';  // print first element
    //if( sizeof... (Args)>0)  error with sizeof == 0
    if constexpr (sizeof... (Types) > 0) // fine no code instantiaded for sizeof == 0
      print(args...);      // no print() function for no argument declared 
  }

  /**
   * @brief Print elements using fold expression
   * @tparam ...Types Args types to be printed
   * @param ...args Pack expansion
  */
  template<typename... Types>
  void print( const Types&... args)
  {
    // fold expression with initializer (std::cout but not sure)
    // no whitespace between printed args
    //(std::cout << ... << args) << '\n';

    // NOTE 'AddSpace(args)' use Class Template Argument Deduction (CTAD) 
    //      to have the effect AddSpace<Args>(args).
    // For each argument create an object that refers to the passed argument 
    // and adds a space when it is used. 
    (std::cout << ... << AddSpace(args)) << '\n';
  }

  // Usage
  //  
  template<typename Stream, typename... Columns>
  class CSVprinter
  {
  public:
    void output_line( const Columns&... columns)
    {
      // need print a line with separated blank between each columns 
    }
  };

  /**
   * @brief
   * @tparam First
   * @tparam ...Args
   * @param ...args
  */
  template<typename T, typename... Types>
  void printjb(T firstArg, Types... args)
  {
    std::cout << firstArg << '\n';  // print first element
    print(args...);
  }

  // just trying to implement a print function with C++17 feature
  // variadic template and fold expression 
  template<auto Sep = ' ', typename First, typename... Args>
  void print17( const First& aFirst2Print, const Args&... aArgs)
  {
    std::cout << aFirst2Print;

    // use auto keyword in lambda expression
    auto outWithSep = [] ( const auto& arg) 
    {
      std::cout << Sep << "arg"; 
    }

    // use fold expression to return
   // IMPORTANT the expansion '...' must have no white space
   //  with comma and lambda, stick all together, otherwise
   // it doesn't compile!!! still doesn't compile
    (... , outWithSep(aArgs)) << '\n';
    
  //  (..., [] ( const auto& arg) -> decltype(auto)
  //   {
  //     std::cout << Sep << "arg"; 
  //   }) << '\n'; 
    
    //std::cout << '\n';
  }
} // End of namespace