#pragma once

//#include <ranges>
#include "vsc19_Simulation.hpp"

namespace {
   /**
    * @brief C++17 new feature (invoke)
    * @tparam Callable 
    * @tparam ...Args 
    * @param aCalbl 
    * @param ...arg 
  */
  template<typename Callable, typename ...Args>
  decltype(auto) callAlgoImpl( Callable&& aCalbl, Args&&... args)
  {
    std::invoke( std::forward<Callable>(aCalbl), // call passed to callable with 
      std::forward<Args>(args)...);              // all additional passed args
  }
}

namespace vsc19 
{
    /// @brief Model of "right-hand-side" dicretization operator (U_t = H(U;t))
    ///  Semi-discrete scheme (explicit)
    class LDeltaOperator 
    {
        public:

        /// @brief Compute numerical flux at ell face (RHS)
        /// @tparam NumArray numerical array of state variables
        /// @tparam FluxFunc numerical flux algorithm
        /// @param aFluxAlgo pointer-2-function
        /// @param aU1 state variable
        /// @param aU2 state variable
        template<std::invocable FluxFunc, typename NumArray> // not sure about the return value?
        decltype(auto) apply( FluxFunc&& aFluxAlgo, NumArray&& aU1, NumArray&& aU2)
        {
            static_assert( std::is_same_v<NumArray, std::valarray<double>>);

            // numerical face flux at cell interface i+1/2
            std::remove_reference_t<NumArray> w_FF1(std::size(aU1)-1);
            std::remove_reference_t<NumArray> w_FF2(std::size(aU2)-1);

            // check the type of 'aNumArray'
            if constexpr ( std::is_rvalue_reference_v<decltype(aNumArray)>) 
            {
                // check the value category of 'aNumArray' used as expression
                static_assert(std::is_lvalue_reference_v<decltype((aNumArray))>);
            }
            // delayed temporary 
            auto &&ptr2Func = Simulation::instance()->getFluxAlgorithm(); 
            
            if constexpr (std::is_invocable<FluxFunc>)
            {
                // forward reference need to use std::forward to restore 
                // the argument value category because inside this method
                // it's a lvalue (shouldn't be use this way, just testing)
                std::invoke( std::forward<FluxFunc>(aFluxAlgo), w_FF1, w_FF2, // lvalue
                             std::forward<NumArray>(aU1), std::forward<NumArray>(aU2));
           }

           // shall call the source term discretization S0-Sf (... energy slope)

           // for now but should return a rhs type (aggregate) SWERHS<ArrayType>
           return std::make_pair(w_FF1,w_FF2); 
        }
    };
} // End of namespace