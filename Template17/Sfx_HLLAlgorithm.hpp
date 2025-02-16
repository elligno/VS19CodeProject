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
#include "../vs19_AppConstant.hpp"

using float64 = double;

using namespace std;

// alias
using ArrayType = valarray<double>;

#if 1
// Alias template (ArrayType<>)
template<typename T = float64>
using NumArrayType = valarray<T>;

#endif // 1

// Algotirm description
// Computational domain extension [0,DIM-1] => DIM number of nodes
// Global domain exetension [0,N] include ghost node ()
// the  extra node is call "Ghost Node" since we have an
// open bondary at the right far end (last compuational node
// is not set by physical boundary condition, need to compute
// state variables which require to compute flux at cell face
// according to numerical algorithm.
// face gradient in the reconstruction process of state variables at cell face.
// Convention (left boundary is a tied node and right boundary is open)
//  Use global face index from 0,...,N-1 in this prototype means [0,99]
// First cell face is between first (i=0) and second node (i=1)
// not part of the computation, its value is set by physical b.c.
// Last cell face is after last node of the computational domain
// return list of numerical flux at those cell faces, pair of these
// can be used to represent "cell" and where
// if constexpr( std::is_same_v<NumArrayType, std::valarray<double>>)
//{
//	//SFINAE (Substitution Failure Is Not An Error)
//	// run this algortihm
//}
// else
//{
//	// do something else (vector or scalarField)
//	//HLL_Scheme(); E. McNeil original code
//}

/**
 * @brief Compute the physical flux of St-Venant equation (incomplete)
 * @tparam container type
 * @param U1 range of values
 * @param U2 range of values
 * @return physical flux at grid points
*/
template<typename Range> // pass const 
decltype(auto) StVenant1D_Incomplete_Flux( Range& aU1, Range& aU2)
{
	// NOTE (S. Meyers "Effective Modern C++", p.12) type deduction
	//  passing a const object to a template taking T& parameter is safe:
	//  the consteness of the object is part of the type deduced for T.
	static_assert( std::is_const_v<decltype(aU1)>, "Constness of the object");
	static_assert( !std::empty(aU1) && !std::empty(aU2), "Incomplete Flux: range empty");
	static_assert(  std::size(aU1) == std::size(aU2),  "Incomplete Flux: range not equal size");

	return (aU2 * aU2) / aU1;
}

namespace Sfx 
{
	//   We call such a scheme minmod - type.
	//   value in each cell is the cell average of the approximate solution after the evolution step.
	//   Reference:
	//       to be completed paper title and authors 
	//   https://people.tamu.edu/~popov//minmod2.pdf
	
	/**
	 * @brief HLL solver is a Riemann Approximate Solver (belongs to the family of Godunov-type scheme)
	 *        Godunov scheme is based on the solution of the Riemann problem in which two states (U_l, U_r) 
	 *        are separated by a discontinuity, defining a Riemann problem.
	 *        Second order Godunov-type schemes based on the Minmod limiter. Namely, the piecewise linear
	 *        projection in such schemes is based on cell averages and uses the Minmod limiter for the slope 
	 *        reconstruction.
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
		// Sanity checks
		static_assert( std::size(aU1) == vsc19::EMCNEILNbSections::value);
		static_assert( std::size(aU2) == vsc19::EMCNEILNbSections::value);

		//
		// Reconstruction process at the cell interface x_j+1/2
		// 

		// NOTE 
		//  MUSCL (Monotone Upwind Scheme Conservation System Law) 
		//  Second-order linear extrapolation (polynomial reconstruction at the interface).
		//  Total Variation Diminishing (TVD) property guarantees convergence of such schemes
		//  Uses the Minmod limiter for the slope reconstruction
		//

        // compute gradient over each cell by applying slope limiter function
        auto [dU1,dU2] = computeDU(aU1, aU2, std::forward<F>(aSlopeLimiter)); // gradient over each cell

		//
		// Reconstruction process at the cell interface x_j+1/2
		// 

        // Compute the right and left states: UR, UL, FR et FL (computational domain)
		const NumArrayType UL1{ std::begin(aU1 + 0.5 * dU1), Sfx::DIM::value};  // j
		const NumArrayType UL2{ std::begin(aU2 + 0.5 * dU2), Sfx::DIM::value};  // j+1
        const NumArrayType UR1{ std::next(std::begin(aU1 - 0.5 * dU1)), Sfx::DIM::value}; 
		const NumArrayType UR2{ std::next(std::begin(aU2 - 0.5 * dU2)), Sfx::DIM::value};
	
		// Compute the flux vector
		//const auto FL1 = UL2[i];
		//const auto FR1 = UR2[i + 1];
		const auto FL1 = UL2; // numerical flux first state variable at cell face j+1/2 left state
		const auto FR1 = UR2; // numerical flux first state variable at cell face j+1/2 right state

        // NOTE see scott Meyer's book "Effective Modern C++" passing a 'const'
		// Flux (physical) for the St-Venant equation (without pressure term)
		const auto FL2 = StVenant1D_Incomplete_Flux(UL1, UL2);
		const auto FR2 = StVenant1D_Incomplete_Flux(UR1, UR2);

		//const auto uL = UL2[i] / UL1[i];
		//const auto uR = UR2[i + 1] / UR1[i + 1];
		const auto uL = UL2 / UL1;
		const auto uR = UR2 / UR1;

		//const auto CR = ::sqrt(g * UR1[i + 1] / wT());
		//const auto CL = ::sqrt(g * UL1[i] / wT());
		const auto CR = std::sqrt( vsc19::cGravity<float64> * UR1); // section width is unity
		const auto CL = std::sqrt( vsc19::cGravity<float64> * UL1); // ditto

		// intermediate step: calcul de US et CS
		// NOTE these formulas are wrong, sign error for each 
		const auto CS = 0.5 * (CL + CR) - 0.25 * (uL - uR);
		const auto uS = 0.5 * (uL - uR) + CL - CR;

		// shock speed for the left and right state
		//const auto SL = std::min(uL - CL, uS - CS);
		//const auto SR = std::max(uR + CR, uS + CS);

		// compute shock speed on left/right shock location (continuity equation)
		const auto dL = uL - CL; 
		const auto dSm = uS - CS;
		std::valarray<double> SL{ dL.size() }; // left shock speed 
		std::transform( std::begin(dL), std::end(dL), // first range
		    std::begin(dSm), // second range
			std::begin(SL), // result
			[](double aVal1, double aVal2) -> double // lambda (anonymous function)
			{ return std::min(aVal1,aVal2); });

		const auto dR = uR + CR;
		const auto dSp = uS + CS;
		std::valarray<double> SR{ dR.size() }; // right shock speed 
		std::transform( std::begin(dR), std::end(dR),  // first range
		    std::begin(dSp), // second range 
			std::begin(SR), // result
			[](double aVal1, double aVal2) -> double // lambda (anonymous function)
			{return std::max(aVal1,aVal2);});

#if 0

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
#endif
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

#if 0
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
//	}
	#endif //0
	} // End of namespace
