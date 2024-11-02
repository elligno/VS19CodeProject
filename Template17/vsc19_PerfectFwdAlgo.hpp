#pragma once

// C++ 20 range library
#include <ranges>

namespace vsc19 
{
    /** */
    template<typename StdVec>
    void TreatmentSrcTerms( StdVec& aS, const StdVec& Q, const StdVec &A, 
       const StdVec &n, double dx, unsigned aNbSections)
    {
        std::cout << "Testing perfect forwarding lvalue\n";

        // ...
        
        // compute derivative by central second-order scheme (Taylor expansion)
        // FiniteDifferenceDerivative w_dxH2nd{ std::string{ "H-Field" }, false }; 
        // w_dxH2nd.setDerivativeOrder(2);  // 2nd order
        // auto w_dxHvarray = w_dxH2nd.d1x(w_Hfield); // derivative of a scalar field

        // ...
    }

    // 
    /** State variables ... possible implementation*/
    template<typename StdVec/*, auto NbSections=101*/> // default value
    void TreatmentSrcTerms( StdVec& aS, StdVec &&Q, StdVec &&A, const StdVec &n, 
        double dx, unsigned aNbSections)
    {
        // error with constexpr, don't understand, size return constexpr!!!
        assert( std::size(Q) == aNbSections);
         
        // check if it is a range (C++20 concept)
        if constexpr (std::ranges::range<decltype(Q)>)
        {
            // if push_back is supported can i do this?
            if ( std::is_same_v<StdVec::value_type, double>)
            {
                // Can freely modify the original (forward reference)
                // move semantic (avoid a copy which can be costly)
                Q.push_back(1.3); //b.c. condition at right far end
                // sanity check
                assert( EMCNEILNbSections::value == std::size(Q));
            }
        }

        std::cout << "Testing perfect forwarding rvalue\n";
    }

    // testing perfect forwarding to be use in algorithm development
    template <typename... Args>
    void call2SrcrTeatmentAlgo(Args&&...args) // forward reference as argument
    {
        // generic lambda (C++20) perfect forwarding with lambda
        auto callSrcAlgorithm = []<typename... Args>(Args&& ...args)
        {
            // don't need to use decltype with std::forward
            // since we have a template (not auto&& -> Universal reference)
            // keep valueness of a type (perfect forwarding)
            TreatmentSrcTerms(std::forward<Args>(args)...);
        };

        // done at compile time!!!
        if constexpr (sizeof...(args) > 0)
        {
            callSrcAlgorithm(args...);
        }

        std::cout << "Exit from test perfect forwarding scientific algo\n";
    }

    /** C++20 template function with universal/forward reference*/ 
    void fwdSrcAlgo(auto&&... aFwdAlgo)
    {
         // done at compile time!!!
        if constexpr (sizeof...(aFwdAlgo) > 0)
        {
            // calling corresponding function according 
            // to value categorie of each argument
           TreatmentSrcTerms(std::forward<decltype(aFwdAlgo)>(aFwdAlgo)...);
        }
    }
} //End of namespace