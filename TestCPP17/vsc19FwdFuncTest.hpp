
#pragma once

// function to forward ()
#include "vs19_NumericalSourceTerms.h"

namespace vsc19 
{
    // Just testing perfect forwarding with different signature
    template<typename... Args>
    void FwdFuncTest(Args ...args)
    {
        // perfect forwarding (testing )
        TreatmentSource2Tpl(std::forward<Args>(args)...);
    }
} // End of namespace