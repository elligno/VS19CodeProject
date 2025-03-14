
#include <iostream>
#include <ranges>
#include <vector>

// Project includes
#include "../Template17/vsc19_PerfectFwdAlgo.hpp"

namespace vsc19 
{
	void testNumSchemeCall()
	{
		// create a field lattice with values
		// hllFluxAlgorithmFwd(...) set of argumnents
		std::vector<double> w_dU1(EMCNEILNbSections::value);
		std::vector<double> w_dU2(EMCNEILNbSections::value);
		std::vector<double> w_FF1(EMCNEILNbSections::value); // out
		std::vector<double> w_FF2(EMCNEILNbSections::value); // out
		// just testing
		const std::vector<double> w_U1(10, 1.);
		const std::vector<double> w_U2(10, 0.);

		// all lvalue (perfect forwarding)
		hllFluxAlgorithmFwd(w_FF1, w_FF2, w_U1, w_U2);
	}
} // End of namspace