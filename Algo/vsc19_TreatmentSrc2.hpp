
#pragma once

#include <iostream>
#include <iterator>
#include <ranges> // C++20
// numeric
#include <numeric>

namespace vsc19
{
    void srcTreatmentTpl
    {
       public:
       // not sure about this implementation
       template<std::input_iterator BegRng, std::input_iterator EndRng>
       void treatmentSrc2( BegRng aBeg, EndRng aEnd, double aDx)
       {
        std::cout << "Testing Generic algo\n";
        //std::adjacent_difference();        
       } 
    }
} // namespace vsc19
