#pragma once

// boost include
//#include <boost/operators.hpp>

// C++ includes
#include <utility> //std::pair
#include <compare> //C++20 <=> operator

namespace vsc19 
{
	// Testing some Boost stuff to be used in our programming environment
  // Base class should be non-copyable/assignable
	class PhysicalObject //: public boost::equality_comparable<PhysicalObject>, // strickly comparable
		                  // public boost::less_than_comparable<PhysicalObject> // less than comparable
	{                     // now implemented with spaceship operator C++20
		struct Point
		{
		   constexpr Point( double aX, double aY) // .cpp file have an error
		   : m_ptCoord(aX, aY),
   		      m_x(aX),
			 m_y(aY)
			{}

            // C++20 spaceship ooerator
			auto operator<=> ( const Point &aPt) const = default;

			constexpr double getX() const { return m_x; }
			constexpr double getY() const { return m_y; }

			std::pair<double /*x*/, double /*y*/> m_ptCoord;
			double m_x;
			double m_y;
		};

	public:
		explicit PhysicalObject( double aX); //??
		// class interface
		bool hasDimension() const;
		std::pair<double, double> getDimension() const;
		virtual bool isUnity() const = 0;
		double getX() const     { return m_X; }
		void   setX( double aX) { m_X = aX;   }
        std::pair<Point, Point> getExtension() const { return { Point(0.,0.),Point(1.,1.) }; }

	private:

    // need to be defined when using STL algorithm such as find
    // need to compare 2 objects

	/** Define operator == and != */
    bool operator == ( const PhysicalObject& aObj) const
    {
    //   return (m_bottomLeft.m_x == aObj.m_bottomLeft.m_x) && 
	//   (m_bottomLeft.m_y == aObj.m_bottomLeft.m_y) && 
	//   (m_upperRight.m_x == aObj.m_upperRight.m_x) && 
	//   (m_upperRight.m_y == aObj.m_upperRight.m_y);
	return (m_bottomLeft == aObj.m_bottomLeft) && 
	(m_upperRight == aObj.m_upperRight);
    }

	// compare member by member, the operator == above will use default implementation
	// don't need to define since it won't be used (default will be trigger)
    /** Define operator <, <=, >=, > */
    auto operator <=> (const PhysicalObject& aObj) const = default;
	// {
    //    return (m_bottomLeft <=> aObj.m_bottomLeft) &&
	//    (m_upperRight <=> aObj.m_upperRight);
	// //   ;
	// }
		// since C++11 can compare 2 pair
		// friend bool operator== ( const Point& aPt1, const Point& aPt2)
		// {
		// 	return aPt1.m_ptCoord == aPt2.m_ptCoord;
		// }

		// friend bool operator!= ( const Point& aPt1, const Point& aPt2)
		// {
		// 	return !(aPt1 == aPt2);
		// }

		// friend bool operator < ( const Point& aPt1, const Point& aPt2)
		// {
		// 	return aPt1.m_ptCoord < aPt2.m_ptCoord;
		// }

		// friend bool operator > ( const Point& aPt1, const Point& aPt2)
		// {
		// 	return aPt2 < aPt1;
		// }

		// // what about other operators such >=,<=?? can we express them
		// friend bool operator<= ( const Point& aPt1, const Point& aPt2)
		// {
		// 	return aPt1.m_ptCoord <= aPt2.m_ptCoord;
		// }
		// friend bool operator>= (const Point& aPt1, const Point& aPt2)
		// {
		// 	return aPt2.m_ptCoord <= aPt1.m_ptCoord;
		// }

    unsigned m_id{};
    double m_X {};       /**< default initialization*/
		Point m_bottomLeft;  /**< */ 
		Point m_upperRight;  /**< */
	};
} // End of namespace