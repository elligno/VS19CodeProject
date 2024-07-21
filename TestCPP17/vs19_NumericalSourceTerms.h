
#pragma once
#include <vector>

namespace vsc19 
{
  void TreatmentTermeS0( std::vector<double> aVec, double dx, int NbSections);// { std::cout << "TreatmentTermeS0 with 3 arguments\n"; }
  void TreatmentTermeS0( std::vector<double> aVec, double dx, double grav, int NbSections); // { std::cout << "TreatmentTermeS0 with 4 arguments\n"; }
  void TreatmentSource2( std::vector<double> &aS, std::vector<double> &&aQ, std::vector<double> &&aA,
                         double aDx = 10., unsigned aNbSections = 101); //rvalue reference
  void TreatmentSource2( std::vector<double> &aS, const std::vector<double> &aQ, const std::vector<double> &aA,
                         double aDx, unsigned aNbSections);  // lvalue reference
} // End of namespace