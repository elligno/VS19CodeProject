
#include "vsc19_SectionFlow.h"

namespace vsc19
{
  // 	SectFlow::SectFlow() 
  // 	: m_id(1), // section number id 1 as default, can be confusing
  // 	  m_x(0.),
  // 	  m_Z(0.),
  // 	  m_H(0.),
  // 	  m_N(0.),
  // 	  m_B(1.),    // unit section width
  // 	  m_S0am(0.), 
  // 	  m_S0av(0.),
  // 	  m_type(SectionType::unit)
  // 	//  m_state()
  //    {
  // 	// something to do
  //    }

  SectionFlow::SectionFlow(unsigned aId, double aX, SectionType aType/*=unit*/)
  : m_id(aId),
    m_x(aX),
    m_Z(0.),
    m_H(0.),
    m_N(0.),
    m_B(1.),        // unit section width
    m_S0am(0.),
    m_S0av(0.),
    m_type(aType)
  {
    // something to do
  }
} // End of namespace