
#pragma once

#include <compare> // spaceship operator (since no other C++ include)
#include <format>  // format feature c++20

namespace vsc19 
{
    // Just type to test C++20 featufes of spaceship operator
    class Type 
    {
        public:
        [[nodiscard]] auto operator ==( const Type& aOther) const noexcept 
        {
            return (m_floatVal==aOther.m_floatVal) && (m_valDbl == aOther.m_valDbl);
        } 

        std::strong_ordering operator <=> ( const Type& aOther) const
        {
            return m_floatVal == aOther.m_floatVal ?  std::strong_ordering::equal :
             m_floatVal < aOther.m_floatVal ? std::strong_ordering::less :
             std::strong_ordering::greater;           
        }

        void assign( const auto& aOther) { std::cout << std::format("This a test with C++20 {} fmt", aOther);}

        private:
        float m_floatVal;
        double m_valDbl;
    };

    /// see Scot Meyer's Effective Modern C++ 
    class Coord2D 
    {
      public:
        Coord2D()=default;
        constexpr Coord2D( double aZCoord, double aDcoord) : m_z{aZCoord}, m_d{aDcoord} {}
        constexpr auto getZcoord() const { return m_z;}
        constexpr auto getDcoord() const { return m_d;}
        constexpr void setXcoord( auto aZcoord) { m_z=aZcoord;}
        constexpr void setYcoord( auto aDcoord) { m_d=aDcoord;}

        // spaceship operator (equality and relational)
        auto operator <=> ( const Coord2D& aOther) const = default;
 
      private: // transversal section coordinate
        double m_z; /**< */
        double m_d; /**< */
    };
} // End of namespace