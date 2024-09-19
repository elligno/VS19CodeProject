
//C++ includes
#include <iostream>
#include <cstdlib>

// App includes
 #include "Template17/vs19_Utilities.hpp"
 //#include "Classes/vs19_valArrField.h"
 //#include "Classes/TestTemplVS19.hpp"
 //#include "Classes/valarrField1D.h"
// #include "Classes/Types2Test.h"
// // BaseNumericalTypes package
// #include "Sfx/Sfx_MthArray.h"

//
// Reference: https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference 
//

// forward call (forward declaration)
namespace vsc19 { 
    void testClasses();
 //   void ExamplOfUseRval();
    void testScalarField1D();
 } 

// Main entry point
int main( int argc, char* argv[])
{
    // testing fold expresion
    auto floatVal=1.2f;
    auto val1=1;
    auto val2=2;
    auto val3=3;  
    vsc19::printAddspace10<float, int,int,int>(floatVal,val1,val2,val3);

    std::cout << "Try to make it work VSCode project\n";
    
    std::cout << "Start testing our new one-dimensional scalar field\n";
    vsc19::testScalarField1D();
    std::cout << "End of testing scalar field\n";

/*     std::cout << "Some test with rvalue reference (check overload resolution)\n";
  //  vsc19::ExamplOfUseRval();
    std::cout << "Rvalue refernce test ended\n";
 */
 //   std::cout << "Numerical array for fast-floating point operation\n";
 //   Sfx::MthArray w_testLink(5);
   // assert( 5 == w_testLink.size());
  //  std::cout << "End numerical test\n";

    // check basic values
    // Perfect forwarding test with lambda generic (template)
    // calling our piece of code to see if it works!!
    vsc19::testClasses();

    std::cout << "Seems to me this is working!!\n";

    return EXIT_SUCCESS;
}