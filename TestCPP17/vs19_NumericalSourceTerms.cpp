// C++ include
#include <iostream>
// 
#include "../vs19_AppConstant.hpp"
#include "vs19_NumericalSourceTerms.h"
#include "../Classes/vsc19_scalarField.h"

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

  // return temporary to initialize variable (for testing)
  auto getVectorInt() { return std::vector{1, 2, 3, 4, 5}; }
  auto asStdVector() { return std::vector{1., 2., 3., 4., 5.}; }
  auto getTpl() { return std::make_tuple(10., 0.,10.); }

  // use Rvalue reference to modify some variable by adding b.c. in the computation phase
  // in our programming we impose b.c. by adding a ghost node, but we don't want to modify
  // the original which is a scalar field (bind temporary to rvalue reference)
  // Usage: TreatmentSource2(w_s, aQ.values().asStdVector(), aA.values().asStdVector(), ...)
  //
  //  NOTE
  //   could use the perfect forwarding (don't need to do that, overload resolution will do)
  void TreatmentSource2(  std::vector<double> &aS, std::vector<double> &&aQ,
      std::vector<double> &&aA, std::vector<double> &&aH,
      const std::vector<double> &n,  double aDx /*= 10.*/, unsigned aNbSections /*= 101*/)
  {
    std::cout << "We are in the Rvalue reference version, we can freely modify the original (avoid copy)\n";

    assert(10 == aA.size());
    assert(10 == aQ.size());

    // set b.c. ghost node value
    auto &&[A, Q, H] = getTpl();                // structured binding, this an rvalue reference?? or universal reference (yes!!)
    aA.push_back(std::forward<decltype(A)>(A)); // adding ghost node to the temporary
    aQ.push_back(std::forward<decltype(Q)>(Q)); // adding ghost node to the temporary
    aH.push_back(std::forward<decltype(H)>(H)); // adding ghost node to the temporary

    // assert(11==aA.size());
    assert(11 == aQ.size());

    double TermeS0{};
    const auto w_grav=9.8;
    for (auto j = 0; j < aNbSections - 1; j++)
    {
      const auto Rh = 2.1; // HydroUtils::R(A[j], B); debug purpose
      // now perform calculation with new added element, for example derivatve at second-order
      // pass-by-value?? i think so!!
      auto ManningFormula = [=](double A, double Q, double n)
      {
        return 9.8 * n * n * Q * std::fabs(Q) /
               (A * A * std::pow(Rh, 4. / 3.));
      };

      auto TermeSf = ManningFormula(aA[j], aQ[j], n[j]); // MAnning (friction law)

      // (Nujic algorithm based on physics assumptions)
      // S0 = average x first-derivative of H
      if (j == 0)
        TermeS0 = (0.5 * w_grav * (aA[j + 1] + aA[j])) * (aH[j + 1] - aH[j]) / aDx;

      else
        TermeS0 = (0.5 * w_grav * (aA[j + 1] + aA[j - 1])) * (aH[j + 1] - aH[j - 1]) /
                  (2. * aDx);

      aS[j] = (TermeSf - TermeS0);
    } // for-loop
  }

  // taking lvalue reference to achieve the same thing, but we need to copy
  // could pass an extra argument for the ghost node value 
  void TraitementTermeSource2(
      std::vector<double> &S, const std::vector<double> &Q,
      const std::vector<double> &A, const std::vector<double> &H,
      const std::vector<double> &n, const double dx, const int NbSections,
      double B /*=1*/)
  {
    double TermeS0{};
    const auto w_grav = 9.8;

    // Adding the ghost node at the end (right far end)? not for this version
    // assume that user had already added the ghost node when calling this  
    
    // E. McNeil NbSections = 101 (N+1) but derivative taken on computational domain
    // we need to set the extra node or ghost node by some b.c. condition. Scalar 
    // field represented on computational domain (100 hundreds node), use of rvalue
    // reference to add to variable but withoput modifying the original (no copy).  
    for( auto j = 0; j < NbSections - 1; j++)
    {
      const auto Rh = 2.1; //HydroUtils::R(A[j], B); debug purpose
      //pass-by-value?? i think so!!
      auto ManningFormula = [=] (double A, double Q, double n)  
      {
        return w_grav*n*n*Q* std::fabs(Q) /
                     (A*A* std::pow(Rh, 4. / 3.));
      };
      auto TermeSf = ManningFormula(A[j],Q[j],n[j]);

      // (Nujic algorithm based on physics assumptions)
      // S0 = average x first-derivative of H 
      if (j == 0)
        TermeS0 = (0.5 * w_grav * (A[j + 1] + A[j])) * (H[j + 1] - H[j]) / dx;

      else
        TermeS0 = (0.5 * w_grav * (A[j + 1] + A[j - 1])) * (H[j + 1] - H[j - 1]) /
                  (2. * dx);

      S[j] = (TermeSf - TermeS0);
    }//for-loop
  }

  void ExamplOfUseRval()
  {
    // create a scalar field with values
    const auto w_grid1d = std::make_shared<gridLattice>(std::string{"d=1 [0,1]x[1:10]"});

  //  vsc19::gridLattice1D w_grid1D{ std::string {"d=1 [0,1]x[1:10]"} };
    scalarField w_A{ w_grid1d, std::string{"Scalar Field for variable A"}}; // created with initial values?? yes set 0
    scalarField w_Q{ w_grid1d, std::string{"Scalar Field for variable Q"}}; // ditto
 #if 1

    // some sanity check
    for( auto& w_valA : w_A.values()) //is it working? not sure!!
    {
      w_valA+=1.; // initialize field values
    }
    for( auto& w_valQ : w_Q.values()) //is it working? not sure!!
    {
      w_valQ+=.5; // initialize field values
    }
    
    // sanity check
    assert(10==w_A.values().size());
    assert(1.==w_A.values()(3));
#endif 

    // soma data for our test
    std::vector<double> w_S{EMCNEILNbSections::value}; // intialize to zero
    std::vector<double> n{EMCNEILNbSections::value};   // Manning friction coefficient
    std::vector<double> w_H{EMCNEILNbSections::value}; // water depth (flat bed) 

     // call numerical treatment version passing temporaries (prvalue: pure reading value)
    // overload resolution (nothing to do with perfect forwarding). Below it should call
    // the function with rvalue reference. 
    TreatmentSource2( w_S, w_Q.values().to_stdVector(), w_A.values().to_stdVector(), 
    std::vector<double>{EMCNEILNbSections::value}, n, 0.1, EMCNEILNbSections::value); // can freely modify the original

    // check if vector size has changed
    assert(10==w_A.values().to_stdVector().size());
    assert(10==w_Q.values().to_stdVector().size());

    std::cout << "We have tested rvalue reference\n";
  }
} // End of namespace