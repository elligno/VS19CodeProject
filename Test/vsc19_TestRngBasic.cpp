#pragma once

#include <ranges>
#include "../Classes/vs19_valArrField.h"

namespace vsc19 
{
    template<typename Cont>
    void TestRngBasic() 
    {
        // d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
        //  must be set to a valid range [1,N]
        //  vsc19::gridLattice1D w_grid1D{std::string {"d=1 [0,1]x[1:10]"} };
        //   const auto dx = w_grid1D.Delta();

        // auto w_gr1d = std::make_shared<vsc19::gridLattice1D>(std::string {"d=1 [0,1]x[1:10]"});
        //  vsc19::scalarField1D w_fieldLattice{w_gr1d, std::string{"Field Lattice"}};

        vsc19::valArrField::gridlatticeptr w_aa{new vsc19::GridLattice{}};
        vsc19::valArrField w_newField{w_aa, std::string{"dwerewr"}};

        //  std::views::counted w_fieldVws(w_newField.cbegin()+1, 5);

        // C++20 range concept and compile-time if (range is a concept of "is iterable")
        // if with initialization C++17 
        if constexpr (vsc19::valArrField w_newField{w_aa, std::string{"Swe"}};
                      std::ranges::range<decltype(w_newField)>)
        {
            std::cout << "scalar field is a range\n";

            namespace rng = std::ranges; //make code cleaner

            auto begRng = rng::cbegin(w_newField); // range utility
            auto endRng = rng::cend(w_newField);   // ditto

            // assert()
            auto checkSiz = rng::size(w_newField);
            // create a view (subrange)
            auto w_vws = 
               std::views::all( std::ranges::next(w_newField.cbegin()) , std::ranges::prev(w_newField.cend()));

            // could we use views to print some field values?
            // C++17 feature Class Template Argument Deduction (CTAD)
            for (const auto &elem : w_newField | std::views::take(5))
            {
                std::cout << "Printing five elements\n";
            }
        }
        else
        {
            // use default container algorithm
            Cont w_cont; w_cont.reserve(101);
        }
    }
} // End of namespace