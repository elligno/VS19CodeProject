
#pragma once

namespace vsc19 
{
	/**
 * @brief
 * @tparam Range
 * @param U1
 * @param U2
 * @return
*/
	template<typename Range>
	auto StVenant1D_Incomplete_Flux( Range& aU1, Range& aU2) noexcept(true)
	{
		static_assert( !std::empty(aU1) && !std::empty(aU2), "Incomplete Flux: range empty");
		static_assert(  std::size(aU1) == std::size(aU2),     "Incomplete Flux: range not equal size");

		return (aU2 * aU2) / aU1;
	}

} // End of namespace
