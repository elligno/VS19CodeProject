
#pragma once

// C++ includes
#include <iostream>
// STL includes
#include <numeric>
#include <vector>
#include <array>

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
    // generic lambda (C++17)
    auto callFoo = []<typename T>( T&& aType2Call)
    {
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