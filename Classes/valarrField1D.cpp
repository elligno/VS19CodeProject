
#include "valarrField1D.h"

namespace vsc19
{
    // ctor 
    valarrField::valarrField( const std::shared_ptr<gridLattice1D>& grid, 
			                  std::string fieldname)
    : m_gridLattice{grid},
      m_fieldName{std::move(fieldname)}
    {
        // reset shared pointer (initialized with nullptr)
        auto nbPts = m_gridLattice->getDivisions();
        auto* ptrVarr = new std::valarray<double>((*m_gridLattice).getDivisions());
        m_gridPointValues.reset(new std::valarray<double>((*m_gridLattice).getDivisions()));
    }
    void valarrField::values( std::valarray<double>& aNewArray)
    {
        m_gridPointValues.reset(&aNewArray);
    }

    valarrField valarrField::operator- () const
    {
        std::valarray<double> w_retArray(m_gridPointValues->size());
        std::transform(std::begin(*m_gridPointValues), std::end(*m_gridPointValues),
                       std::begin(w_retArray), std::negate<double>{});

        valarrField w_retField = *this;
        w_retField.values(w_retArray);

        return w_retField;
    }
    valarrField &valarrField::operator+= (const valarrField &aOther)
    {
        *m_gridPointValues += aOther.values();
        return *this;
    }
    valarrField &valarrField::operator-= (const valarrField &aOther)
    {
  	     return *this += -aOther;
    }
    valarrField &valarrField::operator*= (const valarrField &aOther)
    {
		*m_gridPointValues *= aOther.values();
		return *this;
    }
#if 0
    valarrField valarrField::operator/= (const valarrField& aOther)
    {
      // assert no division by zero
      if (std::all_of(std::begin(aOther.values()), std::end(aOther.values()),
        [](double aVal) {return aVal != 0.; }))
      {
        *m_gridPointValues /= aOther.values();
        return *this;
      }
      // std::all_of(std::begin(aOther.values()), std::end(aOther.values()),
      // [](double aVal){return aVal!=0.;}));
      return valarrField{};
    }

#endif // 0

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    valarrField operator+ ( const valarrField& aF1, const valarrField& aF2)
    {
        std::valarray<double> w_tmp = aF1.values() + aF2.values();
        // copy ctor
		valarrField w_retField {aF1};
        // reference ...
        std::valarray<double>& w_begArr = w_retField.values();
		// not sure about this one???
		std::copy(std::begin(w_tmp),std::end(w_tmp), std::begin(w_begArr));
		//w_retField.values(w_tmp);  ????

		return w_retField;
    }
} // End of namespace