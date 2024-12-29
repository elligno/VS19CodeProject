#pragma once

namespace vsc19 
{
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
	using TESTSIZE5 = AppConstant<5>;
	using DIM = AppConstant<100>;
} // End of namespace
