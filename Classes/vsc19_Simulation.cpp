
#include "vsc19_Simulation.hpp"

namespace vsc19
{
    //
	// Externals functions (computation of the numerical flux) 
	//

	/**
	 * @brief Harten-Lax-Leer Algorithm ( belongs to Approximate Riemann Solver family)
	 * @param aFF1 Numerical flux at cell face j+1/2 (first state variable)
	 * @param aFF2 Numerical flux at cell face j+1/2 (second state variable)
	 * @param aU1  First state variable at global nodes
	 * @param aU2  Second state variable at global nodes
	*/
	extern void HLL_Scheme( std::valarray<double>& aFF1, std::valarray<double>& aFF2, 
        const std::valarray<double>& aU1, const std::valarray<double>& aU2) noexcept;
} // namespace vsc19

namespace vsc19 
{
    vsc19::Simulation::CalculFF Simulation::getFluxAlgorithm( std::string_view aAlgoName) const 
	{
        // not sure about this one
        for (auto &&[flxalEgoname, ptr2ff] : m_listPtr2Func)
        {
            /* compare name */
            if (flxalEgoname == aAlgoName) // is that make sence??
            {
                return ptr2ff;
            }
            else
                continue;
        }

        return nullptr;
        //	return m_fluxAlgo;
    }

    bool Simulation::insert(CalculFF aCalFlux) 
	{
        if( auto [posIter, succeedOk] = m_listPtr2Func.insert( {std::string_view{"HLLScheme"}, aCalFlux}); !succeedOk)
        {
            const auto& [key,val] = *posIter; 
            std::cerr << "Couldn't insert flux algo in simulation bean\n";
            return false;
        }
        return true;
    }
} // End of namespace