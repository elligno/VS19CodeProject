#pragma once

#include <iostream>
// App include
#include "Types2Test.h"

using float64 = double;

namespace vsc19
{
  class SectionFlow // boost 'operator' library such as
  {
    // equality_comparable, less_than_comparable  
  public:

    // for now support only these 2 geometry types
    enum class SectionType { unit = 0, rectangular = 1 };

    // should declare another ctor with Manning, Z and H values
    // ctor above implicitly set these to zero (frictionless, flat bed and ) 
    constexpr SectionFlow( unsigned aId, float64 aX, float64 aH, float64 aZ=0., 
        float64 aN=0., SectionType aType = SectionType::unit)
        : m_id(aId),
        m_x(aX),
        m_Z(aZ),
        m_H(aH),
        m_N(aN),
        m_B(1.), // unit section width
        m_S0am(0.),
        m_S0av(0.),
        m_type(aType)
    {}

    // section parameters or hydraulic parameters
    unsigned m_id;       // section number id
    double m_x;          // x-coordinate
    double m_Z;          // bottom thalweg or topography?? an array
    double m_H;          // unit width, otherwise an array
    double m_N;          // Manning coefficient
    double m_B;          // section width
    double m_S0am;       // some parameters from E. McNeil code
    double m_S0av;       // some parameters from E. McNeil code
    SectionType m_type;  // section geometry type
    Coord2D m_sectCoord;
    
    // default implementation
    SectionType getSectionType() const { return SectionType::unit; }
    constexpr bool isUnitWidth() const { return true; }
    constexpr bool isFrictionLess() const { return true; }
    constexpr bool isFlatBottom() const { return true; }
    constexpr double X() const { return m_x; }
    constexpr double Z() const { return m_Z; }
    constexpr double H() const { return m_H; }
    constexpr double N() const { return m_N; }
    constexpr double B() const { return m_B; }
    constexpr double getS0am() const { return m_S0am; }
    constexpr double getS0av() const { return m_S0av; }
    constexpr unsigned getId() const { return m_id; }
    constexpr SectionType getType() const { return m_type; }

    // setter section properties
    void setH(double aH) { m_H = aH; }
    void setN(double aN) { m_N = aN; }
    void setZ(double aZ) { m_Z = aZ; }
    void setB(double aB) { m_B = aB; }
    void setSectionUnitWidth() { m_B = 1.; }

    // want to use it with mem_fn as i use to do in the ElgoProjects08 projects
    void printSectionInfo() const
    {
      // print some of the section parameters for debugging purpose
      std::cout << "Section id :" << m_id << " " << "X: " << m_x << " " << "Z: " << m_Z << " " << "H: "
        << m_H << " " << "N: " << m_N << " " << "B: " << m_B << " " << "S0am: " << m_S0am << " "
        << "S0av: " << m_S0av << "\n";
    }
    // comparison operator (==, mapped by default !=)
    bool operator== (const SectionFlow& aOther) const 
    {
      return (m_id==aOther.m_id) && (m_x==aOther.m_x);
    }
    // relational operator (<,<=,>,>=)
    std::partial_ordering operator<=> ( const SectionFlow& aOther) const
    {
      auto cmp_id = m_id <=> aOther.m_id;
      if( cmp_id !=0 )
      {
        return cmp_id; // strong ordering converted to return type
      }
      return m_x <=> aOther.m_x; // partial ordering for double
    }
  };
} // End of namespace