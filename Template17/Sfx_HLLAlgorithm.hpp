#pragma once

// C++ include
#include <cassert>
// STL container
#include <valarray>
#include <vector>
// STL algorithm
#include <algorithm>
#include <numeric>

// ...
#include "../vs19_UniversalConst.hpp"

using float64 = double;

using namespace std;

// alias
using ArrayType = valarray<double>;

#if 1
// Alias template (ArrayType<>)
template<typename T = float64>
using NumArrayType = valarray<T>;

#endif // 1

/**
 * @brief Compute the physical flux of ST-Venant equation
 * @tparam container type
 * @param U1 range of values
 * @param U2 range of values
 * @return physical flux at grid points
*/
template<typename Range>
decltype(auto) StVenant1D_Incomplete_Flux( Range& aU1, Range& aU2)
{
	static_assert( !std::empty(aU1) && !std::empty(aU2), "Incomplete Flux: range empty");
	static_assert( std::size(aU1) == std::size(aU2),     "Incomplete Flux: range not equal size");

	return (aU2 * aU2) / aU1;
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
	template<typename F, typename NumArrayType,
	typename = std::enable_if<std::is_same_v<NumArrayType,std::valarray<double>>>>
	std::pair<NumArrayType,NumArrayType> // flux vector components (j+1/2)
		HLLFluxAlgorithm( const NumArrayType& aU1, const NumArrayType& aU2, F&& aMinModSlopeLimiter)    // state variables components (j)
	{
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
		static_assert( std::size(aU1) == Sfx::EMCNEILNbSections::value);
		static_assert( std::size(aU2) == Sfx::EMCNEILNbSections::value);

		//
		// Reconstruction process at the cell interface x_j+1/2
		// 

		// Need documentation abpout each step of the algorithm 
		NumArrayType dU1( std::size(aU1));
		NumArrayType dU2( std::size(aU2));

		// compute gradient over each cell
		std::adjacent_difference( std::cbegin(aU1), std::cend(aU1), std::begin(dU1));
		std::adjacent_difference( std::cbegin(aU2), std::cend(aU2), std::begin(dU2));
		dU1[0] = 0.; // force first element to 0 (adjacent_difference keep first element unchanged)
		dU2[0] = 0.; // force first element to 0 (adjacent_difference keep first element unchanged)
		// apply slope limiter function for each gradient (minimum slope)
		std::adjacent_difference( std::cbegin(dU1), std::cend(dU1),  // range to apply limiter
			std::begin(dU1), // result 
			std::forward<F>(aMinModSlopeLimiter));    // slope limiter function 
		std::adjacent_difference( std::cbegin(dU2), std::cend(dU2),  // range to apply limiter 
			std::begin(dU2), // result 
			std::forward<F>(aMinModSlopeLimiter));   // slope limiter function 
		
		auto w_dU1Shifted = dU1.shift(1);  // circular shift (shifted element at the end with element T{})  
		auto w_dU2Shifted = dU2.shift(1);  // circular shift (shifted element at the end with element T{})
		
		// last global discretization node (ghost node) overwrite
		w_dU1Shifted[aU1.size() - 1] = aMinModSlopeLimiter(0., aU1[aU1.size() - 1] - aU1[aU1.size() - 2]);  // i+1/2
		w_dU2Shifted[aU2.size() - 1] = aMinModSlopeLimiter(0., aU2[aU2.size() - 1] - aU2[aU2.size() - 2]);  // i+1/2

		// don't need anymore of shifted array, might as well to move it 
		w_dU1 = std::move(w_dU1Shifted); // move semantic supported by valarray
		w_dU2 = std::move(w_dU2Shifted);

		//	Calculs pr�alables: �valuation des �l�ments du vecteur dU
		// Boundary point: i = 0
		//dU1[0] = Sfx::minmod(aU1[1] - aU1[0], 0.);  // std::bind2nd(ptr_fun(minmod)  
		//dU2[0] = Sfx::minmod(aU2[1] - aU2[0], 0.);

		// we may have a problem here, index out of range when i=100
		// since aU1 has size is 101 (NbSections) [0,...,100] but a i=100
		// we loop for [i+1]=101 throw an exception out of range.
		// Fix for last index we compare with 0. and i<i < aU1.size()-1 (stop at 99)
		// loop through the number of section 
		// Note: loop shall start at 1 (index) zero has already been evaluated.
		//for (auto i = 1; i < aU1.size() - 1; i++) {  // added by Jean Belanger (15/01/07) 
		//	dU1[i] = Sfx::minmod(aU1[i + 1] - aU1[i], aU1[i] - aU1[i - 1]);  // i+1/2
		//	dU2[i] = Sfx::minmod(aU2[i + 1] - aU2[i], aU2[i] - aU2[i - 1]);  // i+1/2
		//}

		// boundary cnd (avoid index out of range)
		//dU1[aU1.size() - 1] = Sfx::minmod(0., aU1[aU1.size() - 1] - aU1[aU1.size() - 2]);  // i+1/2
		//dU2[aU2.size() - 1] = Sfx::minmod(0., aU2[aU2.size() - 1] - aU2[aU2.size() - 2]);  // i+1/2

		// NOTE 
		//  MUSCL (Monotone Upwind Scheme Conservation System Law) 
		//  Second-order linear extrapolation (polynomial reconstruction at the interface).
		//  Total Variation Diminishing (TVD) property guarantees convergence of such schemes
		//  Uses the Minmod limiter for the slope reconstruction
		//

		// Compute the right and left states: UR, UL, FR et FL
		const auto UL1 { std::begin(aU1 + 0.5 * dU1), std::size(aU1) -1 };
		const auto UL2 = aU2 + 0.5 * dU2;
		const auto UR1 = aU1 - 0.5 * dU1;
		const auto UR2 = aU2 - 0.5 * dU2;

		// Compute the flux vector
		//const auto FL1 = UL2[i];
		//const auto FR1 = UR2[i + 1];

		const decltype(UL2) FL1{ std::begin(UL2), UL2.size() - 1 };          // numerical flux first state variable at cell face j+1/2 left state
		const decltype(UR2) FR1{ std::next( std::begin(UR2)), UR2.size()-1 }; // numerical flux first state variable at cell face j+1/2 right state

		// Flux (physical) for the St-Venant equation (without pressure term)
		const auto FL2 = StVenant1D_Incomplete_Flux(UL1, UL2);
		const auto FR2 = StVenant1D_Incomplete_Flux(UR1, UR2);

		//const auto uL = UL2[i] / UL1[i];
		//const auto uR = UR2[i + 1] / UR1[i + 1];

		const auto uL = Ul2 / Ul1;
		const auto uR = UR2 / UR1;

		//const auto CR = ::sqrt(g * UR1[i + 1] / wT());
		//const auto CL = ::sqrt(g * UL1[i] / wT());

		const auto CR = ::sqrt( vsc19::cGravity<float64> * UR1);
		const auto CL = ::sqrt( vsc19::cGravity<float64> * UL1);

		// �tape interm�diaire: calcul de US et CS
		const auto CS = 0.5 * (CL + CR) - 0.25 * (uL - uR);
		const auto uS = 0.5 * (uL - uR) + CL - CR;

		// shock speed for the left and right state
		//const auto SL = std::min(uL - CL, uS - CS);
		const auto SR = std::max(uR + CR, uS + CS);

		// compute shock speed on left/right shock location (continuity equation)
		const auto dL = uL - CL; 
		const auto dS = uS - CS;
		std::valarray<double> SL{ dL.size(); } // left shock speed 
		std::transform( std::begin(dL), std::end(dL), std::begin(dS), // ranges
			std::begin(SL), // result
			[](double aVal1, double aVal2) -> double // lambda (anonymous function)
			{ return std::min(aVal1,aVal2); });

		const auto dR = uR + CR;
		//uS + CS

		// Implicitly we loop on the list of cell interfaces (x_j+1/2). 
		// Then we compute the interface flux at second-order.  
		for( auto i = 0; i < aU1.size() - 1; i++) {

			//
			// Reconstruction process at the cell interface x_j+1/2
			//

			// Compute the right and left states: UR, UL, FR et FL
			// Second-order linear extrapolation (polynomial reconstruction at the interface).
			// MUSCL is used.
			//const float64 UL1 = aU1[i]   + 0.5*dU1[i];
			//const float64 UR1 = aU1[i+1] - 0.5*dU1[i+1];
			//const float64 UL2 = aU2[i]   + 0.5*dU2[i];
			//const float64 UR2 = aU2[i+1] - 0.5*dU2[i+1];

			//
			// Compute of the flux components vector 
			//

			// Compute the flux vector
		/*	const auto FL1 = UL2[i];
			const auto FR1 = UR2[i+1];*/

			// Flux (physical) for the St-Venant equation (we don't consider pressure term)
			//const auto FL2 = hyd::EvaluationF2_I_1D(UL1[i], UL2[i]);
			//const auto FR2 = hyd::EvaluationF2_I_1D(UR1[i + 1], UR2[i + 1]);

			//
			// HLL algorithm
			//

			// Wave speed ..... i am not sure
			const auto uL = UL2[i] / UL1[i];
			const auto uR = UR2[i + 1] / UR1[i + 1];

			// ******************************NOTE
			//  using 'g' variable for gravity instead of the variable template,
			// because we have some very small difference comparing final result. 
			// Original algorithm use this variable, result are in perfect sync.
			// 
			// const auto wG = Sfx::cGravity<auto>; shall use this variable in  the future
			// define g is deprecated
			//float32 wG = Sfx::SimulationMgr::getSingleton().gravity();
			// i do not know celerity wave or something like that
			const auto CR = std::sqrt( vsc19::cGravity<float64> * UR1[i + 1]);
			const auto CL = std::sqrt( vsc19::cGravity<float64> * UL1[i]);

			//	Une attention particuli�re devra �tre ici port�e sur
			//	les signes (+/-) relativement au produit scalaire avec
			//	Si+1/2

			//	Calcul de SR et SL

			// ***************************CHECK 
			// Note: we have to check the sign of these two formulas, 
			// i suspect there is a wrong sign 
			// 08/02/07 - these formulas are not correct, sign error
			// NOTE i have the article for this algorithm with all formulas
			// i will double check and make the fix if any hs to be done

			// �tape interm�diaire: calcul de US et CS
			const auto CS = 0.5 * (CL + CR) - 0.25 * (uL - uR);
			const auto uS = 0.5 * (uL - uR) + CL - CR;

			// shock speed for the left and right state
			const auto SL = std::min(uL - CL, uS - CS);
			const auto SR = std::max(uR + CR, uS + CS);

			//
			// Numerical flux at the interface j+1/2
			//

			//	Compute the interface flux Fi+1/2 (solve the Riemann problem)
			if (SL > 0.) {
				//aFF1[i] = FL1;
				//aFF2[i] = FL2;
				return { FL1,FL2 };
			}
			else if (SR < 0.) {
				//aFF1[i] = FR1;
				//aFF2[i] = FR2;
				return { FR1, FR2 };
			}
			else {
				auto aFF1 = (SR * FL1 - SL * FR1 + SL * SR * (UR1 - UL1)) / (SR - SL);
				auto aFF2 = (SR * FL2 - SL * FR2 + SL * SR * (UR2 - UL2)) / (SR - SL);
			}
		}
	}
#if 0
	void HLL_Scheme( ArrayType& aFF1, ArrayType& aFF2,   // flux vector components (j+1/2)
		const ArrayType& aU1, const ArrayType& aU2)    // state variables components (j)
	{
		// Right and left state variables (each side of the cell interface x_j+1/2). 
		//float64 UR1, UL1, UR2, UL2;
		//float64 FR1, FL1, FR2, FL2;

		// loop variable
	//	uint32 i;

		// Sanity checks
	//	assert(aU1.size() == SimulationMgr::getSingleton().getNbSections());

		// Prototyping constant 
		assert( vsc19::EMCNEILNbSections::value == std::size(aU1));

		ArrayType dU1(aU1.size());
		ArrayType dU2(aU2.size());

		//
		// Reconstruction process at the cell interface x_j+1/2
		//

		//	Calculs pr�alables: �valuation des �l�ments du vecteur dU
		// Boundary point: i = 0
//		dU1[0] = Sfx::minmod(aU1[1] - aU1[0], 0.);  // std::bind2nd(ptr_fun(minmod)  
//		dU2[0] = Sfx::minmod(aU2[1] - aU2[0], 0.);

		// we may have a problem here, index out of range when i=100
		// since aU1 has size is 101 (NbSections) [0,...,100] but a i=100
		// we loop for [i+1]=101 throw an exception out of range.
		// Fix for last index we compare with 0. and i<i < aU1.size()-1 (stop at 99)
		// loop through the number of section 
		// Note: loop shall start at 1 (index) zero has already been evaluated.
		// for (auto i = 1; i < aU1.size() - 1; i++) {  // added by Jean Belanger (15/01/07) 
		// 	dU1[i] = Sfx::minmod(aU1[i + 1] - aU1[i], aU1[i] - aU1[i - 1]);  // i+1/2
		// 	dU2[i] = Sfx::minmod(aU2[i + 1] - aU2[i], aU2[i] - aU2[i - 1]);  // i+1/2
		// }

		// // boundary cnd (avoid index out of range)
		// dU1[aU1.size() - 1] = Sfx::minmod(0., aU1[aU1.size() - 1] - aU1[aU1.size() - 2]);  // i+1/2
		// dU2[aU2.size() - 1] = Sfx::minmod(0., aU2[aU2.size() - 1] - aU2[aU2.size() - 2]);  // i+1/2

		//
		// MUSCL reconstruction (high-order)
		//

		// Compute the right and left states: UR, UL, FR et FL
		// Second-order linear extrapolation (polynomial reconstruction 
		// at the cell interface).
		// MUSCL is used.
		const decltype(aU1) UL1{ std::begin(aU1 + 0.5 * dU1), std::size(aU1) - 1 };
		const decltype(aU2) UL2{ std::begin(aU2 + 0.5 * dU2), std::size(aU2) - 1 };       // = aU2 + 0.5 * dU2;
		const decltype(aU1) UR1{ std::next(std::begin(aU1 + 0.5 * dU1)), std::size(aU1) }; //= aU1 - 0.5 * dU1;
		const auto UR2 = aU2 - 0.5 * dU2;

		// Compute the flux vector
		//const auto FL1 = UL2[i];
		//const auto FR1 = UR2[i + 1];

		const decltype(UL2) FL1{ std::begin(UL2), UL2.size() - 1 };          // numerical flux left state
		const decltype(UR2) FR1{ std::next(std::begin(UL2)), UL2.size() }; // numerical flux left state

		// Flux (physical) for the St-Venant equation (we don't consider pressure term)
		const auto FL2 = StVenant1D_Incomplete_Flux(UL1, UL2);
		const auto FR2 = StVenant1D_Incomplete_Flux(UR1, UR2);

		const auto uL = UL2 / UL1;
		const auto uR = UR2 / UR1;

		// Width of the cross-section (unit width)
		//hyd::SectionWidth wT;
		//		const auto CL = ::sqrt(g * UL1 / wT());
			//	const auto CR = ::sqrt(g * UR1 / wT());

		// unit width
		auto gUL1 = ( vsc19::cGravity<float64> * UL1);
		std::for_each( std::begin(gUL1), std::end(gUL1), [] (double aVal) { return std::sqrt(aVal); });
		const auto CL = gUL1;
		auto gUR1 = ( vsc19::cGravity<float64> * UR1);
		std::for_each( std::begin(gUR1), std::end(gUR1), [] (double aVal) { return std::sqrt(aVal); });
		const auto CR = gUR1;

		// Implicitly we loop on the list of cell interfaces (x_j+1/2). 
		// Then we compute the interface flux at second-order.  
//		for( auto i = 0; i < aU1.size()-1; i++) {

			//
			// Reconstruction process at the cell interface x_j+1/2
			//

			// Compute the right and left states: UR, UL, FR et FL
			// Second-order linear extrapolation (polynomial reconstruction at the interface).
			// MUSCL is used.
			//const float64 UL1 = aU1[i]   + 0.5*dU1[i];
			//const float64 UR1 = aU1[i+1] - 0.5*dU1[i+1];
			//const float64 UL2 = aU2[i]   + 0.5*dU2[i];
			//const float64 UR2 = aU2[i+1] - 0.5*dU2[i+1];

			//
			// Compute of the flux components vector 
			//

			// Compute the flux vector
		/*	const auto FL1 = UL2[i];
			const auto FR1 = UR2[i+1];*/

			// Flux (physical) for the St-Venant equation (we don't consider pressure term)
			//const auto FL2 = hyd::EvaluationF2_I_1D( UL1[i], UL2[i]);
			//const auto FR2 = hyd::EvaluationF2_I_1D( UR1[i + 1], UR2[i+1]);

			//
			// HLL algorithm
			//

			// Wave speed ..... i am not sure
			//const auto uL = UL2[i]/UL1[i];
			//const auto uR = UR2[i + 1]/UR1[i + 1];

			// ******************************NOTE
			//  using 'g' variable for gravity instead of the variable template,
			// because we have some very small difference comparing final result. 
			// Original algorithm use this variable, result are in perfect sync.
			// 
			// const auto wG = Sfx::cGravity<auto>; shall use this variable in  the future
			// define g is deprecated
			//float32 wG = Sfx::SimulationMgr::getSingleton().gravity();
			// i do not know celerity wave or something like that
//			const auto CR = ::sqrt( g*UR1[i + 1]/wT());
//			const auto CL = ::sqrt( g*UL1[i]/wT());

			//	Une attention particuli�re devra �tre ici port�e sur
			//	les signes (+/-) relativement au produit scalaire avec
			//	Si+1/2

			//	Calcul de SR et SL

			// ***************************CHECK 
			// Note: we have to check the sign of these two formulas, 
			// i suspect there is a wrong sign 
			// 08/02/07 - these formulas are not correct, sign error
			// NOTE i have the article for this algorithm with all formulas
			// i will double check and make the fix if any hs to be done

			// �tape interm�diaire: calcul de US et CS
		const auto CS = 0.5 * (CL + CR) - 0.25 * (uL - uR);
		const auto uS = 0.5 * (uL - uR) + CL - CR;

		// shock speed for the left and right state
		const auto SL = std::min(uL - CL, uS - CS);
		const auto SR = std::max(uR + CR, uS + CS);

		//
		// Numerical flux at the interface j+1/2
		//

		//	Compute the interface flux Fi+1/2 (solve the Riemann problem)
		if (SL > 0.)
		{
			aFF1[i] = FL1;
			aFF2[i] = FL2;
		}
		else if (SR < 0.) {
			aFF1[i] = FR1;
			aFF2[i] = FR2;
		}
		else {
			aFF1[i] = (SR * FL1 - SL * FR1 + SL * SR * (UR1 - UL1)) / (SR - SL);
			aFF2[i] = (SR * FL2 - SL * FR2 + SL * SR * (UR2 - UL2)) / (SR - SL);
		}
	//}
	}
	#endif //0
} // End of namespace
