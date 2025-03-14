#pragma once

// C++ include
#include <iostream>
// App include
//#include "Types2Test.h"

using float64 = double;

namespace vsc19
{
  class SectionFlow
  {
    private:
      /// see Scot Meyer's Effective Modern C++ 
      class Coord2D 
      {
      public:
        Coord2D() = default;
        constexpr Coord2D(double aZCoord, double aDcoord) : m_z{aZCoord}, m_d{aDcoord} {}
        constexpr auto getZcoord() const { return m_z; }
        constexpr auto getDcoord() const { return m_d; }
        constexpr void setXcoord(auto aZcoord) { m_z = aZcoord; }
        constexpr void setYcoord(auto aDcoord) { m_d = aDcoord; }

        // spaceship operator (equality and relational)
        auto operator<=>(const Coord2D &aOther) const = default;

      private:      // transversal section coordinates
        double m_z; /**< ...*/
        double m_d; /**< ...*/
      };

  public:
    // for now support only these 2 geometry types
    enum class SectionType { unitWidth = 0, rectangular = 1 };

    // should declare another ctor with Manning, Z and H values
    // ctor above implicitly set these to zero (frictionless, flat bed and ) 
    constexpr SectionFlow( unsigned aId, float64 aX, float64 aH, float64 aZ=0., 
        float64 aN=0., SectionType aType = SectionType::unitWidth)
        : m_id(aId), // section index
        m_x(aX),  // scetion x-coord
        m_Z(aZ),  // bottom topography
        m_H(aH),   // water depth
        m_N(aN),  // manning coeff
        m_B(1.),     // unit section width
        m_S0am(0.),  // flat bed
        m_S0av(0.),  // ditto
        m_type(aType) 
    {}
    // section parameters or hydraulic parameters
    unsigned m_id;        /**< section number id*/ 
    float64 m_x;          /**< x-coordinate*/ 
    float64 m_Z;          /**< bottom thalweg or topography?? an array*/ 
    float64 m_H;          /**< unit width, otherwise an array*/ 
    float64 m_N;          /**< section Manning coefficient*/
    float64 m_B;          /**< section width ... to be completed*/
    float64 m_S0am;       /**< bottom slope upstream*/
    float64 m_S0av;       /**< bottom slope downstream*/ 
    SectionType m_type;  /**< section geometry type*/ 
    Coord2D m_sectCoord; /**< 2-dimensional coordinate section flow*/
    
    // default implementation
    constexpr SectionType getSectionType() const { return SectionType::unitWidth; }
    constexpr bool isUnitWidth() const { return true; }
    constexpr bool isFrictionLess() const { return true; }
    constexpr bool isFlatBottom() const { return true; }
    constexpr float64 X() const { return m_x; }
    constexpr float64 Z() const { return m_Z; }
    constexpr float64 H() const { return m_H; }
    constexpr float64 N() const { return m_N; }
    constexpr float64 B() const { return m_B; }
    constexpr float64 getS0am() const { return m_S0am; }
    constexpr float64 getS0av() const { return m_S0av; }
    constexpr unsigned getId() const { return m_id; }
    constexpr SectionType getType() const { return m_type; }

    // setter section properties
    constexpr void setH(double aH) { m_H = aH; }
    constexpr void setN(double aN) { m_N = aN; }
    constexpr void setZ(double aZ) { m_Z = aZ; }
    constexpr void setB(double aB) { m_B = aB; }
    constexpr void setSectionUnitWidth() { m_B = 1.; }

    // want to use it with mem_fn as i use to do in the ElgoProjects08 projects
    // NOTE C++20 range library algorithm
    // Usage:
    //  use std invoke C++17 (check values)
    //  std::ranges::for_each(myListOfSectionFlow, &SectionFlow::printSectionInfo);
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