#pragma once

#include <type_traits> // enable_if

#include <cmath>
#include <cstdlib>

namespace vsc19 
{
	// Alias template
	template<typename T>
	using EnableFloatingPoint = std::enable_if_t<std::is_floating_point_v<T>>;

	template<typename T>
	concept dbl_arg_type = std::is_floating_point_v<T>;

  /**
	 * @brief Minmod limiter function for slope limiting gradient evaluation
	 * @param first value to compare with
	 * @param second value to compare with
	 * @return result of the comparison (in documentation about minmod limiter function)
	*/
  template<typename T>
    requires dbl_arg_type<T>
  class MinMod
  {
	public:
		// Don't remember exactly what for? return type
		// something has to do with decay!! need to check
        constexpr auto operator() ( T aVal1, T aVal2)
        {
			if constexpr( aVal1 * aVal2 <= 0.)
				return 0.;

			else if constexpr( (std::abs(aVal1) < std::abs(aVal2)) && (aVal1*aVal2 > 0.))
				return aVal1;

			else if constexpr( (std::abs(aVal2) < std::abs(aVal1)) && (aVal1*aVal2 > 0.))
				return aVal2;

			else if constexpr( std::abs(aVal2) == std::abs(aVal1))		//	ATTENTION, mis pour v�rification !!!! sanity check!!
				return aVal1;
			else
			{
				// NO!!! hard to debug
				std::cerr << "Fonction MINMOD: situation ne correspondant pas a celle attendue\n";
				exit(EXIT_FAILURE); // what else to do?
			}
        }
  };
} // End f namespace