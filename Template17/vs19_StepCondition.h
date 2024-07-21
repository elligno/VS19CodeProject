
#pragma once

// C++ includes
#include <limits>
#include <optional>
// App includes
#include "vs19_UniversalConst.hpp"
// Numerical include
#include "Dsn/Dsn_ScalarField.h"
// jb package
#include "Classes/jb_scalarField.h"
// Sfx Package 
#include "Sfx/Sfx_FieldLattice.h"

#undef max  //compile-time error doesn't recognize max (Microsoft problem)

namespace vsc19
{
  /** Provide support for those 3 version of the scalarField
   *  no choice since we maintain these until we complete
   *  the merge. Might still need it after merge completed.
  */
  template<typename scalarField,
  typename std::enable_if<std::is_pointer<scalarField>::value>::type*>
  auto TimeStepCondition( const scalarField aU1, const scalarField aU2)
  {
    if constexpr ( !std::is_same_v<scalarField, jb::scalarField*> &&
                   !std::is_same_v<scalarField, Dsn::scalarField*> &&
                   !std::is_same_v<scalarField, Sfx::FieldLattice*>)
    {
      return std::make_optional(std::nullopt);
    }

    auto dt = std::numeric_limits<float64>::max();
    
    // start j=2, j=1 is tied node (computational domain)
    for( auto j = 2; j <= aU1->grid().getNoPoints(); j++) 
    {
      const auto V = aU2->values()(j) / aU1->values()(j);           // velocity
      const auto c = std::sqrt(cGravity<float64>*aU1->values()(j)); // unit width 
      auto dtc = aU1->grid().Delta(1) / (::fabs(V) + c);            // time step at i
      dt = (std::min)(dtc, dt);                                     // time step
    }

    //return dt;
    return std::make_optional(dt);
  }
} // End of namespace