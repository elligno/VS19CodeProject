
#pragma once

// C++ includes
#include <iostream>

#include <ranges>  // C++20 ranges

// STL includes
#include <numeric>
#include <vector>
#include <array>
// App include
#include "vs19_valArrField.h"

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
  
  void foo( std::vector<int>& aVecInt)
  {
    std::cout << "Calling reference signature\n";
  }

  // forward reference (perfect forwarding)
  template <typename T>
  void call2Foo( T&& aType) 
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

    //d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
    // must be set to a valid range [1,N]
   // vsc19::gridLattice1D w_grid1D{std::string {"d=1 [0,1]x[1:10]"} };
  //  const auto dx = w_grid1D.Delta();
    
    //auto w_gr1d = std::make_shared<vsc19::gridLattice1D>(std::string {"d=1 [0,1]x[1:10]"}); 
   // vsc19::scalarField1D w_fieldLattice{w_gr1d, std::string{"Field Lattice"}};
    
    vsc19::valArrField::gridlatticeptr w_aa{new vsc19::GridLattice{5,0.,1.}};
    vsc19::valArrField w_newField{w_aa, std::string{"Jean Test"},2.3};

    auto dimm = w_newField.grid().getNoSpaceDim();
    auto min = w_newField.grid().xMin(1);
    auto max = w_newField.grid().xMax(1);
    auto nbPts = w_newField.grid().getNoPoints();
    auto div = w_newField.grid().getDivisions(1);

    const auto& val = w_newField.values();
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
    if constexpr ( vsc19::valArrField w_newField{w_aa, std::string{"Swe"}}; std::ranges::range<decltype(w_newField)>) {
		 	std::cout << "scalar field is a range\n";
      
      namespace rng = std::ranges; // make things cleaner
      
      auto begRng = rng::begin(w_newField);  // use ranges utility (safer)
      auto endRng = rng::end(w_newField);    // ditto
      auto rngDist = std::distance(begRng,endRng);
      
      //assert()
      auto checkSiz = rng::size(w_newField); // ditto

      // is that make sense!! NO do not compile 
    //  std::views<vsc19::scalarField1D> w_vws(w_newField.cbegin()+1, w_newField.cend()-1);

      // could we use views to print some field values?
      // C++17 feature Class Template Argument Deduction (CTAD)
      for( const auto& elem : w_newField | std::views::take(5))
      {
         std::cout << "Printing five elements\n";

         std::cout << "Values is : " << elem << "\n";
      }
    }

    std::cout << "Do whatever\n";
  }

#if 0
    template <typename Range /*Numeric range*/>
    void numericalSchemeRef( const Range& aRange)
    {
      // some basic check before proceeding
      static_assert( std::is_same_v<decltype(aRange), const std::array<double, 3> &>);

      constexpr std::array myArray{1, 2, 3, 4, 5};                             // (1)
      constexpr auto sum = std::accumulate(myArray.begin(), myArray.end(), 0); // (2)
      std::cout << "sum: " << sum << std::endl;

      constexpr std::string w_str{"asd"};
      std::cout << "Checking C++20 new feature " << w_str << std::endl;

      //  constexpr std::vector w_vecInt{ 1,2,3,4 };
      // constexpr auto sum = std::accumulate( w_vecInt.cbegin(), w_vecInt.cend(), 0);
    }
#endif

} // End of namespace