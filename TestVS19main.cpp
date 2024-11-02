
// C include
#include <cassert>
//C++ includes
#include <iostream>
#include <cstdlib>
// STL includes
#include <valarray>
#include <vector>
// boost includes
#include <boost/iterator/transform_iterator.hpp>
// App includes
 #include "Template17/vs19_Utilities.hpp"
 //#include "Template17/vsc19_EmcilTreatmentAlgo.hpp"
 //#include "Classes/vs19_valArrField.h"
 //#include "Classes/TestTemplVS19.hpp"
 #include "Classes/valarrField1D.h"
 #include "Classes/vsc19_SectionFlow.h"
 #include "Classes/vsc19_factoryHelpers.hpp"
// BaseNumericalTypes package
#include "Sfx/Sfx_MthArray.h"

//
// Reference: https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference 
//

// forward call (forward declaration)
namespace vsc19 { 
    void testClasses();
    void ExamplOfUseRval();
    void tesPerfectFwdSrcAlgo();
    void testScalarField1D();
    void SomeCppBasicTests();
 } 

// =============================================================
// Main entry point
// =============================================================
int main( int argc, char* argv[])
{
  #if 0
  vsc19::ExamplOfUseRval();

  const auto grid1Dptr = // string helper for grid parameters
         std::make_shared<vsc19::gridLattice1D>( std::string {"d=1 [0,1.] [0:4]"});

  assert(5==grid1Dptr->getNoPoints());
  std::vector<double> w_initFieldValues(5);
  //w_initFieldValues.reserve(grid1Dptr->getNoPoints());
  std::fill( w_initFieldValues.begin(),
             w_initFieldValues.end(),1.);

  // one-dimensional scalar field
  vsc19::scalarField1D w_scal1DA{ grid1Dptr, std::string{"Scalar field A-var"}, w_initFieldValues};
  vsc19::scalarField1D w_scal1DQ{ grid1Dptr, std::string{"Scalar field Q-var"}};
  auto& w_Aval = w_scal1DA.values();
  auto& w_Qval = w_scal1DQ.values();    
  // copy ctor??? make sense?
  w_Qval = w_Aval;
  auto cgeckCpy = w_scal1DQ.values();
  assert(1.==w_Qval[0]);
  assert(1.==w_Qval[4]);
#endif

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
  //auto checkPtr = std::begin(w_vl1 + 0.5 * w_dU1);
  
  // NOTE
  //  Reconstruction procedure using MUSCL at second-order. This is a compact form
  //  Usage: 
  //     std::transform(... HLLFlux);
  // valarray ctor takes a ptr as arg
  valarray<double> UL1{std::begin(w_vl1 + 0.5 * w_dU1), std::size(w_v1) - 1}; //face 0...99 = 100 (computational domain)
  valarray<double> UR2{std::next(std::begin(w_vl2 + 0.5 * w_dU2)), std::size(w_v2) - 1}; // ditto

  std::list<SectionFlow> myListOfSectionFlow;
  myListOfSectionFlow.push_back(SectionFlow{1, 0.});
  myListOfSectionFlow.push_back(SectionFlow{2, 0.1});
  myListOfSectionFlow.push_back(SectionFlow{3, 0.2});
  myListOfSectionFlow.push_back(SectionFlow{4, 0.3});
  myListOfSectionFlow.push_back(SectionFlow{5, 0.4});

  // factory test
  using namespace std::literals;
  auto w_str2Cmp = "NujicAlgo"sv;
  // initialization and check cond.
  if (auto retEqual = "NujicAlgo" <=> w_str2Cmp;
      retEqual == std::strong_ordering::equal)
  {
    std::cout << "Equal string, can proceed\n";
    // pass 1st arg as prvalue: pure reading value
    // pass 2nd arg as lvalue: left value
    RhsDiscretization w_rhsTestPtr;
    std::shared_ptr<PhysicsAlgo> w_physicsAlgo = // base pointer
        factoryCreator<NujicAlgo>(string{"NujicAlgo"}, &w_rhsTestPtr, myListOfSectionFlow);
    if (nullptr != w_physicsAlgo)
    {
      w_physicsAlgo->calculate();
    }
  }
  else if (retEqual == std::strong_ordering::greater)
  {
    std::cout << "String not equal\n";
  }

  // shared pointer tests
  vsc19::SomeCppBasicTests();

  // testing scientific algorithm algo using Modern C++
  vsc19::tesPerfectFwdSrcAlgo();

  // testing fold expresion
  auto floatVal = 1.2f;
  auto val1 = 1;
  auto val2 = 2;
  auto val3 = 3;
  vsc19::printAddspace10<float, int, int, int>(floatVal, val1, val2, val3);

  // struct with tup[le attribute]
  // vsc19::jbTpl<float,double,double> defTpl;
  vsc19::jbTpl<unsigned, float, double, double> testTplCtor(1, 1.f, 2.5, 4.2);
  auto tplSiz = testTplCtor.size();
  auto [id, A, Q, H] = testTplCtor.m_tpl; // structured binding

  using Nodal_Value = vsc19::jbTpl<unsigned /*id*/, double /*A*/, double /*Q*/, double /*H*/>;
  std::vector<Nodal_Value> w_Uh;
  w_Uh.reserve(3);
  w_Uh.emplace_back(Nodal_Value(1, 1., 2.5, 4.2));
  w_Uh.emplace_back(Nodal_Value(2, 2.4, 0.5, 14.2));
  w_Uh.emplace_back(Nodal_Value(3, 2., 20.5, 1.2));
  assert(3 == w_Uh.size());
  auto check = w_Uh[0];
  // check some values (structured binding)
  auto [id0, Am, Qm, Hm] = w_Uh[0].m_tpl;
  auto valDbl = std::get<1>(w_Uh[0].m_tpl);
  w_Uh[0] = Nodal_Value(3, 0., 0.5, 4.2); // assign ctor
  w_Uh[0] = {3, 0.1, 0.45, 41.2};         // aggregagte tuple-like API
  auto [id3, Am1, Qm1, Hm1] = w_Uh[0].m_tpl;

  // testing extracting values from U_h as we usually do
  std::function<double(const Nodal_Value &)> w_getA =
      [](const Nodal_Value &aVal) -> double
  { return std::get<1>(aVal.m_tpl); };

  // std::function<double(const Nodal_Value &)> w_getQ =
  //     [](const Nodal_Value &aVal) -> double { return aVal[1]; };

  // std::function<double(const Nodal_Value &)> w_getH =
  //     [](const Nodal_Value &aVal) -> double { return aVal[2]; };

  //   switch (aNvalComp) {
  //   case NodalValComp::A: {
  // using delegate function
  using namespace boost;
  std::vector<double> aVec2Fill;
  aVec2Fill.reserve(3);
  aVec2Fill.assign(make_transform_iterator(w_Uh.begin(), w_getA),
                   make_transform_iterator(w_Uh.end(), w_getA));

  // auto vec0 = aVec2Fill[0];
  // auto vec1 = aVec2Fill[1];
  // auto vec2 = aVec2Fill[2];
  std::cout << "Value of U_h for A variable are: \n";
  vsc19::printAddspace10<double, double, double>(aVec2Fill[0], aVec2Fill[1], aVec2Fill[2]);
  //  }
   // sanity check
   assert(3==aVec2Fill.size());

    std::cout << "Try to make it work VSCode project\n";
    
    std::cout << "Start testing our new one-dimensional scalar field\n";
    vsc19::testScalarField1D();
    std::cout << "End of testing scalar field\n";

    std::cout << "Some test with rvalue reference (check overload resolution)\n";
    vsc19::ExamplOfUseRval();
    std::cout << "Rvalue refernce test ended\n";
 
 //   std::cout << "Numerical array for fast-floating point operation\n";
    Sfx::MthArray w_testLink(5);
    assert( 5 == w_testLink.size());
    std::cout << "End numerical test\n";

    // check basic values
    // Perfect forwarding test with lambda generic (template)
    // calling our piece of code to see if it works!!
    vsc19::testClasses();

    std::cout << "Seems to me this is working!!\n";

    return EXIT_SUCCESS;
}