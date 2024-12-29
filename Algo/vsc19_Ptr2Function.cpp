
#include <cassert>
#include <iostream>

namespace vsc19 
{
    // legacy code
    void EMcNeilCalculFF(double* aU1, double* aU2, double* aFF1, double* aFF2) 
    {
        std::cout << "Legacy code function implementation\n";
        assert(aU1==nullptr);
        assert(aU2==nullptr);
        std::cout << "Exit of function pointer EMcNeilCalculFF";
    }
} // End of namespace