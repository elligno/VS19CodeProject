#pragma once

#include <iostream>
// App include
#include "Types2Test.h"

namespace vsc19
{
  class SectionFlow // boost 'operator' library such as
  {
    // equality_comparable, less_than_comparable  
  public:

    // for now support only these 2 geometry types
    enum class SectionType { unit = 0, rectangular = 1 };

    // default ctor (if we want to use it with STL container)
    // SectionFlow() = default;
    // SectionFlow( const SectionFlow& aOther) = default;
    // SectionFlow& operator= ( const SectionFlow& aOther) = default;

    // set most of the section hydraulic parameters to default value
    // such N=0,B=1,Z=0
    SectionFlow( unsigned aId, double aX, SectionType aType = SectionType::unit);

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
    bool isUnitWidth() const { return true; }
    bool isFrictionLess() const { return true; }
    bool isFlatBottom() const { return true; }
    double X() const { return m_x; }
    double Z() const { return m_Z; }
    double H() const { return m_H; }
    double N() const { return m_N; }
    double B() const { return m_B; }
    double getS0am() const { return m_S0am; }
    double getS0av() const { return m_S0av; }
    unsigned getId() const { return m_id; }
    SectionType getType() const { return m_type; }

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
  };
} // End of namespace