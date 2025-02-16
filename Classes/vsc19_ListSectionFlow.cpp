
// C++ include
#include <cassert>
#include <functional> //mem_fn
// app include
#include "vsc19_ListSectionFlow.h"

namespace vsc19
{
  void ListSectionFlow::add( const SectionFlow& aSectF)
  {
    // check for capacity and and size
    m_listOfSect.push_back(aSectF);
  }

  void ListSectionFlow::add( SectionFlow&& aSectF)
  {
    // check for capacity and and size
    m_listOfSect.push_back(std::move(aSectF));
  }
} // End of namespace