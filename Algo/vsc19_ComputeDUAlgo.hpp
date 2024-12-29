
#pragma once

// n umerical include
#include <valarray>
// Appl include
#include <../vs19_AppConstant.hpp>

namespace vsc19 
{
    /** @brief MUSCL reconstruction procedure of state variables at cell face 
	 * @tparam NumArrayType (fast floating-point array)
	 * @tparam F slope limiter function
	 * @param aU1 Range of (state variable A)
	 * @param aU2 Range of (state variable Q)
	 * @param aSlopeLimiter function that ... minmod  
	 * @return pair numerical flux at cell face j+1/2 ]0,N-1] (flux vector components (j+1/2)
	*/
	template<typename F, typename NumArrayType,
	typename = std::enable_if<std::is_same_v<NumArrayType,std::valarray<double>>>>
	auto MUSCLReconstruction( const NumArrayType& aU1, const NumArrayType& aU2,
			F&& aSlopeLimiter) // slope limiter function (Universal reference)
	{
		// Sanity checks (prototype variable)
		static_assert( std::size(aU1) == EMCNEILNbSections::value);
		static_assert( std::size(aU2) == EMCNEILNbSections::value);

		// Need documentation about each step of the algorithm 
		NumArrayType dU1( std::size(aU1)); // gradient of each cell
		NumArrayType dU2( std::size(aU2)); // ditto

		// compute gradient over each cell
		std::adjacent_difference( std::cbegin(aU1), std::cend(aU1), std::begin(dU1));
		std::adjacent_difference( std::cbegin(aU2), std::cend(aU2), std::begin(dU2));

		dU1[0] = 0.; // force first element to 0 (adjacent_difference keep first element unchanged)
		dU2[0] = 0.; // force first element to 0 (adjacent_difference keep first element unchanged)
		
		// apply slope limiter function for each gradient (minimum slope)
		std::adjacent_difference( std::cbegin(dU1), std::cend(dU1),  // range to apply limiter
			std::begin(dU1), // result 
			std::forward<F>(aSlopeLimiter)); // slope limiter function (forward reference preserve lvalue/rvalue-ness of the argument)
		
		std::adjacent_difference( std::cbegin(dU2), std::cend(dU2),  // range to apply limiter 
			std::begin(dU2), // result 
			std::forward<F>(aSlopeLimiter));   // ditto
		
        // with vector we had to push_back, shift first element at the end and keep same size (default value)
		auto w_dU1Shifted = dU1.shift(1);  // circular shift (shifted element at the end with element T{})  
		auto w_dU2Shifted = dU2.shift(1);  // circular shift (shifted element at the end with element T{})

		// last global discretization node (ghost node) overwrite
//		w_dU1Shifted[aU1.size() - 1] = aMinModSlopeLimiter(0., aU1[aU1.size() - 1] - aU1[aU1.size() - 2]);  // i+1/2 use to be 
//		w_dU2Shifted[aU2.size() - 1] = aMinModSlopeLimiter(0., aU2[aU2.size() - 1] - aU2[aU2.size() - 2]);  // i+1/2 ditto

        // NOTE all element are shifted (The new position of each element is i−count where i is the previous position.)
		w_dU1Shifted[aU1.size() - 1] = aSlopeLimiter(/*0.*/w_dU1Shifted[aU1.size() - 1], aU1[aU1.size() - 2] - aU1[aU1.size() - 3]); // i+1/2
		w_dU2Shifted[aU2.size() - 1] = aSlopeLimiter(/*0.*/w_dU2Shifted[aU2.size() - 1],aU2[aU2.size() - 2] - aU2[aU2.size() - 3]);  // i+1/2

		// don't need anymore of shifted array, might as well move it 
		// move semantic supported by valarray
		w_dU1 = std::move(w_dU1Shifted); // I no longer need this value here 
		w_dU2 = std::move(w_dU2Shifted); // ditto

		return {w_dU1,w_dU2};
	}
} // End of namespace