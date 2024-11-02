
#pragma once

// C++ includes
#include <cassert>
#include <iostream>
// C++20 includes
#include <concepts>
#include <ranges>  // C++20 ranges
// STL includes
#include <numeric>
#include <vector>
#include <array>
// App include
#include "vs19_valArrField.h"

namespace vs19 // forward declarations
{
  //** new C++20 feature (template function)*/ 
  void testAutoPrm( const auto& aAutoPrm) 
  {
    std::cout << "Auto parameter C++20 supported" << aAutoPrm << '\n';
  }

  class VS19Type; // forward declaration (incomplete type)
  /** C++20 Concept works fine with incomplete type*/ 
  void myFunc2Class( const std::same_as<VS19Type> auto& aVs19Type) 
  {
    aVs19Type.methodVs19();
  }

 /** Physics algorithm ...*/
 void TraitementTermeSource2(
      std::vector<double> &S, const std::vector<double> &Q,
      const std::vector<double> &A, const std::vector<double> &H,
      const std::vector<double> &n, const double dx, const int NbSections,
      double B /*=1*/);

  /** Another impl of physics algorithm based rvalue reference (move semantic)
   *   Usage
   *     TreatmentSource2(..., qField.asStdVector(), ...) pass a temporary  
  */
  void TreatmentSource2(  std::vector<double>& aS, 
      std::vector<double>&& aQ, std::vector<double>&& aA, std::vector<double>&& aH, //rvalue reference
      const std::vector<double> &n,  double aDx /*= 10.*/, unsigned aNbSections /*= 101*/)
      {
        // GlobalDiscretization->gamma().getDownstream(); b.c.
        // can freely modify the original and avoid the copy
        aQ.push_back(0.); // add ghost node value

        // do some calculation (compute derivative) use ghost node
        assert(101==aQ.size());

        std::cout << "Completed rvalue reference algorithm check\n";
      }

     //  void foo( const std::vector<int>& aVecInt);
     //  void foo( std::vector<int>& aVecInt);
}//End of namespace (fwd declaration)

namespace vsc19 {

  // C++17 
  // glvalue, bind to everything (mutable, unmutable, lvalue, ...) 
  void foo( const std::vector<int>& aVecInt)
  {
    if( std::empty(aVecInt))
    {
      /* code */
      std::cout << "We have an empty vector\n";
    }
    else {
      std::cout << "Vector not empty\n";
    }
  }

  // reference lvalue  
  void foo( std::vector<int>& aVecInt)
  {
    std::cout << "Calling reference signature\n";
  }

  // forward reference (perfect forwarding)
  template <typename T>
  void call2Foo( T&& aType) // Universal reference
  {
    // generic lambda (C++20) perfect forwarding with lambda
    auto callFoo = []<typename T>( T&& aType2Call)
    {
      // don't need to use decltype with std::forward
      // since we have a template (not auto&&) 
      // keep valueness of a type (perfect forwarding)
      foo( std::forward<T>(aType2Call));
    };

    // done at compile time
    if constexpr (!std::is_array_v<T>)
    {
      /* code */
      std::cout << "We have an array type\n";
      callFoo(aType); // might as well to call out generic lambda
    }

    std::cout << "Do whatever\n";
  }

   // testing some
  template <typename Range /*Numeric range*/>
  void numericalSchemeRef( const Range &aRange)
  {
    // some basic check before proceeding
    static_assert(std::is_same_v<decltype(aRange), const vsc19::valArrField &>);

    // d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
    // must be set to a valid range [1,N]
    // vsc19::gridLattice1D w_grid1D{std::string {"d=1 [0,1]x[1:10]"} };
    //  const auto dx = w_grid1D.Delta();

    vsc19::valArrField::gridlatticeptr w_aa{new vsc19::GridLattice{5, 0., 1.}}; // 5 nodes from xmin=0. to xmax=1.
    vsc19::valArrField w_newField{w_aa, std::string{"Jean Test"}, 2.3}; // initialize field with value
    
    const auto dimm = w_newField.grid().getNoSpaceDim();
    const auto min = w_newField.grid().xMin(1);
    const auto max = w_newField.grid().xMax(1);
    const auto nbPts = w_newField.grid().getNoPoints();
    const auto div = w_newField.grid().getDivisions(1);
    const auto &val = w_newField.values();
    auto siz = val.size();
    std::cout << "Value is :" << val[0] << "\n";

    auto beg = w_newField.begin();

    //  auto& w_val =  w_newField.values();
    //  w_val[0] = 1.2;
    //  w_val[1] = 0.3;
    //  w_val[2] = 1.;
    //  w_val[3] = 12.3;
    //  w_val[4] = 0.;
    //  std::views::counted w_fieldVws(w_newField.cbegin()+1, 5);

    // C++20 range concept and compile-time if (range is a concept something that is iterable)
    // if with initialization C++17
    //  IMPORTANT i'm using the same as above, but its a new  field with zero initialization
    // if-initialization that's the reason compiler not complaining about already defined
    //  w_newField here is another instance defined inside if, that's explain why it returns
    //  zero values when calling range::begin and range:;end (distance is ok, we have 5 grid nodes)
    //  we didn't set any values for this field (it  works perfectly). Our field is a range
    if constexpr (vsc19::valArrField w_newField{w_aa, std::string{"Swe"}}; std::ranges::range<decltype(w_newField)>)
    {
      namespace rng = std::ranges; // make things cleaner
      namespace vws = std::views;  // ditto

      std::cout << "scalar field is a range\n";\

      // NOTE our scalar field is not initialized with values (actually it is initialized to 0 default value)
      // the grid has 5 elements or node (below our test should return 0 and i think its what it does)

      auto begRng = rng::begin(w_newField);         // use ranges utility (safer)
      auto endRng = rng::end(w_newField);           // ditto
      //auto rngDist = std::distance(begRng, endRng);  shall return 5
      auto rngDist = rng::distance(w_newField);
      assert(w_newField.grid().getNoPoints() == rngDist);
      auto checkSiz = rng::size(w_newField);        // ditto
      // not sure about this one (views are subrange)
     auto vwsField = w_newField | vws::take(5); // use auto with views

      // could we use views to print some field values?
      // C++17 feature "Class Template Argument Deduction" (CTAD)
      for (const auto &elem : w_newField | std::views::take(5))
      {
        std::cout << "Printing five elements\n";

        std::cout << "Values is : " << elem << "\n";
      }
    }

    // some basic check before proceeding
    static_assert(std::is_same_v<decltype(aRange), const std::array<double, 3> &>);

    constexpr std::array myArray{1, 2, 3, 4, 5};                             // (1)
    constexpr auto sum = std::accumulate(myArray.begin(), myArray.end(), 0); // (2)
    std::cout << "sum: " << sum << std::endl;

    constexpr std::string w_str{"asd"};
    std::cout << "Checking C++20 new feature " << w_str << std::endl;

    //  constexpr std::vector w_vecInt{ 1,2,3,4 };
    // constexpr auto sum = std::accumulate( w_vecInt.cbegin(), w_vecInt.cend(), 0);
  }

  /** Experimenting some new C++20 features which 
   * I believe will be helpfull in our numerical library */  
  template<std::ranges::input_range Range> // C++20 concept of range
  decltype(auto) hllNumAlgorithm1D( const Range& aU1, const Range& aU2) // Range could be our scalar field
  {
     if constexpr (std::ranges::range<decltype(aU1)> && std::ranges::range<decltype(aU2)>)
     {
       std::cout << "We have our ranges and ready to proceed\n";
      static_assert(std::size(aU1) == std::size(aU2));
     }
     if constexpr (!std::empty(aU1)) // C++23 we have "if consteval"
     {
      std::cout << "Ready to compute some of our algorithm\n";
     }
     std::cout << "Completed our basic tests of C++20 features\n";   
     // just return something for debug purpose
     return std::vector<double>(std::size(aU1));
  }
} // End of namespace