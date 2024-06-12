
#include <iostream>
#include <ranges>
#include <vector>

// Sfx type include
//#include "Sfx/Sfx_FieldLattice.h"
// Project includes
#include "../Algo/vs19_numScheme.hpp"
#include "../Template17/vs19_myFirstRng.hpp"


namespace vsc19 
{
  void testRng20Feature() 
  {
	  // C++20 range concept and compile-time if
	  // if with initialization C++17
	  if constexpr (vsc19::range_t<std::vector<int>> r; std::ranges::range<decltype(r)>)
	  {
		  std::cout << "r is a range\n";
	  }

	  // C++17 feature Class Template Argument Deduction (CTAD)
	  std::vector w_vInt{1, 2, 3, 4, 5, 6, 7, 8};
	  for (const auto elem : w_vInt | std::views::take(5))
	  {
		  std::cout << "Printing five elements\n";
	  }

	  // just testing
	  vsc19::range_t<std::vector<int>> w_myRng{std::ranges::next(w_vInt.begin(), 3),
											   std::ranges::prev(w_vInt.end(), -2)};

	  // range size
	  auto w_rngSiz = std::ranges::size(w_myRng);
	  auto w_begIter = w_myRng.begin();
  }

	void testNumSchemeCall()
	{
		// create a field lattice with values
		// hllFluxAlgorithmFwd(...) set of argumnents
		std::vector<double> w_dU1( EMCNEILNbSections::value);
		std::vector<double> w_dU2( EMCNEILNbSections::value);
		std::vector<double> w_FF1( EMCNEILNbSections::value); //out
		std::vector<double> w_FF2( EMCNEILNbSections::value); // out
		// just testing
		const std::vector<double> w_U1(10,1.);
		const std::vector<double> w_U2(10,0.);

		// all lvalue (perfect forwarding)
		hllFluxAlgorithmFwd( w_FF1, w_FF2, w_U1, w_U2);
	}
} // End of namspace