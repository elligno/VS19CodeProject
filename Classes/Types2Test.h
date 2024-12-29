
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

    class Coord2D 
    {
        public:
        Coord2D()=default;
        constexpr Coord2D( double aXcoord, double aYCoord) : m_x{aXcoord}, m_y{aYCoord} {}

        constexpr auto getXcoord() const { return m_x;}
        constexpr auto getYcoord() const { return m_y;}

        constexpr void setXcoord( auto aXcoord) { m_x=aXcoord;}
        constexpr void setYcoord( auto aYcoord) { m_y=aYcoord;}

        // spaceship operator
        auto operator <=> ( const Coord2D& aOther) const
        {
            return (m_x <=> aOther.m_x);  // defines ordering <,<=,>, >=
        }

        private:

        double m_x;
        double m_y;
    };
} // End of namespace