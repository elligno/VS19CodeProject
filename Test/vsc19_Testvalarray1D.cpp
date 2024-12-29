
// C++ include
#include <numeric>
// C++20 include
#include <ranges> // view
// App includes
#include "../vs19_AppConstant.hpp"
#include "../vs19_UniversalConst.hpp"
#include "../Classes/valarrField1D.h"
#include "../Classes/vs19_MinMod.hpp"
#include "../Algo/vs19_numScheme.hpp" // protype numerical algorithm
#include "../Algo/vsc19_NumericalSourceTerms.h"

namespace vsc19 
{
    void testScalarField1D()
    {
        // Test One-dimensional scalar field
        // d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
        //  since we are using std::valarray indexing element are from 0,...,N-1
        //  where is the number of grid nodes (point values)
        //  Version of the scalar field which use NumricalArray (2-dimension)
        //  indexing of an element goes from 1,...,N
        //  getBase() and getMaxI() are returning those index base on array type
        //  numerical array used to store data for each grid node
        //  "d=1 [0,1] [0:99]" -> one dimension, xmin=0, xmax=1 and nide index
        //  extend from 0 to N-1 where is the number of grid node (here N=100)
        //  must be set to a valid range [1,N]
        vsc19::gridLattice1D w_grid1D{std::string{"d=1 [0,1] [0:99]"}};
        const auto dx = w_grid1D.Delta();
        assert(0.01 == dx);
        // some basic check
        const auto w_xmin = w_grid1D.xMin();
        const auto w_xmax = w_grid1D.xMax();
        // create shared ptr grid
        auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
        assert(99 == grid1Dptr->getMaxI());
        assert(0 == grid1Dptr->getBase());

        // one-dimensional scalar field
        vsc19::scalarField1D w_scal1D{grid1Dptr, std::string{"scalar field 1D"}};
        auto &w_valu = w_scal1D.values();
        auto begVal = std::begin(w_scal1D.values());
        auto endVal = std::end(w_scal1D.values());
        auto dist = std::distance(begVal, endVal);
        assert(w_scal1D.values().size() == dist);
     //   std::fill(std::begin(w_scal1D.values()), std::end(w_scal1D.values()), 2.3);
        // 49 shall last point with value 2.5
        std::fill_n(std::begin(w_scal1D.values()),w_scal1D.values().size()/2, 2.5);
        // 50 shall be the first index with value 1.0
        std::fill_n(std::next(std::begin(w_scal1D.values()), w_scal1D.values().size()/2), w_scal1D.values().size()/2,1.);
        //auto varr = w_scal1D.values();
        auto check = w_scal1D.values()[w_scal1D.grid().getPoint(10)];
        assert(0.01 == w_scal1D.grid().Delta()); // grid spacing1
        assert(100==w_scal1D.values().size());
        auto vec = w_scal1D.asStdVector(); //prvalue: pure reading value (temporary)
        auto&& vecRef = w_scal1D.asStdVector(); // give the return value a name I can use it as lvalue
        // calling the right function according to the number of args 
        // cGravity is a variable template to declare universal constant
        vsc19::numSchemeFlux( w_scal1D.asStdVector(), w_scal1D.grid().Delta(),
                              vsc19::cGravity<double>, vsc19::DIM::value);
        // physics based algorithm
        double* w_U1 = nullptr;
        double* w_U2 = nullptr;
        double* w_FF1 = nullptr;
        double* w_FF2 = nullptr;
        vsc19::hllSchemeFlux(w_U1,w_U2,w_FF1,w_FF2);
        vsc19::numericalScheme1D(w_scal1D);
        
        // does it move ctor? pass a temporary!! 
        std::vector<double> w_testSiz(std::begin(w_scal1D.values()),std::end(w_scal1D.values())); //prvalue: pure reading value
        assert(vsc19::DIM::value == w_testSiz.size());
        w_testSiz.push_back(1.); // physical boundary cond.
        assert(vsc19::EMCNEILNbSections::value == w_testSiz.size());

        // assert(w_scal1D.grid().getPoint(1)==2.3);  first grid point
        vsc19::scalarField1D w_copyField = w_scal1D; // copy construct
        // auto check1 = w_copyField.grid().getPoint(1);
        auto check11 = w_copyField.values()[w_copyField.grid().getPoint(1)];
        assert(check == check11);
        assert(w_copyField.values().size() != 0);
        vsc19::scalarField1D w_mvField = std::move(w_copyField); // move copy ctor
        // auto cpySiz = w_copyField.values().size(); cannot call size anymore after move
        // assert(w_copyField.values().size()==0); // moved
        auto aa = w_scal1D.values()[1];
        assert(w_mvField.values().size() != 0);
        auto w_sumField = w_scal1D + w_mvField; // add 2-scalar field
                                                //  auto  = w_scal1D.values()[1];
        assert(w_sumField.values().size() == 100);
        auto check12 = w_sumField.values()[w_sumField.grid().getPoint(10)];
        // substract 2 scalar field
        auto w_testMinus = w_sumField - w_scal1D;
        auto w_chectX = 2.*w_scal1D; 
        auto check13 = w_chectX.values()[w_sumField.grid().getPoint(10)];
        auto w_chectP = 2.+w_scal1D; 
        auto check14 = w_chectP.values()[w_sumField.grid().getPoint(10)];

        assert(w_testMinus.values().size() != 0);
        //auto check15 = w_testMinus.values()[w_testMinus.grid().getPoint(10)];

        // create a scalarField1D for testing (need to create a grid lattice first)
        //   vsc19::GridLattice w_grid2Test{ std::string{"d=1 [0,1] [1:100]"}};
        // some initialization
//        auto gridPtr = std::make_shared<vsc19::GridLattice>(100, 0., 1.);
//        vsc19::valArrField w_vfield{gridPtr, std::string{"Test Field"}};

        // just prototyping slope limiter gradient function (TVD scheme)
        // avoid spurious oscillations in solution that prevent to converge
        auto w_fieldVec = w_scal1D.asStdVector(); // size = 100
        // apply std::adjacent_difference
        std::vector<double> result;
        result.reserve(w_fieldVec.size());
        std::adjacent_difference(w_fieldVec.cbegin(), w_fieldVec.cend(), std::back_inserter(result));
        assert(result.size() == w_fieldVec.size());
        // apply minmod limiter
        result[0]=0.; // force first lement to 0
        result.push_back(0.); // add this for comparison last element
        assert(result.size() == w_fieldVec.size()+1);
        std::adjacent_difference(result.begin(),result.end(),result.begin(),vsc19::MinMod<double>{});
        // remove first element (adjacent_difference keep first element)
        auto rngViewDU = result | std::views::drop(1);
        assert(rngViewDU.size()==w_fieldVec.size());
        std::vector<double> w_S(w_fieldVec.size());
        // test rvalue reference (move) add b.c. node inside this algorithm
        // can modify the original since we pass a temporary
        TreatmentSource2(w_S,  //pass temporary
        w_sumField.asStdVector(),w_copyField.asStdVector(),w_scal1D.asStdVector(),
        std::vector<double>(100),0.1,100);

   //     assert(100==w_copyField.size());

        std::cout << "";
    }

    void valArrayTest() 
    {
        const auto grid1Dptr = // string helper for grid parameters
            std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1.] [0:4]"});

        assert(5 == grid1Dptr->getNoPoints());
        std::vector<double> w_initFieldValues(5);
        // w_initFieldValues.reserve(grid1Dptr->getNoPoints());
        std::fill(w_initFieldValues.begin(),
                  w_initFieldValues.end(), 1.);

        // one-dimensional scalar field
        vsc19::scalarField1D w_scal1DA{grid1Dptr, std::string{"Scalar field A-var"}, w_initFieldValues};
        vsc19::scalarField1D w_scal1DQ{grid1Dptr, std::string{"Scalar field Q-var"}};
        auto& w_Aval = w_scal1DA.values();
        auto& w_Qval = w_scal1DQ.values();
        // copy ctor??? make sense?
        w_Qval = w_Aval;
        auto cgeckCpy = w_scal1DQ.values();
        assert(1. == w_Qval[0]);
        assert(1. == w_Qval[4]);
        
        using namespace std;
        using namespace vsc19;

        vector w_v1 = {1., 2., 3., 4., 5.};
        vector w_v2 = {6., 7., 8., 9., 10.};
        valarray<double> w_vl1(w_v1.data(), 5);
        valarray<double> w_dU1(1., 5);
        valarray<double> w_vl2(w_v2.data(), 5);
        valarray<double> w_dU2(1., 5);
        valarray<double> w_testSiz(w_v1.data(), 6);
        w_testSiz[5] = 12.5;

        // const double* returned by begin for valarray
        // auto checkPtr = std::begin(w_vl1 + 0.5 * w_dU1);

        // NOTE
        //  Reconstruction procedure using MUSCL at second-order. This is a compact form
        //  Usage:
        //     std::transform(... HLLFlux);
        // valarray ctor takes a ptr as arg and begin return an iterator which is a pointer to double
        valarray<double> UL1{std::begin(w_vl1 + 0.5 * w_dU1), std::size(w_v1) - 1};            // face 0...99 = 100 (computational domain)
        valarray<double> UR2{std::next(std::begin(w_vl2 + 0.5 * w_dU2)), std::size(w_v2) - 1}; // ditto 1...100
    }
} // End of namespace