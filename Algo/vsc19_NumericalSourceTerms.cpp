
// C++ include
#include <iostream>
// App includes
#include "../vs19_AppConstant.hpp"
#include "../vs19_UniversalConst.hpp"
#include "../Classes/vsc19_scalarField.h"
#include "vsc19_NumericalSourceTerms.h"

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

  // return temporary (prvalue: pure reading value) to initialize variable
  auto getVectorInt() noexcept { return std::vector{1, 2, 3, 4, 5}; }
  auto asStdVector()  noexcept { return std::vector{1., 2., 3., 4., 5.}; }
  auto getBCFromTpl() noexcept { return std::make_tuple(10., 0.,10.); }

  // use Rvalue reference to modify some variable by adding b.c. in the computation phase
  // in our programming we impose b.c. by adding a ghost node, but we don't want to modify
  // the original which is a scalar field (bind temporary to rvalue reference)
  // Usage: TreatmentSource2(w_s, aQ.values().asStdVector(), aA.values().asStdVector(), ...)
  //
  //  NOTE
  //   could use the perfect forwarding (don't need to do that, overload resolution will do)
  //   Also, some of our algorithm we need to add the ghost node, as in this case, could use
  //   this version of the algorithm and pass field data (computational domain without ghost
  //   node), then add the ghost without modifying the original
  void TreatmentSource2( std::vector<double>& aS, 
      std::vector<double>&& aQ, std::vector<double>&& aA, std::vector<double>&& aH, //rvalue reference
      const std::vector<double> &n,  double aDx /*= 10.*/, unsigned aNbSections /*= 101*/)
    {
      std::cout << "Rvalue reference version, can freely modify the original (avoid copy)\n";

      assert(EMCNEILNbSections::value - 1 == aA.size()); //100
      assert(EMCNEILNbSections::value - 1 == aQ.size()); // ditto

      // Set b.c. ghost node value (need to modify our range)
      // actually we don't modify the original since we pass an rvalue reference
      // NOTE rvalue is typically temporary and so can be freely modified and avoid a copy.
      // We provide 2 version of the algoritm, one with lvalue and another with rvalue
      // (corner stone of the move semantic)
      //
      // C++17 structured binding (retrieve boundary values for the ghost nodes)
      auto &&[A, Q, H] = getBCFromTpl();          // universal reference (yes!!)
      aA.push_back(std::forward<decltype(A)>(A)); // adding ghost node to the temporary
      aQ.push_back(std::forward<decltype(Q)>(Q)); // ditto
      aH.push_back(std::forward<decltype(H)>(H)); // ditto

      // assert(11==aA.size());
      assert(EMCNEILNbSections::value == aQ.size()); // 100+1

      double TermeS0{};
      const auto Rh = 2.1;             // HydroUtils::R(A[j], B); debug purpose
      // pass-by-value?? i think so!! lambda return by-value by default
      auto ManningFormula = [=](const auto &A, const auto &Q, const auto &n)
      {
        return cGravity<double> * n * n * Q * std::fabs(Q)/(A * A * std::pow(Rh, 4. / 3.));
      };

      for (auto j = 0; j < EMCNEILNbSections::value - 1; j++) //< =99 computational nodes
      {
        //const auto g = cGravity<double>;  variable template
        // now perform calculation with new added element, for example derivatve at second-order

        auto TermeSf = ManningFormula(aA[j], aQ[j], n[j]); // Manning (friction law)

        // (Nujic algorithm based on physics assumptions)
        // S0 = average x first-derivative of H
        if (j == 0)
          TermeS0 = (0.5 * cGravity<double> * (aA[j + 1] + aA[j])) * (aH[j + 1] - aH[j]) / aDx;
        else // central scheme at second-order
          TermeS0 = (0.5 * cGravity<double> * (aA[j + 1] + aA[j - 1])) * (aH[j + 1] - aH[j - 1]) /(2. * aDx);

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
   // const auto w_grav = 9.8;

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
        return cGravity<double>*n*n*Q*std::fabs(Q) /
                     (A*A* std::pow(Rh, 4. / 3.));
      };
      auto TermeSf = ManningFormula(A[j],Q[j],n[j]);

      // (Nujic algorithm based on physics assumptions)
      // S0 = average x first-derivative of H 
      if (j == 0)
        TermeS0 = (0.5*cGravity<double>*(A[j + 1] + A[j])) * (H[j + 1] - H[j]) / dx;

      else
        TermeS0 = (0.5*cGravity<double>*(A[j + 1] + A[j - 1])) * (H[j + 1] - H[j - 1]) /
                  (2. * dx);

      S[j] = (TermeSf - TermeS0);
    }//for-loop
  }
  std::valarray<double> TreatmentSource2( const scalarField1D& aQ, const scalarField1D& aA, 
  const ListSectionFlow& aListSectFlow) 
  {
    // NOTE (design)
    //  possible implementation is to use perfect forwarding to stdVec impl. 
    //  provide to customer an interface with scalar field, but behind the 
    //  hood we stilluse old implementation. Doesn't really matter if in
    //  the future we change the impl., client won't notice it. But those 
    //  old impl are not accessible by the client.
    //  Instead of free functions like those above, could be a static class.
#if 0 // future deveopment
    // lvalue test
       std::vector w_vecA {1.2,3.5,54.2,23.4,2.};   //lvalue
       std::vector w_vecQ {11.2,32.5,4.2,3.4,2.12}; // lvalue
       fwdSrcAlgo(w_vecByRef, w_vecA, w_vecQ, n, dx, 5); //value case
#endif
    return std::valarray<double>(); // debugging purpose
  }
} // End of namespace