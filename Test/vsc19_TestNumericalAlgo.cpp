// C++ include
#include <list>
// App includes
#include "../Classes/valarrField1D.h"
#include "../Classes/vsc19_SectionFlow.h"
#include "../Template17/vsc19_EmcilTreatmentAlgo.hpp"
 
namespace vsc19 {

   void testTreatmentSrc2() 
   {
       //using namespace vsc19;
       std::list<SectionFlow> myListOfSectionFlow;
       myListOfSectionFlow.push_back(SectionFlow{1, 0.});
       myListOfSectionFlow.push_back(SectionFlow{2, 0.1});
       myListOfSectionFlow.push_back(SectionFlow{3, 0.2});
       myListOfSectionFlow.push_back(SectionFlow{4, 0.3});
       myListOfSectionFlow.push_back(SectionFlow{5, 0.4});

       // create shared ptr grid
        auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});

        // one-dimensional scalar field
        vsc19::scalarField1D w_scal1D{grid1Dptr, std::string{"scalar field 1D"}};
        auto &w_valu = w_scal1D.values();
        auto begVal = std::begin(w_scal1D.values());
        auto endVal = std::end(w_scal1D.values());
        std::fill(std::begin(w_scal1D.values()), std::end(w_scal1D.values()), 2.3);
        // for our test just make a copy
   //     vsc19::scalarField1D w_cpyScal1D{}w_scal1D;
   }
} // End of namespace