
#include "vsc19_ComputeDUAlgo.hpp" // MinMod slope limiter function
//#include "vs19_SweFluxEvaluation.hpp"

namespace vs19 
{
   /** @brief MUSCL reconstruction procedure of state variables at cell face 
	 * @tparam NumArrayType (numerical array for fast floating-point array)
	 * @tparam F slope limiter function
	 * @param aU1 Range of (state variable)
	 * @param aU2 Range of (state variable)
	 * @param aSlopeLimiter function that ... minmod  
	 * @return pair numerical flux at cell face j+1/2 ]0,N-1]
	*/
	template<typename F, typename NumArrayType, // state variables components
	typename = std::enable_if<std::is_same_v<NumArrayType,std::valarray<double>>>>
	auto MUSCLReconstruction( const NumArrayType& aU1, const NumArrayType& aU2, 
			F&& aSlopeLimiter) // slope limiter function (Universal reference)
	{

		// NOTE 
		//  MUSCL (Monotone Upwind Scheme Conservation System Law) 
		//  Second-order linear extrapolation (polynomial reconstruction at the interface).
		//  Total Variation Diminishing (TVD) property guarantees convergence of such schemes
		//  Uses the Minmod limiter for the slope reconstruction

        // compute gradient over each with applying slope limiter function 
        computeDU(aU1, aU2, std::forward<F>(aSlopeLimiter)); // gradient over each cell

		//
		// Reconstruction process at the cell interface x_j+1/2
		// 
        // Compute the right and left states: UR, UL, FR et FL (computational domain)
		const NumArrayType UL1{ std::begin(aU1 + 0.5 * dU1), Sfx::DIM::value};  // j
		const NumArrayType UL2{ std::begin(aU2 + 0.5 * dU2), Sfx::DIM::value};  // j+1
        const NumArrayType UR1{ std::next(std::begin(aU1 - 0.5 * dU1)), Sfx::DIM::value}; 
		const NumArrayType UR2{ std::next(std::begin(aU2 - 0.5 * dU2)), Sfx::DIM::value};

        // is it possible to do this? is that make sense? not really!! use type
		// vector doesn't tell me much
		// i think what would make sense, return a vector of cellFaceVariables 
        return {UL1,UL2,UR1,UR2}; // return a tuuple?? can i use structured binding?

		//const auto UL2 = aU2 + 0.5 * dU2;
	//	const auto UR1 = aU1 - 0.5 * dU1;
	//	const auto UR2 = aU2 - 0.5 * dU2;
#if 0
	//	const decltype(UL2) FL1{ std::begin(UL2), UL2.size() - 1 };           numerical flux first state variable at cell face j+1/2 left state
	//	const decltype(UR2) FR1{ std::next(std::begin(UR2)), UR2.size()-1 };  numerical flux first state variable at cell face j+1/2 right state
	
        // is this work?
		const decltype(auto) FL1{ std::begin(UL2), UL2.size() - 1 };          // numerical flux first state variable at cell face j+1/2 left state
		const decltype(auto) FR1{ std::next(std::begin(UR2)), UR2.size()-1 }; // numerical flux first state variable at cell face j+1/2 right state

		// Compute the flux vector
		//const auto FL1 = UL2[i];
		//const auto FR1 = UR2[i + 1];

		// Flux (physical) for the St-Venant equation (without pressure term)
		const auto FL2 = StVenant1D_Incomplete_Flux(UL1, UL2);
		const auto FR2 = StVenant1D_Incomplete_Flux(UR1, UR2);

		//const auto uL = UL2[i] / UL1[i];
		//const auto uR = UR2[i + 1] / UR1[i + 1];

		const auto uL = UL2 / UL1;
		const auto uR = UR2 / UR1;

		//const auto CR = ::sqrt(g * UR1[i + 1] / wT());
		//const auto CL = ::sqrt(g * UL1[i] / wT());

		const auto CR = std::sqrt(vsc19::cGravity<double>*UR1 /*/ hyd::SectionWidth{}*/);
		const auto CL = std::sqrt(vsc19::cGravity<double>*UL1 /*/ hyd::SectionWidth{}*/);

		// Étape intermédiaire: calcul de US et CS
		const auto CS = 0.5 * (CL + CR) - 0.25 * (uL - uR);
		const auto uS = 0.5 * (uL - uR) + CL - CR;

		// shock speed for the left and right state
		//const auto SL = std::min(uL - CL, uS - CS);
		const auto SR = std::max( uR + CR, uS + CS);

		// compute shock speed on left/right shock location (continuity equation)
		const auto dL = uL - CL; 
		const auto dS = uS - CS;
		std::valarray<double> SL{ dL.size(); } // left shock speed 
		std::transform( std::begin(dL), std::end(dL), std::begin(dS), // ranges
			std::begin(SL), // result
			[](double aVal1, double aVal2) -> double // lambda (anonymous function)
			{ return std::min(aVal1,aVal2); });
#endif
    }
} // End of namespace