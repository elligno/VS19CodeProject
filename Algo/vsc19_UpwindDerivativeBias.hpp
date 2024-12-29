
#include "../vs19_AppConstant.hpp"

namespace vsc19 
{
    // Usage
    //   create a view with All() and pass it to this function
    //
  	// We are testing an implementation of the upwind scheme 
	// using generic programming (STL). It will be part of the 
	// numerical library. 
    //  return decltype auto, not sure its what we want
	template<std::ranges::input_range Rng> // Range of values 
	auto Upwind1stDerivatr( Rng aRng) // pas by-value
	{
        static_assert(EMCNEILNbSections::value == std::ranges::size(aRng));

        // Design Note
        //  Special care should be taken about these typename, i'm not
        //  sure if it is required in C++17, need to check.
        // typedef typename
        //	std::iterator_traits<Range::iterator>::value_type value_type;

        using value_type = std::ranges::range_value_t<Rng>;

        // typedef typename
        //	std::iterator_traits<Range::iterator>::difference_type diff_t;

        using diff_t = std::ranges::range_difference_t<Rng>;

        // diff_t w_Length = std::distance( std::begin(aRng), std::end(aRng));
        // const auto w_Length = std::distance(std::begin(aRng), std::end(aRng));
        const auto w_Length = std::ranges::distance(aRng);

        // create a vector of w_length initialized to zero, is it?
        std::vector<value_type> w_dU(w_Length);

        // NOTE:
        // adjacent_difference leave the first element as is, it shall not be considered
        // in the computation since it doesn't correspond to any gradient (container value)
        // In our case, i do think we use it exclusively for flux derivative F = F_i - F_i-1
        // cell face ... to be completed
        // compute the gradient (U_i - U_i-1) at first ordewr
        std::adjacenwt_difference(std::ranges::begin(aRng), std::ranges::end(aRng), w_dU.begin());
        assert(w_dU.size() == w_Length); // really need it?w

        // 		w_dU.pop_fronwt();  // remove first element, not part of the result
        // 		assert( w_dU.size()==w_Length-1);
        return std::move(w_dU) | std::ranges::drop(1); 
        
        // return value optimization
        //return std::vector<value_type>(w_dU.begin(), w_dU.end());
        // return w_dU;
    }
} // End of namespace