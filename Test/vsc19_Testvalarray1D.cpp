
// C++ include
#include <numeric>
// C++20 include
#include <ranges> // view
// App includes
#include "../vs19_AppConstant.hpp"
#include "../vs19_UniversalConst.hpp"
#include "../Classes/valarrField1D.h"
#include "../Classes/vsc19_StateVectorField.h"  // vector field
#include "../Classes/vs19_MinMod.hpp"
#include "../Algo/vs19_numScheme.hpp" // protype numerical algorithm
#include "../Algo/vsc19_NumericalSourceTerms.h"

namespace vsc19 
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
    void testScalarField1D()
    {
        // create shared ptr grid
        auto grid1Dptr = // unit move VS19 project
          std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
        assert(99 == grid1Dptr->getMaxI()); //sanity check
        assert(0 == grid1Dptr->getBase()); 
        const auto dx = grid1Dptr->Delta();
        assert(0.01 == dx);
        const auto w_xmin = grid1Dptr->xMin();
        const auto w_xmax = grid1Dptr->xMax();
        assert(0. == w_xmin);
        assert(1. == w_xmax);

        // one-dimensional scalar field from grid
        vsc19::scalarField1D w_scal1D{ grid1Dptr, std::string{"scalar field 1D"}};
        //auto &w_valu = w_scal1D.values();
        auto begVal = std::begin(w_scal1D.values()); 
        auto endVal = std::end(w_scal1D.values());
        auto dist = std::distance(begVal, endVal);
        assert(w_scal1D.values().size() == std::distance(begVal, endVal));
     //   std::fill(std::begin(w_scal1D.values()), std::end(w_scal1D.values()), 2.3);
        // 49 shall last point with value 2.5
        std::fill_n( std::begin( w_scal1D.values()),w_scal1D.values().size()/2, 2.5);
        // 50 shall be the first index with value 1.0
        std::fill_n( std::next( std::begin(w_scal1D.values()), w_scal1D.values().size()/2), 
        w_scal1D.values().size()/2, 1.);

        // ... to be completed
        auto w_xgrid = w_scal1D.grid().getPoint(10); // shall be 0.1 dx=0.01
        assert(0.1 == w_scal1D.grid().getPoint(10)); // grid spacing
        assert(vsc19::DIM::value == w_scal1D.values().size());
        auto vec = w_scal1D.asStdVector();      //prvalue: pure reading value (temporary)
        auto&& vecRef = w_scal1D.asStdVector(); // give the return value a name I can use it as lvalue 
                                                // (Universal reference??) i do think so!!

        // calling the right function according to the number and type of args 
        // cGravity is a variable template to declare universal constant
        vsc19::hllSchemeFlux( w_scal1D.asStdVector(),  // container of values
                              w_scal1D.grid().Delta(), // grid spacing
                              vsc19::cGravity<double>, vsc19::DIM::value); // constant 

        // physics based algorithm
        double* const w_U1 = new float64[5];
        w_U1[0] = 1.; // initial values
        w_U1[1] = 2.;
        w_U1[2] = 3.;
        w_U1[3] = 4.;
        w_U1[4] = 5.;
        double* const w_U2 = new float64[5];
        w_U2[0] = 5.; // initial values
        w_U2[1] = 4.;
        w_U2[2] = 3.;
        w_U2[3] = 2.;
        w_U2[4] = 1.;
        // to be computed by the algorithm
        double* w_FF1 = new float64[5]; 
        double* w_FF2 = new float64[5]; // ditto
        // takes pointer as arguments (support legacy code)  
        vsc19::hllSchemeFlux(w_U1,w_U2,w_FF1,w_FF2);
        // clean-up code (all done)
        delete [] w_U1; delete [] w_U2; delete [] w_FF1; delete [] w_FF2;

        // call another algorithm passing library type
        vsc19::numericalScheme1D(w_scal1D);
        
        // does it move ctor? pass a temporary!! (prvalue: pure reading value)
        std::vector<double> w_testSiz(std::begin(w_scal1D.values()),std::end(w_scal1D.values())); 
        assert(vsc19::DIM::value == w_testSiz.size());
        w_testSiz.push_back(1.); // physical boundary cond.
        assert(vsc19::EMCNEILNbSections::value == w_testSiz.size());

        // assert(w_scal1D.grid().getPoint(1)==2.3);  first grid point
        vsc19::scalarField1D w_copyField = w_scal1D; // copy construct
        // auto check1 = w_copyField.grid().getPoint(1);
        const auto check11 = w_copyField.values()[0];
        assert(w_scal1D.values()[0] == check11); // expect equal
        assert(w_copyField.values().size() != 0); 
        // move copy ctor semantic
        vsc19::scalarField1D w_mvField = std::move(w_copyField);
        
        //auto cpySiz = w_copyField.values().size(); cannot call size anymore after move
        //assert(w_copyField.values().size()==0); // moved
        //auto aa = w_scal1D.values()[1];
        assert(w_mvField.values().size() != 0);
        auto w_sumField = w_scal1D + w_mvField; // add 2-scalar field
                                                //  auto  = w_scal1D.values()[1];
        assert(w_sumField.values().size() == vsc19::DIM::value);
        auto check12 = w_sumField.values()[10]; // 5 is expected
        assert(5==check12); // expected equal
        // want to test assignment ctor
        w_sumField = w_mvField; //assign ctor
        assert(2.5==w_sumField.values()[2]); // expected equal
  
        //assign move ctor???

        // substract 2 scalar field
        const auto w_testMinus = w_sumField - w_scal1D; // 2.5 expected
        auto w_chectX = 2.*w_scal1D; 
        auto check13 = w_chectX.values()[10];
        auto w_chectP = 2. + w_scal1D; 
        auto check14 = w_chectP.values()[10];
 
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
        assert( result.size() == w_fieldVec.size());
        // apply minmod limiter
        result[0]=0.; // force first lement to 0
        result.push_back(0.); // add this for comparison last element
        assert(result.size() == w_fieldVec.size()+1);
        std::adjacent_difference(result.begin(),result.end(),result.begin(),vsc19::MinMod19<double>{});
        // remove first element (adjacent_difference keep first element)
        auto rngViewDU = result | std::views::drop(1); // adaptor? drop_view!! convert vector to view?
        assert(rngViewDU.size() == w_fieldVec.size());
        std::vector<double> w_S( w_fieldVec.size());

        // NOTE
        //   below function implementation use physical boundary (adding the ghost node)
        //   purpose of this we need to evaluate the last computational node (domain)
        //   physical boundary are right open (not tied node a far right)
        //   we don't want to modify the original, passing rvalue reference 
        // test rvalue reference (move) add b.c. node inside this algorithm
        // can modify the original since we pass a temporary (physical boundary cnd)
        TreatmentSource2(w_S,  //pass temporary (below modify inside adding physical b.c.)
        w_sumField.asStdVector(),w_mvField.asStdVector(),w_scal1D.asStdVector(),
        std::vector<float64>(DIM::value),0.1,DIM::value);
        // sanity check (check sie since these has been modified)
        assert( vsc19::DIM::value == w_mvField.values().size()); // expedted equal
        assert( vsc19::DIM::value == w_sumField.values().size());  // ditto
        assert( vsc19::DIM::value == w_scal1D.values().size());    // ditto

        std::cout << "Passing temporary to numerical algo. Done!\n";

        // IMPORTANT  +++ testing view of the range library
        //  std::views::all(rng) is an adaptor that convert a range to a view.
        //  It returns a ref_view if not already a view, otherwise a copy of the view.  
        // Since the design of our numerical types is STL-like API (provides begin/end) 
        // can be manipulate as STL container. But with te help of view or range library
        // we can use it in the algorithm 
        // create a view from adaptor 'all', which convert a range into a ref_view
        auto w_vws =  std::views::all(w_mvField); //lvalue

        // could we use views to print some field values?
        // i do think so, looks like by defining STL-like API for our scalar field
        // we can use it as range or view (iterate through all its element)
        // C++17 feature Class Template Argument Deduction (CTAD), not sure what's that means?
        for( const auto &elem : w_mvField | std::views::take(5))
        {
            std::cout << "Field value is: " << elem << "\n";
        }

        // if I modify the view it also modify the field it refers to? yes it does!!
        // ref_view doesn't own the range, hold a reference to the range.
        // NOTE views on lvalue have a reference semantic (C++20 The Complete Guide)
        for( auto &elem : w_mvField | std::views::take(5))
        {
            elem = 21.34;
            std::cout << "Field value is: " << elem << "\n";
        }
        // just checking 
        const auto& fieldVal = w_mvField.values();
        std::cout << "mv field value is: " << fieldVal[0] << "\n"; //expect_equal 21.34
        std::cout << "mv field value is: " << fieldVal[1] << "\n"; // ditto
        std::cout << "mv field value is: " << fieldVal[2] << "\n"; // ditto
        std::cout << "mv field value is: " << fieldVal[3] << "\n"; // ditto
        std::cout << "mv field value is: " << fieldVal[4] << "\n"; // ditto
        std::cout << "mv field value is: " << fieldVal[5] << "\n"; // expect_equal 2.5

        // what it does exactly? it changes values of the w_mvField? 
        // since it returns a ref_view (doesn't own the field), after thsi operation   
        // all field values should have been changed, isn't it?
        auto w_transfvws = std::views::all(w_mvField) | std::views::transform([] ( float64 aValu2Transf){return 2.*aValu2Transf;});  
        // just a test
     //   auto w_chkDAta = (std::views::all(w_mvField) | std::views::transform([] ( float64 aValu2Transf){return 2.*aValu2Transf;})).data();  
        auto vlaVws = w_transfvws[0]; // is that correct? yes it it!!!
        //assert(&w_mvField.values()[0] == w_transfvws.data());

        // do some basic check (sanity check)
        std::cout << "mv transf field value is: " << w_mvField.values()[0] << "\n"; //expect_equal 21.34
        std::cout << "mv transf field value is: " << w_mvField.values()[1] << "\n"; // ditto
        std::cout << "mv transf field value is: " << w_mvField.values()[2] << "\n"; // ditto
        std::cout << "mv transf field value is: " << w_mvField.values()[3] << "\n"; // ditto
        std::cout << "mv transf field value is: " << fieldVal[4] << "\n"; // ditto      
        std::cout << "mv transf field value is: " << fieldVal[5] << "\n"; // expect_equal 2.5
        
        // owning view
        auto testView = std::views::all(std::move(w_mvField)); //rvalue

        std::vector<int> w_vec{1,2,3};
        auto dataPtr = (std::views::all(w_vec)).data(); // | std::views::transform( [] ( auto aValu2Transf) {return 2.*aValu2Transf;})).data();
        auto val = *dataPtr;
        constexpr std::string_view str { "Hello, C++20!" };
        auto strTrunc = (str | std::views::drop(7)).data();
        
        //auto begIter = dataPtr; 
        auto w_scalFvec = w_scal1D.asStdVector();
        auto w_scalFval = w_scal1D.values();
        auto refType = std::views::all(w_scal1D);
        
        // convert scalar field to view and then retrieve pointer
        // I don't know how to retrieve data from pointer
        // it has to do with valarray ()
        auto fieldPtr = (std::views::all(w_scal1D)).data();
        auto* ptr = fieldPtr;
        assert(nullptr!=fieldPtr); // just a check!!!
        
        std::cout << "Done with field testing\n"; 
    }

    // Test valarray
    // void valArrayTest() 
    // {
    //     const auto grid1Dptr = // string helper for grid parameters
    //         std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1.] [0:4]"});

    //     assert(5 == grid1Dptr->getNoPoints());
    //     std::vector<double> w_initFieldValues(5); //zero initialization
    //     // w_initFieldValues.reserve(grid1Dptr->getNoPoints());
    //     std::fill(w_initFieldValues.begin(),
    //               w_initFieldValues.end(), 1.);

    //     // one-dimensional scalar field
    //     vsc19::scalarField1D w_scal1DA{ grid1Dptr, std::string{"Scalar field A-var"}, w_initFieldValues};
    //     vsc19::scalarField1D w_scal1DQ{ grid1Dptr, std::string{"Scalar field Q-var"}};

    //     auto& w_Aval = w_scal1DA.values();
    //     auto& w_Qval = w_scal1DQ.values();
        
    //     // copy ctor??? make sense?
    //     w_Qval = w_Aval;
    //     auto cgeckCpy = w_scal1DQ.values();
    //     assert(1. == w_Qval[0]);
    //     assert(1. == w_Qval[4]);
        
    //     using namespace std;
    //     using namespace vsc19;

    //     vector w_v1 = {1., 2., 3., 4., 5.};
    //     vector w_v2 = {6., 7., 8., 9., 10.};
    //     valarray<float64> w_vl1(w_v1.data(), 5);
    //     valarray<float64> w_dU1(1., 5);
    //     valarray<float64> w_vl2(w_v2.data(), 5);
    //     valarray<float64> w_dU2(1., 5);
    //     valarray<float64> w_testSiz(w_v1.data(), 6);
    //     w_testSiz[5] = 12.5;

        // const double* returned by begin for valarray
        // auto checkPtr = std::begin(w_vl1 + 0.5 * w_dU1);

        // NOTE
        //  Reconstruction procedure using MUSCL at second-order. This is a compact form
        //  Usage:
        //     std::transform(... HLLFlux);
        // valarray ctor takes a ptr as arg and begin return an iterator which is a pointer to double
    //     valarray<double> UL1{ std::begin(w_vl1 + 0.5 * w_dU1), std::size(w_v1) - 1};            // face 0...99 = 100 (computational domain)
    //     valarray<double> UR2{ std::next(std::begin(w_vl2 + 0.5 * w_dU2)), std::size(w_v2) - 1}; // ditto 1...100
    // }

     // testing if we can modify a const shared_pointer? yes if the object it pointed-to
     // is not constant. Below we can see that the scalar field is modify even if we have
     // a const smart pointer 
    // void passShrdPtrRef( const std::shared_ptr<vsc19::scalarField1D>& aField2Modify)
    // {
    //     auto& valuesUpdated = aField2Modify->values();
    //     valuesUpdated[0] = 1.34;
    //     valuesUpdated[1] = 12.34;
    //     valuesUpdated[2] = 1.04;
    //     valuesUpdated[3] = 0.34;
    //     // copy ctor??
    //     auto w_sclrField = *aField2Modify;
    //     // support move semantic?? it should be!!
    //     auto w_sclrFild = std::move(w_sclrField);
    //     // shall be zero after 
    //     auto chksiz = w_sclrFild.values().size();

    //     std::cout << "checking basic functionnalities for scalar field\n";
    // }

    // testing our new type vectorField. Replace the old type StateVector
    // which was a pair shred_ptr of scalarField. This new type is more 
    // complete provides services for scalar field
    void vectorFieldtest()
    {
        std::cout << "Starting State Vector Field test\n";
        // create field with default config
        StateVectorField w_myField2Test{ std::string{"Field2Test"}};
        if( w_myField2Test.isNull())
        {
            // create grid from a string representation
            auto w_grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
            assert(99 == w_grid1Dptr->getMaxI());
            assert(0 == w_grid1Dptr->getBase());
            assert(100 == w_grid1Dptr->getDivisions());
            assert(100 == w_grid1Dptr->getNoPoints());

            auto ret = w_myField2Test.reset(w_grid1Dptr);
            assert(ret);

            // -----------------------------------
            //  just testing some stuff (modifying the pointed-to check)
     //       passShrdPtrRef(w_myField2Test.getA());
        }

        // basic test 
        auto& w_Afield = w_myField2Test.getAfield();
        auto& w_Qfield = w_myField2Test.getQfield();
        // check field is empty
        assert(vsc19::DIM::value == w_Afield.values().size());
        assert(vsc19::DIM::value == w_Qfield.values().size());
        
        // initialize the field to some values (step function 1.|0.5)
        std::fill_n(w_Afield.begin(),w_Afield.values().size()/2,1.);
        std::fill( std::next(w_Afield.begin(), w_Afield.values().size()/2), w_Afield.end(), 0.5);
        auto isZeroInit = std::all_of( w_Qfield.begin(), w_Qfield.end(),
        [] (float64 aValue) { return aValue==0;});
        if( !isZeroInit) // sanity check
        {
            //set to zero 
            std::cerr << "Problem with initialization of Q-field, force init to 0\n";
            std::fill(w_Qfield.begin(), w_Qfield.end(),0.);
        }

        std::cout << "Completed initialization of StateVectorField\n";

        // Do some tests about copy and move
        StateVectorField w_cpyFrom = w_myField2Test;
        assert(vsc19::DIM::value == w_cpyFrom.getA()->values().size());
        assert(vsc19::DIM::value == w_cpyFrom.getQ()->values().size());
        // grid dimension        
        assert(0 == w_cpyFrom.getA()->grid().getBase());
        assert(99 == w_cpyFrom.getQ()->grid().getMaxI());
        // init values check
        auto w_Avarray = w_Afield.values();
        auto w_Qvarray = w_Qfield.values();
        assert(w_Avarray[0]==1.);
        assert(w_Avarray[50]==.5);
        assert(w_Avarray[99]==.5);
        assert(w_Qvarray[99]==0.);
        assert(w_Qvarray[0]==0.);
        
        // test create from tuple value (nbpts,min,max)
        StateVectorField w_tplField2Test{ std::string{"TplField2Test"}, std::make_tuple(100, 0.,1.)};
        if( !w_tplField2Test.isNull())
        {
            assert(99 == w_tplField2Test.grid().getMaxI());
            assert(0 == w_tplField2Test.grid().getBase());
            assert(100 == w_tplField2Test.grid().getDivisions());
            assert(100 == w_tplField2Test.grid().getNoPoints());
            assert(100 == w_tplField2Test.getAfield().values().size());
            assert(100 == w_tplField2Test.getQfield().values().size());
        }

        auto isZeroValues = std::all_of( w_tplField2Test.getAfield().begin(), w_tplField2Test.getAfield().end(),
        [] (float64 aValue) { return aValue==0;});
        assert(isZeroValues);

        // test assignment ctor
        StateVectorField w_asgnField2Test{ std::string{"AsgnField2Test"}};
        assert(w_asgnField2Test.isNull());

        // what happen if assign another state vector field?
        w_asgnField2Test = w_cpyFrom;
        // retieve array (valarray)
        const auto& w_Avarr1 = w_cpyFrom.Avalues();
        const auto& w_Avarr2 = w_asgnField2Test.Avalues();
        //assert(w_Avarr1==w_Avarr2);
        auto boolArr = w_Avarr1==w_Avarr2;
    //     std::cout << "w_Avarr1 == w_Avarr2 is " << std::boolalpha << "\n";
        const auto& w_Qvarr1 = w_tplField2Test.Qvalues();
        const auto& w_Qvarr2 = w_asgnField2Test.Qvalues();
        auto cheQ = w_Qvarr1==w_Qvarr2;
        // check move semantic
        StateVectorField w_mvField=std::move(w_asgnField2Test);

        std::cout << "End StatVectorField test\n";
    }
} // End of namespace