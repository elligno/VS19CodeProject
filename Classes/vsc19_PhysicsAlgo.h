#pragma once

// C++ includea
#include <iostream>
#include <list>
// Test includes
#include "vsc19_SectionFlow.h"

namespace vsc19
{    
  class RhsDiscretization {

    public:
      void traitementTerme2() {std::cout << "Source Treatment Nujic Implementation\n";}
  };

   class PhysicsAlgo 
   {
    public:
      virtual void calculate()=0;
   };

   class NujicAlgo : public PhysicsAlgo {
    public:
     NujicAlgo( std::string aName, const std::list<SectionFlow>& aListSectFlow)
     : m_alphaCoeff{0.},
       m_name{std::move(aName)},
       m_rhsDiscrTerms{nullptr}
     {}
    NujicAlgo( std::string aName, RhsDiscretization* aRhsDiscr,
    const std::list<SectionFlow>& aListSectFlow, double aAlphaCoeff=0.4)
    : m_alphaCoeff{aAlphaCoeff},
      m_name{std::move(aName)},
      m_rhsDiscrTerms{nullptr}
      {}

    template<typename T>
    NujicAlgo(const T& aType) {}

      void calculate() override {std::cout << "Nujic Physics Algorithm in Action\n";};
    private:
      double m_alphaCoeff;
      std::string m_name;
      RhsDiscretization* m_rhsDiscrTerms;
      std::list<SectionFlow> m_listSectionFlow;
   };

   class EMCNeilAlgo : public PhysicsAlgo 
   {
    public:
    EMCNeilAlgo(std::string aName, /*RhsDiscretization* aRhsDiscr,*/ const std::list<SectionFlow>& aListSectFlow)
    : m_name{std::move(aName)},
      m_listSectionFlow{aListSectFlow}
     // m_rhsDiscrTerms{nullptr}
    {}
    public:
    void calculate() override {std::cout << "EMcNeil Physics Algorithm in Action\n";};
    private:
      std::string m_name;
      std::list<SectionFlow> m_listSectionFlow;
      //RhsDiscretization* m_rhsDiscrTerms;
   };
} // namespace vsc19