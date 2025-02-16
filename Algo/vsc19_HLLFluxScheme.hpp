
#paragma once

#include <type_traits>

namespace vsc19 
{
    // Just experimenting some new features of Modern C++ 
    template<typename NumArray>
    class HLLFluxScheme( NumArray&& aU1, NumArray&& aU2)
    {
        if constexpr (std::is_rvalue_reference<decltype(aU1)>)
        {
            // this is an rvalue we can freely modify the arg 
        }
        else // 
        {}
    };
} // End of namespace 