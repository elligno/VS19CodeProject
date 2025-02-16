// C++ includes
#include <cassert>
#include <ranges> // C++20 include
// App include
#include "../vs19_AppConstant.hpp"

namespace vsc19 
{
    // Usage
    //   create a view with All() and pass it to this function
	template<std::ranges::view Rng> // Range of values 
    requires std::ranges::input_range<Rng>
	auto DerivativeBiasUpwind( Rng aRng) // pass by-value (view cheap to copy/move)
	{
        using value_type = std::ranges::range_value_t<Rng>;
       // using diff_t = std::ranges::range_difference_t<Rng>;

        assert(EMCNEILNbSections::value == std::ranges::size(aRng));

        // create a vector initialized to zero, is it?
        std::vector<value_type> w_dU(std::ranges::distance(aRng));

        // adjacent_difference leave the first element as is, it shall not be considered
        // in the computation since it doesn't correspond to any gradient (container value)
        // 
        // compute the gradient (U_i - U_i-1) at first order
        std::adjacent_difference(std::ranges::cbegin(aRng), std::ranges::cend(aRng), w_dU.begin());
    
        // NOTE use 'move' cast function to avoid undefined behavior because returning 
        // a view by value (internally hold a reference to 'w_dU' (range) which is destroyed
        // at the end of the D1stBiasUpwind) result in undefined behavior.
        //      on local object (see "C++20 The Comple Guide") N. Josuttis 
        //   or return std::vector<float64>(w_dU.begin(),w_dU.end()) | std::ranges::drop(1);
        //  use a rvalue not a copy of the view
        //
        //  could do this: return std::vector<value_type>(std::next(w_dU.begin()), w_dU.end());
        return std::move(w_dU) | std::views::drop(1); 
    }
} // End of namespace