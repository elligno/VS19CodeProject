
#pragma once

#include <cassert>
#include <ranges>

namespace vsc19 
{
	/**
	 * @brief
	 * @tparam Range
	 * @param U1
	 * @param U2
	 * @return
	 */
	//template <typename Range>
	auto StVenant1D_Incomplete_Flux( std::ranges::input_range auto aU1, 
	std::ranges::input_range auto aU2) noexcept(true)
	{
		assert(!std::ranges::empty(aU1) && !std::ranges::empty(aU2));
		assert(std::ranges::size(aU1) == std::ranges::size(aU2));

		return (aU2 * aU2) / aU1;
	}
} // End of namespace
