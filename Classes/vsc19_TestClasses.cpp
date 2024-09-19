
#include <iostream>
#include <ranges>             //C++20 ranges library
#include "TestTemplVS19.hpp"
#include "valarrField1D.h"

namespace vsc19 
{
   // some basic tests
    void testClasses()
    {
        std::cout << "Try to make it compile at least\n";
        std::array<int,5> w_array2Foo {1,2,3,4,5}; // initialize data
        // w_vec2Foo.reserve(5);
        // w_vec2Foo.push_back(1);
        // w_vec2Foo.push_back(2);
        // w_vec2Foo.push_back(3);
        // w_vec2Foo.push_back(4);
        // w_vec2Foo.push_back(5);
        // // generic lambda C++ 20
        int w_arrayType[3];
        w_arrayType[0]=4;
        w_arrayType[1]=2;
        w_arrayType[2]=8;
        class A{};
        auto chck = std::is_array_v<A[3]>;
        A w_arrA[3];
        std::vector w_vecInt {1,2,3,4,5};
        call2Foo(w_vecInt); // passing an lvalue (perfect forwarding)
        std::cout << "Looks like we have made the call succesfull\n";
    }

    void testScalarField1D()
    {
        // Test One-dimensional scalar field
    //d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
    // since we are using std::valarray indexing element are from 0,...,N-1
    // where is the number of grid nodes (point values) 
    // Version of the scalar field which use NumricalArray (2-dimension)
    // indexing of an element goes from 1,...,N 
    // getBase() and getMaxI() are returning those index base on array type
    // numerical array used to store data for each grid node  
    // "d=1 [0,1] [0:99]" -> one dimension, xmin=0, xmax=1 and nide index 
    // extend from 0 to N-1 where is the number of grid node (here N=100)
    // must be set to a valid range [1,N]
    vsc19::gridLattice1D w_grid1D{ std::string {"d=1 [0,1] [0:99]"} };
    const auto dx = w_grid1D.Delta();
    // some basic check
    const auto w_xmin = w_grid1D.xMin();
    const auto w_xmax = w_grid1D.xMax();
    // create shared ptr grid
    auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>( std::string {"d=1 [0,1] [0:99]"});
    
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
    }
} // End of namespace