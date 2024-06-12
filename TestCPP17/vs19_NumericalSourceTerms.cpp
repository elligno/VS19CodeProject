
#include <iostream>
#include "vs19_NumericalSourceTerms.h"

namespace vsc19 
{
  void TreatmentTermeS0( std::vector<double> aVec, double dx, int NbSections) 
  { 
    std::cout << "TreatmentTermeS0 with 3 arguments\n"; 
  }
  
  void TreatmentTermeS0( std::vector<double> aVec, double dx, double grav, int NbSections) 
  { 
    std::cout << "TreatmentTermeS0 with 4 arguments\n"; 
  }
} // End of namespace