
#pragma once

#include <cassert>
#include <ranges>

namespace vsc19 
{
	/**
      * @brief Compute the physical flux of One-Dimensional St-Venant equation (incomplete)
      * @tparam container type
      * @param U1 range of values
      * @param U2 range of values
      * @return physical flux at grid points
    */
    template<typename Range>
    requires std::same_as<Range,std::valarray<double>>
	auto StVenant1D_Incomplete_Flux( const Range& aU1, const Range& aU2) noexcept(true)
	{
		// assert(!std::ranges::empty(aU1) && !std::ranges::empty(aU2));
		// assert(std::ranges::size(aU1) == std::ranges::size(aU2));
		assert( !std::ranges::empty(aU1) && !std::ranges::empty(aU2));
		assert(  std::ranges::size(aU1) == std::ranges::size(aU2));
	
		return (aU2 * aU2) / aU1;
	}

#if 0
	/**
 * @brief Compute the physical flux of St-Venant equation (incomplete)
 * @tparam container type
 * @param U1 range of values
 * @param U2 range of values
 * @return physical flux at grid points
*/
template<typename Range> // pass const 
decltype(auto) StVenant1D_Incomplete_Flux( Range& aU1, Range& aU2)
{
	// NOTE (S. Meyers "Effective Modern C++", p.12) type deduction
	//  passing a const object to a template taking T& parameter is safe:
	//  the consteness of the object is part of the type deduced for T.
	static_assert( std::is_const_v<decltype(aU1)>, "Constness of the object");
	static_assert( !std::ranges::empty(aU1) && !std::ranges::empty(aU2), "Incomplete Flux: range empty");
	static_assert(  std::ranges::size(aU1) == std::ranges::size(aU2),  "Incomplete Flux: range not equal size");

	return (aU2 * aU2) / aU1;
}
#endif
} // End of namespace
