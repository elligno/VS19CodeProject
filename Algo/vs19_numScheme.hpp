#pragma once

#include <iostream>
#include <type_traits> // 
#include <format>      //C++20 feature
// STL numerical container
#include <vector>
#include <valarray>
#include <numeric>
#include <array>
#include <algorithm>
#include <ranges>      // C++20 feature
// ... to be cpompleted
#include "../Algo/vsc19_UpwindDerivativeBias.hpp"
//#include "../Template17/vs19_myFirstRng.hpp"
#include "../Template17/vs19_myFirstRngView.hpp"
#include "../Classes/vs19_MinMod.hpp"
//#include "BaseTypes/vs19_SectionFlow.h"
//#include "BaseTypes/vs19_StateVariable.h"
#include "../vs19_AppConstant.hpp"
#include "../vs19_UniversalConst.hpp"
//#include "../TestCPP17/vs19_NumericalSourceTerms.h" // numerical source terms 
#include "../Classes/valarrField1D.h"
//#include "../Classes/vsc19_Simulation.hpp"
#include "vs19_SweFluxEvaluation.hpp"

using float64 = std::double_t;

// forward declarations
namespace vsc19 {
  void EMcNeilCalculFF(double *aU1, double *aU2, double *aFF1, double *aFF2);
  void TreatmentTermeS0(std::vector<double> aVec, double dx, int NbSections);
  void TreatmentTermeS0(std::vector<double> aVec, double dx, double grav, int NbSections);
 // void HLL_Scheme(std::valarray<double>& aFF1,std::valarray<double>& aFF2, 
  //  const std::valarray<double>& aU1, const std::valarray<double>& aU2);
}

namespace vsc19 
{
   /* template <typename T>
  using is_vector = is_same_v<T, std::vector< typename T::value_type>,
    typename T::allocator_type>>;*/

  // C++20 concept
  template<typename T>
  concept IsPointer = std::is_pointer_v<T>;

  template<IsPointer T, auto N=101>
    void hllSchemeFlux( T aU1, T aU2, T aFF1, T aFF2) // E. McNeil original version
    {
      static_assert( N == vsc19::EMCNEILNbSections::value);
    
      // do some numerical stuff (function-ptr from legacy code)
      // by using "call" function above (did some stuff like that in May24)
      EMcNeilCalculFF(aU1,aU2,aFF1,aFF2); 
    }

  // In our programming environment we could use this kind of implementation
  // to support legacy code (many implementations of this function)  
  // NumArray: is a numerical array for fast-floating point operation
  // range_t: is a numerical interface for our numerical container
  // support C++20 range concept (used with our physics algorithm)
  template<typename Range, typename ...Types> 
  void hllSchemeFlux( Range&& aRange, Types&&... args) 
  {
    namespace rng = std::ranges;

    // use type
   // std::remove_reference_t<NumArray> w_numArray;

    // sanity check before we proceed with this algorithm
    // view cheap to copy/move, efficiency always!!!
    if constexpr (rng::range<decltype(aRange)> && // must be a range
         rng::viewable_range<decltype(aRange)>) // can be convert to a view
    {
      std::cout << "We can convert this range to a view" << "\n";
      std::cout << "Numerical Array of size: " <<  rng::size(aRange) << '\n';

      // depending if it is a rvalue/lvalue call different algo
      std::cout << std::boolalpha << std::is_rvalue_reference_v<decltype(aRange)> << '\n';

      // NOTE: case below we have a rvalue, use 'copy' forward reference version
      // pass argument using forward which in turn is a move since aRange rvalue.
      // if it is a rvalue reference use version (add physical b.c. at right far end)
      // can modify since we know we don't modify the original (not sure about this!!)
      if constexpr ( std::is_lvalue_reference_v<decltype(aRange)>) // check the type of 'aRange'
      {
        // check the value category of 'aRange' used as expression
        static_assert(std::is_lvalue_reference_v<decltype((aRange))>);
        auto fmt = std::format("Range type is a {} ", std::is_lvalue_reference_v<decltype(aRange)>);
        std::cout << std::boolalpha << fmt << '\n';
        // once 'forward' cannot use this (move) and iterator invalidated  
        //auto begNarr = aRange.begin();  copy of the iterator
        
        std::cout << "Range is a rvalue reference\n";
        // call algorithm which support view (pass an rvalue??)
        std::vector<rng::range_value_t<Range>> w_out;
        w_out.reserve(rng::size(aRange)); 
        //auto [cpyin, cpyout] =  move since NumArray is a rvalue
        rng::copy( std::forward<Range>(aRange), std::back_inserter(w_out));
 
        // int64_t OutputPosition{std::distance(
        //     w_out.begin(), cpyout - 1)};

        // std::cout << "\nThe last element copied is at "
        //              "position "
        //           << OutputPosition
        //           << "\nin the destination";

        // std::cout << "\nIts value is "
        //           << w_out[OutputPosition];

        // what we have here? looks that we have something
        w_out.push_back(0.); // right open b.c. (physical boundary)
        //   auto chkSizout = w_out.size();

        // call algorithm 'DerivativeBiasUpwind1st' derivative bias upwind
        // yield a span view???

        // contiguous storage (span view is created)
        auto w_spn = std::views::counted(rng::begin(w_out), rng::size(w_out));
        // Compute first derivative (stencil bias upwind backward) 
        auto w_dU = DerivativeBiasUpwind(w_spn); // pass by-value (cheap to copy/move)
        if( rng::view<decltype(w_dU)>)
        {
          std::cout << "Size of the returned view is: " << w_dU.size() << '\n';
          // convert a range to view
          auto w_Uview = std::views::all(aRange); // pass a rvalue (owning_view)
          auto w_ptr2Data = w_Uview.data(); // ptr to data
          // fast-floating point container
          std::valarray<rng::range_value_t<Range>> w_testData(w_ptr2Data, w_Uview.size());
          for( auto&& U1 : aRange) // value category not known 
          {                           // avoid to make a copy 
            std::cout << "U1 nodal value is: " << '\n';
            auto UL = U1 + w_dU[0]; // left state  (i+1/2)
            auto UR = U1 + w_dU[1]; // right state (i+1/2)
          }
        }
      }
    }//if
    // use trait to check type of container and compile-time if
    else if constexpr (std::is_same_v<Range, std::vector<float64>>)
    {
      // C++20 utility for ranges
      if ( !std::ranges::empty(aRange))
      {
        assert(std::ranges::size(aRange) == vsc19::DIM::value);
        std::cout << "range with exact value\n";

        // calling algo which support std::vector signature
      }

      // call function from legacy code (could be functions that support pointer)
      TreatmentTermeS0( std::forward<Range>(aRange), std::forward<Types>(args)...);
    }
    // else if constexpr( range_t<Range> numrng; std::ranges::range<decltype(numrng)>)
    // { 
    //   // C++20 utility for ranges
    //   if (!std::ranges::empty(aRange))
    //   {
    //     assert(std::ranges::size(aRange) == vsc19::DIM::value);
    //     std::cout << "range with exact value\n";
    //   }
    //   // call function from legacy code 
    //   TreatmentTermeS0(aRange, std::forward<Types>(args)...);
    // }
    else {std::cerr << "Not supported in the current version\n";}

    std::cout << "Test varidic algo functions completed\n";
  }

  #if 0
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
#endif
} // End of namespace
