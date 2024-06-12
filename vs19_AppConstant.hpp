#pragma once

namespace vsc19 
{
	/**
	 * @brief Gravity acceleration constant (m/s^2)
	 * @tparam T float number precision
	*/
	template<typename T> constexpr T cGravity{ 9.80665 };

	/**
	 * @brief Helper to define some application constant
	 * @tparam T constant type
	*/
	template<auto v>
	struct AppConstant {
		static constexpr auto value = v; // inline variable and a definition since C++17
	};

	/**
	*   @brief E. McNeil constant for the number of sections (scenario)
	*/
	using EMCNEILNbSections = AppConstant<101>;
} // End of namespace
