
#include <iostream>
#include <cstdlib>

// App include
#include "Classes/vs19_valArrField.h"
#include "Classes/valarrField1D.h"
#include "Classes/Types2Test.h"
// BaseNumericalTypes package
#include "Sfx/Sfx_MthArray.h"

//
// Reference: https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference 
//

// forward call
namespace vsc19 { 
    void testClasses();
    void ExamplOfUseRval();
    } 

// Main entry point
int main( int argc, char* argv[])
{
    std::cout << "Try to meke it work VSCode project\n";

    // some test with rvalue reference (check overload resolution)
 //   vsc19::ExamplOfUseRval();

    // numerical array for fast-floating point operation
 //   Sfx::MthArray w_testLink(5);
   // assert( 5 == w_testLink.size());

    // Test One-dimensional scalar field
    //d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
    // must be set to a valid range [1,N]
    vsc19::gridLattice1D w_grid1D{ std::string {"d=1 [0,1] [1:100]"} };
    const auto dx = w_grid1D.Delta();
    // some basic check
    const auto w_xmin = w_grid1D.xMin();
    const auto w_xmax = w_grid1D.xMax();
    // create shared ptr grid
    auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string {"d=1 [0,1] [1:100]"});
    // one-dimensional scalar field
    vsc19::scalarField1D w_scal1D{grid1Dptr,std::string{"scalar field 1D"}};
    auto& w_valu = w_scal1D.values();
    auto begVal = std::begin(w_scal1D.values());
    auto endVal = std::end(w_scal1D.values());
    auto dist = std::distance(begVal,endVal);
    assert(100==dist);
    std::fill(std::begin(w_scal1D.values()), std::end(w_scal1D.values()),2.3);
    auto check = w_scal1D.values()[w_scal1D.grid().getPoint(10)];
    
    //assert(w_scal1D.grid().getPoint(1)==2.3);  first grid point
    vsc19::scalarField1D w_copyField = w_scal1D; // copy construct
    //auto check1 = w_copyField.grid().getPoint(1);
    auto check11 = w_copyField.values()[w_copyField.grid().getPoint(1)];
    assert(check == check11);
    assert(w_copyField.values().size() != 0);
    vsc19::scalarField1D w_mvField = std::move(w_copyField); // move copy ctor
    //auto cpySiz = w_copyField.values().size(); cannot call size anymore after move 
    //assert(w_copyField.values().size()==0); // moved
    auto aa = w_scal1D.values()[1];
    assert(w_mvField.values().size()!=0);
    auto w_sumField = w_scal1D + w_mvField; // add 2-scalar field
  //  auto  = w_scal1D.values()[1];
    assert(w_sumField.values().size()==100);
    auto check12 = w_sumField.values()[w_sumField.grid().getPoint(10)];
    // substract 2 scalar field
    auto w_testMinus = w_sumField - w_scal1D;
   
    assert(w_testMinus.values().size()!=0);
    auto check13 = w_testMinus.values()[w_testMinus.grid().getPoint(1)];
    // create a scalarField1D for testing (need to create a grid lattice first)
 //   vsc19::GridLattice w_grid2Test{ std::string{"d=1 [0,1] [1:100]"}};
    // some initialization
    auto gridPtr = std::make_shared<vsc19::GridLattice>(100, 0., 1.);
    vsc19::valArrField w_vfield{ gridPtr, std::string{"Test Field"}};

    // check basic values

    // Perfect forwarding test with lambda generic (template)
    // calling our piece of code to see if it works!!
    vsc19::testClasses();

    std::cout << "Seems to me this is working!!\n";

    return EXIT_SUCCESS;
}