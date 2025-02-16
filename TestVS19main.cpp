
// C include
#include <cassert>
//C++ includes
#include <type_traits>  // is_same_as
#include <filesystem>
#include <iostream>
#include <cstdlib>
// STL includes
#include <valarray>
#include <vector>
// App includes
#include "Algo/vs19_numScheme.hpp"
#include "Template17/vs19_NodeVarTmpl.hpp"
#include "Template17/vs19_Utilities.hpp"
#include "Template17/vsc19_DbgLog.hpp"    // debug file logger
 //#include "Template17/vsc19_EmcilTreatmentAlgo.hpp"
 //#include "Classes/vs19_valArrField.h"
 //#include "Classes/TestTemplVS19.hpp"
#include "Classes/vsc19_EnvVarMgr.h"
#include "Classes/vsc19_Directory.h"
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
    void testHllSchemeImpl();
    void SomeCppBasicTests();
    void vectorFieldtest();
  //  void passShrdPtrRef( const std::shared_ptr<vsc19::scalarField1D>& aField2Modify);
    std::vector<vsc19::pairtimeline> 
     readDataFullResultFromFile(std::filesystem::path &aFile2read);
 } 
namespace vs19 {
  void testStdFileSystem(); // std::filesystem tests
  void testStringView();
  void testRngAlgo( std::string_view aFileName);
}

constexpr int factorial1(int n)
{
  int out = 1;
  for (int i = 2; i <= n; i++)
    out *= i;
  return out;
}

// Factorial evaluation using metaprogramming (compile-time evaluation)
// variable template?? not too sure, yes that's what it is!!!
  // Factorial evaluation using metaprogramming (compile-time evaluation)
// variable template?? not too sure, yes that's what it is!!!
template<int N> // recursive process
constexpr int factorial = factorial<N-1>*N;
// specialized (stop criteria)
template<> constexpr int factorial<0> = 1;

// just a test 
std::string getString() {return std::string{"JeanAutoTest"};}
// auto getStringRef() 
// { 
//   std::string w_localVar{"JeanAutoTest"};
// } 

// =============================================================
//                    Main entry point
// =============================================================
int main( int argc, char* argv[])
{
   vsc19::testHllSchemeImpl();

  using namespace std::string_literals;   // operator 's'
  using namespace std::literals::string_view_literals; // operator 'sv'

    auto &&testjbAuto = getString(); // universal reference ()
    auto w_tmpStr{"tmpstr"s};
    // const auto& w_tmpStrcRef = {"tmpstrcRef"s}; // initializer_list!!! be carefull!!
    const auto &w_tmpStrcRef{"tmpstrcRef"s};
    auto &&testTmpstr = w_tmpStr; // ditto
    auto &&cstref = w_tmpStrcRef;
    //  auto&& retCref = getStringRef();
    std::cout << std::is_same_v<decltype(testjbAuto), std::string &&> << '\n'; // true
    std::cout << std::is_same_v<decltype(testTmpstr), std::string &> << '\n';  // true
    std::cout << std::is_lvalue_reference_v<decltype((testTmpstr))> << '\n';   // true
    std::cout << std::is_same_v<decltype((testjbAuto)), std::string> << '\n';  // ??
    std::cout << std::is_same_v<decltype((cstref)), const std::string &> << '\n';
    // std::cout << std::is_same_v<decltype((retCref)),const std::string&> << '\n';

    // All those variables could be set in a file that is read by the simulation
    // manager at start up, avoid re-compile each time we add/modify one of them
    std::cout << "Settings some environment variables\n";

    vsc19::EnvVariablesManager w_vs15EnvVarMgr;
    w_vs15EnvVarMgr.put(std::string{"ALGOVALIDATE_ROOT"},
                        std::string{R"(E:\JeanProjects\TestVS2019\AlgoValidation)"});
    w_vs15EnvVarMgr.put(std::string{"TESTVS19RESULT_ROOT"}, std::string{R"(E:\JeanProjects\TestVS2019\Results)"});
    w_vs15EnvVarMgr.put(std::string{"TESTVS19_ROOT"}, std::string{R"(E:\JeanProjects\TestVS2019)"});

    // directory test
    auto w_dirPath = vsc19::Directory::GetEnvVariablePath(std::string{"TESTVS19_ROOT"sv});
    std::cout << "Path for this system variable is :" << w_dirPath.lexically_normal().string() << '\n';

    // some basic tests (StateVectorField)
    // std::cout << "About to start VectorField test\n";
    //  vsc19::testScalarField1D();
    //  vsc19::vectorFieldtest();
    vs19::testStdFileSystem();

    namespace fs = std::filesystem;
    fs::path w_rootDir = fs::current_path();
    auto succeed = fs::create_directory(w_rootDir / "tmp"sv);
    constexpr auto w_dbgFileName = "DbgLogFF12.txt"sv;
    auto w_fileStatus{fs::status(w_dbgFileName)};
    if (fs::is_regular_file(w_fileStatus)) // check status
    {
      std::ofstream w_ofStrm{"DbgLogFF12"sv};
      if (w_ofStrm.is_open())
      {
        // don't remember what this for?
        vsc19::DbgLogFile<decltype(w_ofStrm), double, double> w_baseTes;
        w_baseTes.outputLine(25.4, 1.34);
      }
      w_ofStrm.close();
    }

    auto w_fileName{"HLL_Scheme_FullResult_05.txt"s};
    w_rootDir /= "data";
    w_rootDir.append(w_fileName);
    if (w_rootDir.has_filename())
    {
      auto check = w_rootDir.stem(); // file name
      auto w_rootPath = w_rootDir.root_path();
      //   auto w_rootDir  = w_rootDir.root_directory(); ?? doesn't work!! need initialization???
      auto w_remFile = w_rootDir.remove_filename();
      std::cout << "Root directory is: " << w_rootDir.lexically_normal().string() << '\n';
      std::cout << "Reading data from file: " << w_rootDir.lexically_normal().string() << '\n';
    }

    // template argument deduction? ok deduce type!!
    // print arguments separated by a white space
    vsc19::printAddspace10(1, 2, 3, 4, 5);

    vsc19::testScalarField1D();
    vs19::testStdFileSystem();
    // not too sure what it is?
    vsc19::testSomeBasicTmpl();
    vs19::testStringView();
    // vsc19::ExamplOfUseRval();

    std::list<vsc19::SectionFlow> myListOfSectionFlow;
    myListOfSectionFlow.emplace_back(1, 0., 1.);
    myListOfSectionFlow.emplace_back(2, 0.1, 1.);
    myListOfSectionFlow.emplace_back(3, 0.2, 1.);
    myListOfSectionFlow.emplace_back(4, 0.3, 1.);
    myListOfSectionFlow.emplace_back(5, 0.4, 1.);

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