
// C include
#include <cassert>
//C++ includes
#include <filesystem>
#include <iostream>
#include <cstdlib>
// STL includes
#include <valarray>
#include <vector>
// App includes
#include "Template17/vs19_Utilities.hpp"
#include "Template17/vsc19_DbgLog.hpp"    // debug file logger
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
    using dbltpl = std::tuple<double, double, double, double, double>;

    // using twographcmp = std::tuple
    using pairtimeline =
      std::pair<double /*Time*/, std::vector<dbltpl> /*line of values*/>;

  //  std::vector<pairtimeline> gVecTimeValues;

    void testClasses();
    void NodalVarTest();
    void testSomeBasicTmpl();
    void ExamplOfUseRval();
    void tesPerfectFwdSrcAlgo();
    void testScalarField1D();
    void SomeCppBasicTests();
    std::vector<vsc19::pairtimeline> 
     readDataFullResultFromFile(std::filesystem::path &aFile2read);
 } 
namespace vs19 {
  void testStdFileSystem();
  void testStringView();
}

// Factorial evaluation using metaprogramming (compile-time evaluation)
// variable template?? not too sure
template<int N> // recursive process
constexpr auto factorial = factorial<N-1>*N;
// specialized (stop criteria)
template<> auto factorial<0> = 1;

// =============================================================
//                    Main entry point
// =============================================================
int main( int argc, char* argv[])
{
  namespace fs = std::filesystem;
  using namespace std::literals;
  fs::path w_rootDir = fs::current_path();
  auto succeed = fs::create_directory(w_rootDir/"tmp");
  auto w_dbgFileName = "DbgLogFF12.txt"sv;
  auto w_fileStatus{fs::status(w_dbgFileName)};
  std::ofstream w_ofStrm{"DbgLogFF12"sv};
  vsc19::DbgLogFile<decltype(w_ofStrm), double,double> w_baseTes;
  w_baseTes.outputLine(25.4,1.34);
  
  auto w_fileName {"HLL_Scheme_FullResult_05.txt"s};
  w_rootDir /= "data";
  w_rootDir.append(w_fileName);
  if( w_rootDir.has_filename()) {
    auto check = w_rootDir.stem(); // file name
    auto w_rootPath = w_rootDir.root_path();
 //   auto w_rootDir  = w_rootDir.root_directory(); ?? doesn't work!! need initialization???
    auto w_remFile  = w_rootDir.remove_filename();
    std::cout << "Root directory is: " << w_rootDir.lexically_normal().string() << '\n';
    std::cout << "Reading data from file: " << w_rootDir.lexically_normal().string() << '\n';
  }

  // template argument deduction? ok deduce type!!
  // print arguments separated by a white space
  vsc19::printAddspace10(1,2,3,4,5);

  vsc19::testScalarField1D();
  vs19::testStdFileSystem();
  // not too sure what it is?
  vsc19::testSomeBasicTmpl();
  vs19::testStringView();
  // vsc19::ExamplOfUseRval();

  std::list<vsc19::SectionFlow> myListOfSectionFlow;
  myListOfSectionFlow.emplace_back(1, 0.);
  myListOfSectionFlow.emplace_back(2, 0.1);
  myListOfSectionFlow.emplace_back(3, 0.2);
  myListOfSectionFlow.emplace_back(4, 0.3);
  myListOfSectionFlow.emplace_back(5, 0.4);

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
    vsc19::RhsDiscretization w_rhsTestPtr;
    std::shared_ptr<vsc19::PhysicsAlgo> w_physicsAlgo = // base pointer
        factoryCreator<vsc19::NujicAlgo>(std::string{"NujicAlgo"}, &w_rhsTestPtr, myListOfSectionFlow);
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

  std::cout << "Try to make it work VSCode project\n";

  std::cout << "Start testing our new one-dimensional scalar field\n";
  vsc19::testScalarField1D();
  std::cout << "End of testing scalar field\n";

  std::cout << "Some test with rvalue reference (check overload resolution)\n";
  vsc19::ExamplOfUseRval();
  std::cout << "Rvalue refernce test ended\n";

  //   std::cout << "Numerical array for fast-floating point operation\n";
  Sfx::MthArray w_testLink(5);
  assert(5 == w_testLink.size());
  std::cout << "End numerical test\n";

  // check basic values
  // Perfect forwarding test with lambda generic (template)
  // calling our piece of code to see if it works!!
  vsc19::testClasses();

  std::cout << "Seems to me this is working!!\n";

  return EXIT_SUCCESS;
}