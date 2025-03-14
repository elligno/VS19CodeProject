
#pragma once

// STL includes
#include <valarray>
#include <vector>
// App includes
// #include "../Classes/vsc19_ListSectionFlow.h"
// #include "../Classes/valarrField1D.h"

namespace vsc19 {
   class scalarField1D;
   class ListSectionFlow;
}
namespace vsc19 
{
  void TreatmentTermeS0( std::vector<double> aVec, double dx, int NbSections);              // { std::cout << "TreatmentTermeS0 with 3 arguments\n"; }
  void TreatmentTermeS0( std::vector<double> aVec, double dx, double grav, int NbSections); // { std::cout << "TreatmentTermeS0 with 4 arguments\n"; }
  
  void TreatmentSource2( std::vector<double>& aS, 
      std::vector<double>&& aQ, std::vector<double>&& aA, std::vector<double>&& aH, //rvalue reference
      const std::vector<double> &n,  double aDx = 10., unsigned aNbSections = 101); //rvalue reference

  void TreatmentSource2( std::vector<double> &aS, const std::vector<double> &aQ, const std::vector<double> &aA,
                         double aDx, unsigned aNbSections);  // lvalue reference

  std::valarray<double> TreatmentSource2( const scalarField1D& aQ, const scalarField1D& aA, 
  const ListSectionFlow& aListSectFlow);
} // End of namespace