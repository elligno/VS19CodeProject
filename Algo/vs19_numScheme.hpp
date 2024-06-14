#pragma once

#include <iostream>
#include <type_traits> // 
#include <format> //C++20 feature
// STL numerical container
#include <valarray>
#include <numeric>
#include <array>
#include <algorithm>
#include <ranges>   // C++20 feature
// ... to be cpompleted
#include "../Template17/vs19_myFirstRng.hpp"
#include "../Classes/vs19_MinMod.hpp"
//#include "BaseTypes/vs19_SectionFlow.h"
//#include "BaseTypes/vs19_StateVariable.h"
#include "../vs19_AppConstant.hpp"
//#include "../TestCPP17/vs19_NumericalSourceTerms.h" // numerical source terms 

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

  template<typename T, auto N=101>
  requires(IsPointer<T>)
    void hllSchemeWithPtr( T aU1, T aU2, T aFF1, T aFF2) // E. McNeil version
    {
      static_assert( N == vsc19::EMCNEILNbSections::value);
    
      // do some numerical stuff (for examp[le E. McNeil function-ptr])
      // by using "call" function above (did some stuff like that in May24)
      // 
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

  template<typename T>
  concept IsPointer = std::is_pointer_v<T>;

  template<typename T>
  requires(!IsPointer<T>)
    T maxValue(T a, T b)
    {
      return b < a ? a : b;
    }

  // In our programming environment we could use this kind of signature??? 
  // can call it with (ArrayType aU, double dx, int nbSections)
  template<typename NumArray, typename... Types> //to be completed
  void numSchemeVariadic( NumArray&& aNumArray, Types... args)  // not sure about that signature!!  
  {
    // just a test VS15 it don't work
    MinMod<double> w_myMinMod{};

    // C++20 range concept and compile-time if  (if with initializatoion and follow by condition C++17)
    if constexpr( range_t<NumArray> numrng; std::ranges::range<decltype(numrng)>) // condition if it is a range
    { 
      static_assert( std::size(NumArray) == 101);
      std::cout << "rng is a range with exact value\n";
    }

    // Numerical impl. of the derivative at second-order
    //TreatmentTermeS0( aNumArray, std::forward<Types>(args...));  is that make sense?
    TreatmentTermeS0( aNumArray, std::forward<Types>(args)...); // is that make sense?
  }

  template<typename Range /*Numeric range*/>
  void numericalSchemeRef( const Range& aRange)
  {
    constexpr std::array myArray{ 1, 2, 3, 4, 5 };                            // (1)
    constexpr auto sum = std::accumulate(myArray.begin(), myArray.end(), 0);  // (2)
    std::cout << "sum: " << sum << std::endl;

   /*  constexpr std::string w_str{ "asd" };
    std::cout << "Checking C++20 new feature " << w_str << std::endl;

    constexpr std::vector<int> w_vecInt{ 1,2,3,4 };*/

    // some basic check before proceeding
    static_assert( std::is_same_v<decltype(aRange), const std::array<double, 3>&>);

    //if( !std::is_same_v<Range>) 
    //{
    //  std::cout << "Not a range\n";
    //}

    // just a dummy test
    //auto w_vaArraySiz = std::size(aRange);
    for( auto i = 0; i < std::size(aRange); i++)
    {
      std::cout << "Value is: " << aRange[i] << "\n";
    }

    //if(std::is_const_v)
  }

  
  // Usage:
  // compute dU1,dU2 minmod, don't need it anymore
  // allocate memory for aFF1, aFF2 
  //  hllFluxAlgorithm<double>()
  template<typename T, typename ArrayType = std::valarray<T>>
  void hllFluxAlgorithm(       ArrayType& aFF1, ArrayType& aFF2,          // return 
                         const ArrayType& aU1, const ArrayType& aU2,      // in 
                               ArrayType&& dU1, ArrayType&& dU2) noexcept // don't need it anymore (forward reference)
  {
    std::cout << "Debugging purpose\n";

    static_assert( std::is_same_v<decltype(ArrayType), const std::valarray<double, 3>&>);
    static_assert( std::size(aU1) == std::size(aU2), "Numerical array must be same size");

    // MinMod slope limiter
    // loop on cell faces (MUSCL reconstruction algorithm)

    // return global domain faces
    //auto w_glbFaces = createGlobalFaces();

    for( auto i = 0; i < std::size(aU1)-1; ++i)
    {
      // U1 variable
      const auto w_U1L = aU1[i] + dU1[i];
      const auto w_U1R = aU1[i + 1] - dU1[i + 1];

      // U2 variable
      const auto w_U2L = aU2[i] + dU2[i];
      const auto w_U2R = aU2[i + 1] + dU2[i + 1];
      /*     StateVector UL{ U1[i] + dU1[i], U2[i] + dU2[i] };
           StateVector UR{ U1[i + 1] - dU1[i + 1], U2[i + 1] - dU2[i + 1] };*/
           //  std::cout << "StateVector not supported yet\n";
             // compute physical flux
             // aFF1[i]=1.22;
             // aFF2[i]=0/23;
    }
  }

  // Numerical algorithm using default values for function template parameters 
  // Could be boost::ublas fast floating container 

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
    // C++20 range concept and compile-time if  (if with initializatoion and follow by condition C++17)
    if constexpr ( range_t<ArrayType> numrng; !std::ranges::range<decltype(numrng)>) // condition if it is a range
    {
      // std::ranges::distance(); just check
      using sect_sizet = decltype(NbSections);
      using arr_valuetype = ArrayType::value_type; // before C++20 need "typename ArrayType::value_type"

      static_assert( std::size(ArrayType) == NbSections);
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
    
#if 0
            // slope limiter stencil: 
    // Do some reconstruction of state variables at cell face (MUSCLreconstr)
    ArrayType dU1{ aU1.size() }; // gradient over cell
    std::adjacent_difference(std::begin(aU1), std::end(aU1), std::begin(dU1));
    dU1[0] = 0.; // force to zero
    ArrayType dU2{ aU2.size() }; // gradient over cell 
    std::adjacent_difference(std::begin(aU2), std::end(aU2), std::begin(dU2));
    dU2[0] = 0.; // force to zero (adjacent_difference leave first element as is)

    // Design Note
    //  we need to use range view for this slope limiter because
    //  compare adjacent value with stencil [i,i+1]
    //  [0,N[ and ]0,N]
    // slope limiter
    MinMod w_minModSLopeLimiter{};
    std::adjacent_difference(dU1.cbegin(), dU1.cend(), dU2.cbegin(), dU1.begin(), w_minModSLopeLimiter);

    // ... not sure about this one
    std::vector<arr_valuetype> w_dU1(std::begin(dU1), std::end(dU1));
    w_dU1.push_back(0.);

    // limiter function gradient sharp profile
    // remove first element ( boost range ]begin(),end()]) 

    std::vector<typename ArrayType::value_type> w_dU2(std::begin(dU2), std::end(dU2));
    w_dU2.push_back(0.);
    // limiter function gradient sharp profile
    // remove first element ( boost range ]begin(),end()]) 

#endif // 0

    // loop on cell faces
 /*   auto UL = 0.;
    auto UR = 0.;

    auto w_glbFaces = createGlobalFaces();

    for( auto i = 0; i < w_glbFaces.length(); ++i)
    {
      StateVector UL{ U1[i] + dU1[i], U2[i] + dU2[i] };
      StateVector UR{ U1[i+1] - dU1[i+1], U2[i+1] - dU2[i+1] };*/

      // compute physical flux
    }
/*
    // HLL algorithm
    auto FL = UL;
    // Factory method to create equation SCL (flux(UL,UR)) 
    auto FR = 0.; // physical flux

    HLLParams w_hllPrm{UL,UR};
    auto [SL,SR] = w_hllPrm.computeShockSpeed( UL,UR);

    // Numerical flux 
    if( SL<0.)
    {
      aFF1[0] = 0.;
    }
    else if( SR>0.)
    {
      aFF2[0] = 0.;
    }
    else {} // Whole formulae
    */
 // }

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

  /**
   * @brief Testing constexpr function (arguments are constexpr user defined type).
   *        constexpr variable initialized with this function is statically initialized.
  */
  // constexpr double SomeHydraulicPrm( SweStateVariable aStateVar, SectionFlow aSectF) noexcept
  // {
  //   return aStateVar.value()*aStateVar.value() / 2.*aSectF.getB();
  // }

} // End of namespace
