
#include <iostream>
#include <cstdlib>

// App include
#include "Classes/valarrField1D.h"
// BaseNumericalTypes package
#include "Sfx/Sfx_MthArray.h"

//
// Reference: https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference 
//

// forward call
namespace vsc19 { void testClasses();} 

// Main entry point
int main( int argc, char* argv[])
{
    std::cout << "Try to meke it work VSCode project\n";

    // numerical array for fast-floating point operation
    Sfx::MthArray w_testLink(5);
    assert( 5 == w_testLink.size());

    //d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
    // must be set to a valid range [1,N]
    vsc19::gridLattice1D w_grid1D{ std::string {"d=1 [0,1]x[1,10]"} };
    auto dx = w_grid1D.Delta();

    // Perfect forwarding test with lambda generic (template)
    // calling our piece of code to see if it works!!
    vsc19::testClasses();

    std::cout << "Seems to me this is working!!\n";

    return EXIT_SUCCESS;
}