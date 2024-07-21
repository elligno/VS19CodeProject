
#include "vs19_valArrField.h"

namespace vsc19 
{
  valArrField::valArrField( const std::shared_ptr<GridLattice>& aGridField, std::string aName)
  : m_name{ std::move(aName) },  // best solution, don't use string-view
	m_gridLattice{ aGridField }
  {

		// allocate the grid_point_values array:
		if( m_gridLattice->getNoSpaceDim() == 1)
			m_gridValues.reset( new dblvarray(m_gridLattice->getDivisions(1))); // set initial values to zero
  }

	valArrField::valArrField( const std::shared_ptr<GridLattice>& aGrid, std::string aName, double aValue)
	: m_name{ std::move(aName) },  // best solution, don't use string-view
	  m_gridLattice{ aGrid }
	{
		// allocate the grid_point_values array:
		if (m_gridLattice->getNoSpaceDim() == 1)
			m_gridValues.reset( new dblvarray(aValue, m_gridLattice->getDivisions(1))); // set initial values to zero
	}
	valArrField::valArrField( const std::shared_ptr<GridLattice>& aGrid, std::string aName, 
		const std::vector<double>& aValue)
	: m_name{ std::move(aName) },  // best solution, don't use string-view
	  m_gridLattice{ aGrid }
	{
		// allocate the grid_point_values array:
		if (m_gridLattice->getNoSpaceDim() == 1)
			m_gridValues.reset( new dblvarray( aValue.data(), m_gridLattice->getDivisions(1))); // set initial values to zero
	}

	void valArrField::values( dblvarray& new_array)
	{
		m_gridValues.reset(&new_array);
	}

#if 1
	valArrField operator+ ( const valArrField& aV1, const valArrField& aV2)
	{
		valArrField w_retField{
			std::make_shared<GridLattice>(aV1.grid().getDivisions(1), aV1.grid().xMin(1), aV1.grid().xMax(1)),
			std::string{"sasd"} };

		//valArrField w_tmpField{ aV1 };  copy ctor??
		auto& w_valArr = w_retField.values();
		w_valArr = aV1.values() + aV2.values();
		return w_retField;
	}
#endif // 0

	valArrField operator+ (valArrField&& aLhs, const valArrField& aRhs)
	{
		aLhs += aRhs;
		return std::move(aLhs);
	}

	valArrField operator+ ( valArrField&& aLhs, double aValue)
	{
		aLhs += aValue;
		return std::move(aLhs);

		//valArrField w_tmpField{ aV1 }; // copy ctor??
		//std::valarray<double>& w_valArr = w_tmpField.values();
		//w_valArr = aV1.values() + aValue;
		//return w_tmpField;
	}

	valArrField operator+ (double aValue, valArrField&& aRhs)
	{
		aRhs += aValue;
		return std::move(aRhs);

		//valArrField w_tmpField{ aV2 }; // copy ctor??
		//std::valarray<double>& w_valArr = w_tmpField.values();
		//w_valArr = aValue + aV2.values();
		//return w_tmpField;
	}

	valArrField operator- ( valArrField&& aLhs, const valArrField& aRhs)
	{
#if 0
		valArrField w_tmpField{ aV1 }; // copy ctor??
		std::valarray<double>& w_valArr = w_tmpField.values();
		w_valArr = aV1.values() - aV2.values();
		return w_tmpField;

#endif // 0

		aLhs -= aRhs;
		return std::move(aLhs);
	}

	valArrField operator- ( valArrField&& aLhs, double aRhs)
	{
		aLhs -= aRhs;
		return std::move(aLhs);
	}

	valArrField operator- (double aValue, valArrField&& aRhs)
	{
		aRhs -= aValue;
		return std::move(aRhs);
	}

	valArrField::iterator valArrField::begin()               { return std::begin(*m_gridValues); }
	valArrField::const_iterator valArrField::cbegin() const  { return std::begin(*m_gridValues); }
	valArrField::iterator valArrField::end()                 { return std::end(*m_gridValues);   }
	valArrField::const_iterator valArrField::cend() const    { return std::end(*m_gridValues);   }
	valArrField::reverse_iterator valArrField::rbegin()
	{
		return reverse_iterator(end());
	}

	valArrField::const_reverse_iterator valArrField::rbegin() const
	{
		return const_reverse_iterator( cend());
	}

	valArrField::reverse_iterator valArrField::rend()
	{
		return reverse_iterator(begin());
	}

	valArrField::const_reverse_iterator valArrField::rend() const
	{
		return const_reverse_iterator(cbegin());
	}

} // End of namespace