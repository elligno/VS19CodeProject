
#include <cassert>
#include <iostream>

namespace vsc19 
{
    // legacy code support
    void EMcNeilCalculFF(double* aU1, double* aU2, double* aFF1, double* aFF2) noexcept
    {
        std::cout << "Legacy code function implementation\n";
        assert(aU1!=nullptr);
        assert(aU2!=nullptr);

        auto w_U1L = aU1[0];
        auto w_U2L = aU2[0];
        
        std::cout << "Exit of function pointer EMcNeilCalculFF";
    }
} // End of namespace