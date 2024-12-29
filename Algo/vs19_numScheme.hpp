#pragma once

#include <iostream>
#include <type_traits> // 
#include <format>      //C++20 feature
// STL numerical container
#include <valarray>
#include <numeric>
#include <array>
#include <algorithm>
#include <ranges>      // C++20 feature
// ... to be cpompleted
#include "../Template17/vs19_myFirstRng.hpp"
#include "../Classes/vs19_MinMod.hpp"
//#include "BaseTypes/vs19_SectionFlow.h"
//#include "BaseTypes/vs19_StateVariable.h"
#include "../vs19_AppConstant.hpp"
//#include "../TestCPP17/vs19_NumericalSourceTerms.h" // numerical source terms 
#include "../Classes/valarrField1D.h"

#if 0 // code is duplicated
/**
   * @brief coming ...
   * @tparam Callable 
   * @tparam ...Args 
   * @param aCalbl 
   * @param ...arg 
  */
  template<typename Callable, typename ...Args>
  decltype(auto) call( Callable&& aCalbl, Args&&... args)
  {
    std::invoke( std::forward<Callable>(aCalbl), // call passed to callable with 
      std::forward<Args>(args)...);              // all additional passed args
  }

 template<typename T>
  concept IsPointer = std::is_pointer_v<T>;

#endif 

// forward declarations
namespace vsc19 {
  void EMcNeilCalculFF(double *aU1, double *aU2, double *aFF1, double *aFF2);
  void TreatmentTermeS0(std::vector<double> aVec, double dx, int NbSections);
  void TreatmentTermeS0(std::vector<double> aVec, double dx, double grav, int NbSections);
}
namespace vsc19 
{
   /* template <typename T>
  using is_vector = is_same_v<T, std::vector< typename T::value_type>,
    typename T::allocator_type>>;*/

  /**
   * @brief C++17 new feature (invoke)
   * @tparam Callable 
   * @tparam ...Args 
   * @param aCalbl 
   * @param ...arg 
  */
  template<typename Callable, typename ...Args>
  decltype(auto) call( Callable&& aCalbl, Args&&... args)
  {
    std::invoke( std::forward<Callable>(aCalbl), // call passed to callable with 
      std::forward<Args>(args)...);              // all additional passed args
  }

  // C++20 concept
  template<typename T>
  concept IsPointer = std::is_pointer_v<T>;

  template<IsPointer T, auto N=101>
    void hllSchemeFlux( T aU1, T aU2, T aFF1, T aFF2) // E. McNeil version
    {
      static_assert( N == vsc19::EMCNEILNbSections::value);
    
      // do some numerical stuff (function-ptr from legacy code)
      // by using "call" function above (did some stuff like that in May24)
      EMcNeilCalculFF(aU1,aU2,aFF1,aFF2); 
    }

  template<typename T>
  requires(!IsPointer<T>)
    T maxValue(T a, T b)
    {
      return b < a ? a : b;
    }

  // In our programming environment we could use this kind of implenentation
  // to support legacy code (many implementations of this function)  
  // NumArray: is a numerical array for fast-floating point operation
  // range_t: is a numerical interface for our numerical container
  // support C++20 range concept (used with our physics algorithm)
  template<typename NumArray, typename... Types> 
  void numSchemeFlux( NumArray&& aNumArray, Types... args) 
  {
    // C++20 range concept and compile-time if  
    // (initialization and follow by condition C++17)
    if constexpr( range_t<NumArray> numrng; std::ranges::range<decltype(numrng)>)
    { 
      // C++20 utility for ranges
      if (!std::ranges::empty(aNumArray))
      {
        assert(std::ranges::size(aNumArray) == vsc19::DIM::value);
        std::cout << "range with exact value\n";
      }
      // call function from legacy code 
      TreatmentTermeS0(aNumArray, std::forward<Types>(args)...);
    }
    std::cout << "Test varidic algo functions completed\n";
  }

  // Experimenting some of the new C++20 concepts programming physics based algorithm
  template<std::ranges::input_range Range /*Numeric range*/>
  void numericalScheme1D( Range aRange)
  {
    namespace rng = std::ranges; // make things cleaner
    namespace vws = std::views;  // ditto
    
    using namespace std::literals;
    constexpr auto w_str{ "string_view"sv }; 
    std::cout << "Checking C++20 new feature " << w_str << std::endl;

    // some basic check before proceeding
    static_assert(std::is_same_v<decltype(aRange), vsc19::scalarField1D>);
    
    if constexpr (std::ranges::range<decltype(aRange)>)
    {
      auto cdist = std::ranges::distance(aRange);
    //  static_assert(!std::ranges::empty(std::add_const_t<decltype(aRange)>));

      std::cout << "scalar field is a range and is not empty\n";

      // transforming field in a view
      std::array<Range::value_type, vsc19::DIM::value> w_arrFieldValues;
      auto resultFromFieldCpy = std::ranges::copy(aRange,std::begin(w_arrFieldValues));
      static_assert(!std::is_same_v<std::ranges::dangling, decltype(resultFromFieldCpy.in)>);
      
      auto as = resultFromFieldCpy.out;
      
      std::cout << "Just about to leave numerical scheme 1D\n";
    }
    // some basic check before proceeding
//    static_assert( std::is_same_v<decltype(aRange), const std::array<double, vsc19::EMCNEILNbSections::value-1>>);
    static_assert(std::ranges::sized_range<decltype(aRange)>);
    auto [beg, end] = std::ranges::subrange(aRange); // structured binding

    //auto w_vaArraySiz = std::size(aRange);
     for( auto i = 0; i < std::ranges::size(aRange); i++)
     {
    //   std::cout << "Value is: " << aRange[i] << "\n";
     }
  }

  /**
   * @brief Numerical algorithm using default values for function template parameters
   * @tparam Args range of values 
   * @tparam Args range of values 
   * @tparam Args flux face values (in/out)
   * @tparam Args flux face values (in/out)
   * @return nothing 
  */
  template< typename ArrayType, // numerial array for fast floating-point operation
    auto NbSections = vsc19::EMCNEILNbSections::value> // default size for prototyping
  void hllFluxAlgorithm(        ArrayType& aFF1,      ArrayType& aFF2, // face flux
                         const  ArrayType& aU1, const ArrayType& aU2) noexcept // ?? not sure
  {
    // C++20 range concept and compile-time if  (if with initialization and follow by condition C++17)
    if constexpr ( range_t<ArrayType> numrng; !std::ranges::range<decltype(numrng)>) // condition if it is a range
    {
      // std::ranges::distance(); just check
      using sect_sizet = decltype(NbSections);
      using arr_valuetype = ArrayType::value_type; // before C++20 need "typename ArrayType::value_type"

      static_assert( std::size(ArrayType) == NbSections, "Array Size not equal to EMcNeil data");
      std::cout << "numrng is not a range with exact value\n";

      // difference type for the specified range's iterator
      // range_difference_t<ArrayType>; //??
      // distance
      auto rngFF1Dist = std::ranges::distance(aFF1);

      std::cout << "E. McNeil size value\n";
    }

    // aU over global domain (include ghost node)
  //  static_assert( std::size(aFF1) == NbSections - 1);         //<< "Face Flux and global U array incompatible";
  //  static_assert( std::size(aFF2) == NbSections - 1);         //<< "Face Flux and global U array incompatible";
    static_assert( std::is_same_v<double, ArrayType::value_type>); //<< "";
   }

  /**
   * @brief
   * @tparam ...Args
   * @param ...args
  */
  template<typename... Args>
  void hllFluxAlgorithmFwd(Args&&... args) // use forward reference (preserve type constness)
  {                                         // for perfect forwarding to the right function
    // Anthony Williams "C++ Concurrency" Appendix A give an introduction
    // about variadic template and the perfect forwarding feature

    // perfect forwarding preserve lvalue/rvalue-ness (forward to right function)
    hllFluxAlgorithm(std::forward<Args>(args)...); // call forward once for all arguments
    // look at N. Josuttis book move semantic page 145
  //  hllFluxAlgorithm(std::forward<Args>(args)...); // not the same thing, call forward for each argument
  }
} // End of namespace
