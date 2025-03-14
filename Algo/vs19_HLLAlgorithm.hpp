#pragma once

// C++ include
#include <cassert>
// Range library
#include <ranges>
// STL container
#include <valarray>
#include <vector>
#include <map>
// STL algorithm
#include <algorithm>
#include <numeric>
// ...
#include "../vs19_AppConstant.hpp"
#include "../vs19_UniversalConst.hpp"
#include "vs19_SweFluxEvaluation.hpp"
//#include "numeric/Sfx_MinMod.hpp"   minmod
//#include "../Classes/vs19_MinMod.hpp"
#include "../Algo/vs19_MUSCLReconstr.hpp"
#include "../Classes/vsc19_Simulation.hpp"

// alias
//using ArrayType = valarray<double>;

#if 1 // for now testing purpose
// Alias template (ArrayType<>)
template<typename T = double>
using NumArrayType = std::valarray<T>;
#endif // 1

// forward declarations
namespace vsc19 {
	void HLL_Scheme(std::valarray<double>& aFF1,std::valarray<double>& aFF2, 
	  const std::valarray<double>& aU1, const std::valarray<double>& aU2);
}
  
namespace Sfx 
{    
	//   HLL solver is a Riemann Approximate Solver (belongs to the family of Godunov-type scheme)
	//   Godunov scheme is based on the solution of the Riemann problem
	//   in which two states (U_l, U_r) are separated by a discontinuity, 
	//   defining a Riemann problem.
	//   
	//   Second order Godunovtype schemes based on the Minmod limiter. Namely, the piecewise linear projection in
	//   such schemes is based on cell averages and uses the Minmod limiter for the slope reconstruction. 
	//   We call such a scheme minmod - type.
	//   value in each cell is the cell average of the approximate solution after the evolution step.
	//   Reference:
	//       to be completed paper title and authors 
	//   https://people.tamu.edu/~popov//minmod2.pdf
	
	/**
	 * @brief HLL solver is a Riemann Approximate Solver (belongs to the family of Godunov-type scheme)
	 * @tparam NumArrayType (nunmerical array for fast floating-point array)
	 * @tparam F slope limiter function
	 * @param aU1 Range of nodal value (state variable)
	 * @param aU2 Range of nodal value (state variable)
	 * @param aSlopeLimiter 
	 * @return pair numerical flux at cell face j+1/2 ]0,N-1]
	*/
	template<typename Func, typename NumArrayType,
	typename = std::enable_if<std::is_same_v<NumArrayType,std::valarray<double>>>>
	auto HLLFluxAlgorithmTest( const NumArrayType& aU1, const NumArrayType& aU2, Func&& aSlopeLimiter) 
	{
		// flux vector components (j+1/2)
		// Computational domain extension [1,N]
		// Global domain exetension [1,N+1]
		// the  extra node is call "Ghost Node" since its main use is to compute 
		// gradient in the reconstruction process of state variables at cell face.   
		// First cell face is between first and second node (first node is tied)
		// not part of the computation, its value is set by b.c. 
		// Last cell face is just after last node of the computational domain
		// return list of numerical flux at those cell faces, pair of these 
		// can be used to represent "cell" and where 
		//if constexpr( std::is_same_v<NumArrayType, std::valarray<double>>)
		//{
		//	//SFINAE (Substitution Failure Is Not An Error)
		//	// run this algortihm
		//}
		//else
		//{
		//	// do something else (vector or scalarField)
		//	//HLL_Scheme(); E. McNeil original code
		//}

		// Sanity checks
		assert( std::size(aU1) == vsc19::EMCNEILNbSections::value);
		assert( std::size(aU2) == vsc19::EMCNEILNbSections::value);

		// NOTE 
		//  MUSCL (Monotone Upwind Scheme Conservation System Law) 
		//  Second-order linear extrapolation (polynomial reconstruction at the interface).
		//  Total Variation Diminishing (TVD) property guarantees convergence of such schemes
		//  Uses the Minmod limiter for the slope reconstruction

		//
		// MUSCL reconstruction (second-order extrapolation) at the cell interface x_j+1/2
		//
		
		// compute gradient over each cell by applying slope limiter function
		// auto [dU1,dU2] = computeDU(aU1, aU2, std::forward<F>(vsc19::MinMod19<double>{})); // gradient over each cell
		auto [UL1, UL2, UR1, UR2] = // at global faces = DIM
		   vs19::MUSCLReconstruction(aU1, aU2, std::forward<Func>(aSlopeLimiter));

		const std::valarray<double> FL1 = UL2; //{ std:::begin(UL2), UL2,size() - 1 };          // numerical flux left state
		const std::valarray<double> FR1 = UR2; //{ std::next(std::begin(UL2)), UL2.size() }; // numerical flux left state

		// Flux (physical) for the St-Venant equation (we don't consider pressure term)
		const auto FL2 = vsc19::StVenant1D_Incomplete_Flux(UL1, UL2);
		const auto FR2 = vsc19::StVenant1D_Incomplete_Flux(UR1, UR2);

		const auto uL = UL2 / UL1;
		const auto uR = UR2 / UR1;

		// unit width
		const auto CL = std::sqrt(vsc19::cGravity<double> * UL1);
		const auto CR = std::sqrt(vsc19::cGravity<double> * UR1);

		// ***************************CHECK!!!!! NEED TO BE FIX!!
		// Note: we have to check the sign of these two formulas,
		// I suspect there is a wrong sign (double check confirm there are sign errors)
		// 08/02/07 - these formulas are not correct, sign error
		// NOTE i have the article for this algorithm with all formulas
		// i will double check and make the fix if any hs to be done

		// �tape interm�diaire: calcul de US et CS
		const auto CS = 0.5 * (CL + CR) - 0.25 * (uL - uR); // sign error
		const auto uS = 0.5 * (uL - uR) + CL - CR;			// error

		//
		// Numerical flux at the interface j+1/2
		//

        std::map<size_t,std::pair<double,double>> w_mapface2FF;

		for( size_t i = 0; i < vsc19::DIM::value/*global faces*/; i++)
		{
			// shock speed for the left and right state
			const auto SL = std::min(uL[i] - CL[i], uS[i] - CS[i]);
			const auto SR = std::max(uR[i] + CR[i], uS[i] + CS[i]);

			//	Compute the interface flux Fi+1/2 (solve the Riemann problem)
			if (SL > 0.)
			{
				//	aFF1[i] = FL1[i];
				//	aFF2[i] = FL2[i];
				w_mapface2FF.try_emplace(i, std::make_pair(FL1[i], FL2[i]));
			}
			else if (SR < 0.)
			{
				// aFF1[i] = FR1[i];
				// aFF2[i] = FR2[i];
				w_mapface2FF.try_emplace(i, std::make_pair(FR1[i], FR2[i]));
			}
			else
			{
				auto FF1 = (SR * FL1[i] - SL * FR1[i] + SL * SR * (UR1[i] - UL1[i])) / (SR - SL);
				auto FF2 = (SR * FL2[i] - SL * FR2[i] + SL * SR * (UR2[i] - UL2[i])) / (SR - SL);
				w_mapface2FF.try_emplace(i, std::make_pair(FF1, FF2));
			}
			// add it to the map to be returned (not syre about this one)
            //w_mapface2FF.try_emplace(0, {0, 0}); // since key 0 exist 
		}//for-loop	

		return w_mapface2FF;
	}
#if 0
template<typename NumArray>
	void HLLFluxAlgorithmPtr2Func( NumArray& aFF1,NumArray& aFF2, 
		const NumArray& aU1, const NumArray& aU2)
	{
		// Numerical Flux basic algorithm (support legacy code pointer-to-function)
		CalculFF w_calculFF = Simulation::instance().getAlgorithm(std::string_view{"HLL_Scheme"});
		assert(nullptr != w_calculFF);

		// std::invoke wrapper functions C++17 !!!! contraint on calling pointer-to-function legacy code
		if constexpr (std::is_invocable<CalculFF, decltype(w_FF1), decltype(w_FF2), decltype(w_U1), decltype(w_U2)>())
		{
			// (sanity check physics algorithm)
			// compile-time check algorithm return value using trait and utility
			//   if constexpr ( std::is_same_v<std::invoke_result_t<CalculFF, decltype(w_FF1), decltype(w_FF2), decltype(w_U1), decltype(w_U2)>,void>)
			//   {
			// We compute the F_i+1/2 interface flux (legacy code call pointer-to-function)
			// make sure that we make the right call
			std::invoke(w_calculFF, aFF1, aFF2, aU1, aU2);
			//   }
			// w_calculFF(w_FF1, w_FF2, w_U1, w_U2);
		}
	}
#endif
} // End of namespace