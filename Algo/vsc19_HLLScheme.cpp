// C++ includes
#include <cassert>
#include <valarray>
#include <ranges>

#include "../Classes/vs19_MinMod.hpp"
#include "../vs19_AppConstant.hpp"
#include "../vs19_UniversalConst.hpp"
#include "vs19_SweFluxEvaluation.hpp"

namespace vsc19
{
	void HLL_Scheme( std::valarray<double>& aFF1, std::valarray<double>& aFF2,   // flux vector components (j+1/2)
		const std::valarray<double>& aU1, const std::valarray<double>& aU2) noexcept    // state variables components (j)
	{
		// Prototyping constant
		assert(vsc19::EMCNEILNbSections::value == aU1.size());
		assert(aU1.size()==aU2.size());

		std::valarray<double> dU1(aU1.size());
		std::valarray<double> dU2(aU2.size());

		//
		// Reconstruction process at the cell interface x_j+1/2
		//

		vsc19::MinMod19<double> w_minmod{};
		
		//	Calculs pr�alables: �valuation des �l�ments du vecteur dU
		// Boundary point: i = 0
		dU1[0] = w_minmod(aU1[1] - aU1[0], 0.);
		dU2[0] = w_minmod(aU2[1] - aU2[0], 0.);
		
		// we may have a problem here, index out of range when i=100
		// since aU1 has size is 101 (NbSections) [0,...,100] but a i=100
		// we loop for [i+1]=101 throw an exception out of range.
		// Fix for last index we compare with 0. and i<i < aU1.size()-1 (stop at 99)
		// loop through the number of section
		// Note: loop shall start at 1 (index) zero has already been evaluated.
		for (auto i = 1; i < aU1.size() - 1; i++)
		{																 // added by Jean Belanger (15/01/07)
			dU1[i] = w_minmod(aU1[i + 1] - aU1[i], aU1[i] - aU1[i - 1]); // i+1/2
			dU2[i] = w_minmod(aU2[i + 1] - aU2[i], aU2[i] - aU2[i - 1]); // i+1/2
		}
		
		// boundary cnd (avoid index out of range)
		dU1[aU1.size() - 1] = w_minmod(0., aU1[aU1.size() - 1] - aU1[aU1.size() - 2]); // i+1/2
		dU2[aU2.size() - 1] = w_minmod(0., aU2[aU2.size() - 1] - aU2[aU2.size() - 2]); // i+1/2
		
		assert(aU1.size() == aU2.size());
		assert(aU1.size() == vsc19::EMCNEILNbSections::value);

		const std::valarray<double> UL1(std::begin(aU1 + 0.5 * dU1), aU1.size() - 1);
		const std::valarray<double> UL2{std::begin(aU2 + 0.5 * dU2), aU2.size() - 1};			 // = aU2 + 0.5 * dU2;
		const std::valarray<double> UR1{std::next(std::begin(aU1 + 0.5 * dU1)), aU1.size() - 1}; //= aU1 - 0.5 * dU1;
		const std::valarray<double> UR2{std::next(std::begin(aU2 - 0.5 * dU2)), aU1.size() - 1};

		// Compute the flux vector
		// const auto FL1 = UL2[i];
		// const auto FR1 = UR2[i + 1];
		// Question: do I need to add const before decltype or decltype will deduce the const?
		const std::valarray<double> FL1 = UL2; //{ std:::begin(UL2), UL2,size() - 1 };          // numerical flux left state
		const std::valarray<double> FR1 = UR2; //{ std::next(std::begin(UL2)), UL2.size() }; // numerical flux left state

		// Flux (physical) for the St-Venant equation (we don't consider pressure term)
		auto FL2 = vsc19::StVenant1D_Incomplete_Flux(UL1, UL2);
		auto FR2 = vsc19::StVenant1D_Incomplete_Flux(UR1, UR2);

		auto uL = UL2 / UL1;
		auto uR = UR2 / UR1;

		// Width of the cross-section (unit width)
		// hyd::SectionWidth wT;
		//		const auto CL = ::sqrt(g * UL1 / wT());
		//	const auto CR = ::sqrt(g * UR1 / wT());

		// unit width
		auto CL = std::sqrt(vsc19::cGravity<double> * UL1);
		auto CR = std::sqrt(vsc19::cGravity<double> * UR1);

		// ***************************CHECK
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

		for (size_t i = 0; i < vsc19::DIM::value; i++)
		{
			// shock speed for the left and right state
			const auto SL = std::min(uL[i] - CL[i], uS[i] - CS[i]);
			const auto SR = std::max(uR[i] + CR[i], uS[i] + CS[i]);

			//	Compute the interface flux Fi+1/2 (solve the Riemann problem)
			if (SL > 0.)
			{
				aFF1[i] = FL1[i];
				aFF2[i] = FL2[i];
			}
			else if (SR < 0.)
			{
				aFF1[i] = FR1[i];
				aFF2[i] = FR2[i];
			}
			else
			{
				aFF1[i] = (SR * FL1[i] - SL * FR1[i] + SL * SR * (UR1[i] - UL1[i])) / (SR - SL);
				aFF2[i] = (SR * FL2[i] - SL * FR2[i] + SL * SR * (UR2[i] - UL2[i])) / (SR - SL);
			}
		}
	}

} // namespace vsc19
