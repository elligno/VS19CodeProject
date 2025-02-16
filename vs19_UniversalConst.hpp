
#pragma once

	// math includes
#include <numbers>    // pi and others since C++20

namespace vsc19 
{
		//------------------------------------------------------------
		//                    Math Utilities
		//------------------------------------------------------------

		/**
		 * @brief Gravity acceleration constant (m/s^2)
		 * @tparam T float number precision
		*/
		template<typename T> constexpr T cGravity{ 9.80665 };

		/**
		 * @brief Global constant for pi and a few multiples.
		 * @tparam T floating number precision
		*/
		//template<typename T> constexpr T cPi { 3.1415923456 };
		template<typename T> constexpr T cPi{ std::numbers::pi };
		template<typename T> constexpr T c2Pi{ cPi<T> *2. };
		template<typename T> constexpr T cPiOver2{ cPi<T> / 2. };
		template<typename T> constexpr T c1OverPi{ 1. / cPi<T> };
		template<typename T> constexpr T c1Over2Pi{ 1. / c2Pi<T> };
		template<typename T> constexpr T cPiOver180{ cPi<T> / 180. };
		template<typename T> constexpr T c180OverPi{ 180. / cPi<T> };
		// not sure if we keep it!!
	//	template<typename T = double> constexpr T gPi{ 4. * ::atan(1.) };

		/**
		 * @brief variable template
		 * @tparam T precision type
		 */
		template <typename T = long double>
		constexpr T gPi = {3.14159265358979};
} // End of namespace