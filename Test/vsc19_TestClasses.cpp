// C++ includes
#include <iostream>
#include <ranges>             //C++20 ranges library
// library includes
#include "../vs19_AppConstant.hpp"
#include "../Classes/Types2Test.h"
//#include "../Classes/vs19_MinMod.hpp"
#include "../Classes/vsc19_scalarField.h"
//#include "../Classes/valarrField1D.h"
#include "../Classes/TestTemplVS19.hpp"
#include "../Algo/vsc19_NumericalSourceTerms.h"
#include "../Template17/vsc19_PerfectFwdAlgo.hpp"

namespace {
    // return temporary to initialize variable (for testing)
    auto getVectorInt() noexcept { return std::vector{1, 2, 3, 4, 5}; }
    auto asStdVector()  noexcept { return std::vector{1., 2., 3., 4., 5.}; }
    auto getBCFromTpl() noexcept { return std::make_tuple(10., 0.,10.); }
}

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
        call2Foo(std::vector{1,2,3,4,5});
        std::cout << "Looks like we have made the call succesfull\n";
    }

    std::vector<double> getVector() {return std::vector{0.2,28.6,13.4,4.21,15.23};}

    // testing perfect forwarding numerical algorithm 
    void tesPerfectFwdSrcAlgo() 
    {
        // some data to test 
        std::vector<double> w_vecByRef(101); //1.2,2.6,3.4,4.1,5.};
    //    const std::vector w_vecByConstRef {0.2,28.6,13.4,4.21,15.23};
        // shall call the first version of the algorithm (lvalue)
        double dx = 0.1;
#if 0 // rvalue test
        //std::is_rvalue_reference
        auto&& w_fwdRef = getVector();
       // call2SrcrTeatmentAlgo(w_vecByRef, std::vector{0.,0.,0.,0.,0.}, getVector(),n, dx,5);
    //   TreatmentSrcTerms(w_vecByRef, std::vector{0.,0.,0.,0.,0.}, getVector(),n, dx,5);
       fwdSrcAlgo(w_vecByRef, std::vector{0.,0.,0.,0.,0.}, getVector(),n, dx,5);
#endif
      // lvalue test
      // std::vector<double> n(EMCNEILNbSections::value);
     //  std::vector w_vecA {1.2,3.5,54.2,23.4,2.};   //lvalue
     //  std::vector w_vecQ {11.2,32.5,4.2,3.4,2.12}; // lvalue
      // fwdSrcAlgo(w_vecByRef, w_vecA, w_vecQ, n, dx, 5); //value case

    // create shared ptr grid (typical use of perfect forwarding)
    const auto grid1Dptr = // string helper for grid parameters
         std::make_shared<vsc19::gridLattice1D>( std::string {"d=1 [0,1000.] [0:99]"});
    dx = grid1Dptr->Delta(); // grid spacing
    
    //sanity check
    assert(10.==dx);
    assert(99==grid1Dptr->getMaxI());
    assert(0==grid1Dptr->getBase());
   
    // initial values for testing
    std::vector<double> w_initFieldValues(EMCNEILNbSections::value);
    std::fill_n( w_initFieldValues.begin(),50,10.);
    std::fill( w_initFieldValues.begin()+w_initFieldValues.size()/2,
               w_initFieldValues.end(),1.);

    // one-dimensional scalar field
     scalarField1D w_scal1DA{ grid1Dptr, std::string{"Scalar field A-var"}, w_initFieldValues};
     scalarField1D w_scal1DQ{ grid1Dptr, std::string{"Scalar field Q-var"},  // set to zero
                                         std::vector<double>(w_scal1DA.values().size())};
     assert(w_scal1DA.values().size()==100);

     using stdvec = std::vector<double>;

     // just testing (set to zero???)
     std::array<double, EMCNEILNbSections::value> w_testCpy;
     w_testCpy.fill(0.); // zero initialization
     assert(w_testCpy.size() == 101);
     std::copy(std::begin(w_scal1DA.values()), std::end(w_scal1DA.values()),w_testCpy.begin());
     assert(w_testCpy[w_scal1DA.values().size()]==0.);   // global discretization
     assert(w_testCpy[w_scal1DA.values().size()-1]==1.); // last value computational domain
     w_testCpy[w_scal1DA.values().size()]=1.; //b.c cond.

     // move semantic version (perfect forwarding)
     // explicit operator conversion from scalar field (function signature based on std vector)
     // cast to std vector to make clean "fwdSrcAlgo" is wrapper function that call physics algo 
     // Here we are passing temporaries to 2nd,3rd, and 4th args], the call
     // will be forwarded to the function with forward reference
     fwdSrcAlgo(w_vecByRef, static_cast<stdvec>(w_scal1DQ),    // explicit conversion operator
                             static_cast<stdvec>(w_scal1DA),    // ditto
                             stdvec(EMCNEILNbSections::value),  // set to zero
                             dx, w_scal1DA.values().size());    // value case
                             
    //    fwdSrcAlgo(w_vecByRef, std::vector<double>{1.2,3.5,54.2,23.4,2.}, 
    //    std::vector<double> {11.2,32.5,4.2,3.4,2.12}, n, dx, 5); //value case

       std::cout << "Exited test perfect forwarding\n";
    }

    void ExamplOfUseRval()
    {
        // create a scalar field with values
        const auto w_grid1d = std::make_shared<gridLattice>(std::string{"d=1 [0,1] [1:100]"});

        //  vsc19::gridLattice1D w_grid1D{ std::string {"d=1 [0,1]x[1:10]"} };
        scalarField w_A{w_grid1d, std::string{"Scalar Field for variable A"}}; // created with initial values?? yes set 0
        scalarField w_Q{w_grid1d, std::string{"Scalar Field for variable Q"}}; // ditto

        // some sanity check
        for (auto &w_valA : w_A.values()) // is it working? not sure!!
        {
            w_valA += 1.; // initialize field values
        }
        for (auto &w_valQ : w_Q.values()) // is it working? not sure!!
        {
            w_valQ += .5; // initialize field values
        }

        // sanity check
        assert(EMCNEILNbSections::value-1 == w_A.values().size());
        assert(1. == w_A.values()(3));

        // IMPORFTANT below we use braces initialization which result a vector of 1 element (101.0)
        //  use parentheses () to initialize vector with a specific number of elements
        // soma data for our test
        // std::vector<double> w_S{EMCNEILNbSections::value}; // intialize to zero
        // std::vector<double> n{EMCNEILNbSections::value};   // Manning friction coefficient
        // std::vector<double> w_H{EMCNEILNbSections::value}; // water depth (flat bed)

        // this is the correct way to initialize a vector (variable template)
        std::vector<double> w_S(EMCNEILNbSections::value); // intialize to zero
        std::vector<double> n(EMCNEILNbSections::value);   // Manning friction coefficient
        std::vector<double> w_H(EMCNEILNbSections::value); // water depth (flat bed)

        // call numerical treatment version passing temporaries (prvalue: pure reading value)
        // overload resolution (nothing to do with perfect forwarding). Below it should call
        // the function with rvalue reference.
        // by doing this we can modify the passed args without disturbing the original
        // we avoid a copy which can be expensive (in this case we need to add b.c. value)
        // call rvalue reference version because we want to modify the passed arg
        TreatmentSource2(w_S,                                                      // source term evaluation (finite-difference scheme at second-order)
                         w_Q.values().to_stdVector(), w_A.values().to_stdVector(), // prvalue (can freely modify the original)
                         std::vector<double>(EMCNEILNbSections::value),            // set to zero for debugging purpose (prvalue)
                         n, 0.1, EMCNEILNbSections::value);

        // check if vector size has changed (added a value inside function TreatmentSource2)
        // but didn't change the number of element of our vector
        assert(EMCNEILNbSections::value-1 == w_A.values().to_stdVector().size());
        assert(EMCNEILNbSections::value-1 == w_Q.values().to_stdVector().size());

        std::cout << "We have tested rvalue reference\n";
    }
} // End of namespace