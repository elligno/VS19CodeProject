
#include "vsc19_PhysicalObject.h"

namespace vsc19 {

	bool PhysicalObject::hasDimension() const
	{
		// substract left and right corner (default implementation)
		return (m_bottomLeft.m_ptCoord.first - m_upperRight.m_ptCoord.first) != 0.;
	}

	std::pair<double, double> PhysicalObject::getDimension() const
	{
		return std::pair<double, double>();
	}

	bool PhysicalObject::isUnity() const
	{
		return (m_bottomLeft.m_ptCoord.first - m_upperRight.m_ptCoord.first) == 1. &&
			   (m_bottomLeft.m_ptCoord.second - m_upperRight.m_ptCoord.second) == 1.;
	}
} // End of namespace

// check N. Josuttis C++11 std book
// std::pair<Point, Point> vs15::PhysicalObject::getExtension() const
// { 
// 	return std::make_pair<Point, Point>(m_bottomLeft,m_upperRight);
// }

// vs15::PhysicalObject::Point::Point( double aX, double aY)
// : m_x(aX),
//   m_y(aY)
// {
// 	m_ptCoord = std::make_pair(aX, aY);
// }
