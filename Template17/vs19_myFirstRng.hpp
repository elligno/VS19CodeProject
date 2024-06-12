#pragma once

// C++ include
#include <ranges>   // C++20 features

namespace vsc19 
{
	/**
	 * @brief Range type (pair of iterator support begin/end) 
	 * @tparam T type of range
	*/
	template <typename T>
	struct range_t : private T {
		using T::begin, T::end; /*...*/

		range_t() = default;
		range_t(T::iterator begRng, T::iterator endRng) : m_beg{ begRng }, m_end{ endRng } {}

		typename T::iterator m_beg;
		typename T::iterator m_end;
	};
} // End of namespace
