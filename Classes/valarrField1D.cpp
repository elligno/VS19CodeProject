
#include "valarrField1D.h"

namespace vsc19
{
    // ctor 
    scalarField1D::scalarField1D( const std::shared_ptr<gridLattice1D>& grid, 
			                  std::string fieldname)
    : m_gridLattice{grid},
      m_gridPointValues{nullptr},
      m_fieldName{std::move(fieldname)}
    {
        // reset shared pointer (initialized with nullptr)
        auto nbPts = m_gridLattice->getDivisions();
        auto* ptrVarr = new std::valarray<double>((*m_gridLattice).getDivisions());
        m_gridPointValues.reset(new std::valarray<double>((*m_gridLattice).getDivisions()));
     }
    // scalarField1D::scalarField1D( const std::shared_ptr<gridLattice1D>& aGrid, std::string aName, 
		// 	const std::vector<double>& aValues)
    // : m_gridLattice{grid},
    //   m_gridPointValues{nullptr},
    //   m_fieldName{std::move(fieldname)}
    // {
    //   m_gridPointValues.reset( new std::valarray<double>( aValues.data(), (*m_gridLattice).getDivisions()));
    // }
    void scalarField1D::values( std::valarray<double>& aNewArray)
    {
        m_gridPointValues.reset(&aNewArray);
    }

    scalarField1D scalarField1D::operator- () const
    {
        std::valarray<double> w_retArray(m_gridPointValues->size());
        std::transform(std::begin(*m_gridPointValues), std::end(*m_gridPointValues),
                       std::begin(w_retArray), std::negate<double>{});

        scalarField1D w_retField = *this;
        w_retField.values(w_retArray);

        return w_retField;
    }
    scalarField1D &scalarField1D::operator+= (const scalarField1D &aOther)
    {
      *m_gridPointValues += aOther.values();
      return *this;
    }
    scalarField1D &scalarField1D::operator-= (const scalarField1D &aOther)
    {
  	  return *this += -aOther;
    }
    scalarField1D &scalarField1D::operator*= (const scalarField1D &aOther)
    {
      *m_gridPointValues *= aOther.values();
		  return *this;
    }
    scalarField1D &scalarField1D::operator*= ( double aDblVal)
    {
      *m_gridPointValues *= aDblVal;
      return *this;
    }
#if 0 // return std::optional<scalarField1D>
    scalarField1D scalarField1D::operator/= (const scalarField1D& aOther)
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
      return scalarField1D{};
    }

#endif // 0

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    scalarField1D operator+ ( const scalarField1D& aF1, const scalarField1D& aF2)
    {
        auto w_tmp = aF1.values() + aF2.values();

        const auto& aa = aF1.values();
        const auto& bb = aF2.values();
        
        auto nx = aF1.grid().getDivisions();
        auto xmin = aF1.grid().xMin();
        auto xmax = aF1.grid().xMax();
        auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(nx,xmin,xmax);

        // copy ctor
		    scalarField1D w_retField {grid1Dptr,std::string{"return field"}};
        // reference ...
        auto& w_begArr = w_retField.values();
		    // not sure about this one???
		    std::copy(std::begin(w_tmp),std::end(w_tmp), std::begin(w_begArr));
		    //w_retField.values(w_tmp);  ????

		    return w_retField;
    }

    // this is tempoarary fix for now, just testing
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    scalarField1D operator-(const scalarField1D &aF1, const scalarField1D &aF2)
    {
      auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
      scalarField1D w_retField{ grid1Dptr, std::string{"return field"}};

      auto &dblArr = w_retField.values();
      std::transform( std::begin(aF1.values()), std::end(aF1.values()),
                      std::begin(aF2.values()), std::begin(dblArr), std::minus<double>{});

      return w_retField;
    }

   	scalarField1D operator+ (const scalarField1D& aF1, double aDbl)
    {
      auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
      scalarField1D w_retField{ grid1Dptr, std::string{"return field"}};

      using namespace std::placeholders;
      auto &dblArr = w_retField.values();
      std::transform( std::begin(aF1.values()), std::end(aF1.values()),
                      std::begin(dblArr), std::bind(std::plus<double>{}, _1, aDbl));

      return w_retField;

    }
    
	  scalarField1D operator+ (double aDbl, const scalarField1D& aF1)
    {
      return aF1+aDbl;
    }

	  scalarField1D operator- (const scalarField1D& aF1, double aDbl)
    {
      auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
      scalarField1D w_retField{grid1Dptr, std::string{"return field"}};

      using namespace std::placeholders;
      auto &dblArr = w_retField.values();
      std::transform(std::begin(aF1.values()), std::end(aF1.values()),
                     std::begin(dblArr), std::bind(std::minus<double>{}, _1, aDbl));

      return w_retField;
    }

    scalarField1D operator- (double aDbl, const scalarField1D& aF1)
    {
      auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
      scalarField1D w_retField{grid1Dptr, std::string{"return field"}};

      using namespace std::placeholders;
      auto &dblArr = w_retField.values();
      std::transform(std::begin(aF1.values()), std::end(aF1.values()),
                     std::begin(dblArr), std::bind(std::minus<double>{}, aDbl, _1));

      return w_retField;
    }

   	scalarField1D operator* (const scalarField1D& aF1, double aDbl)
    {
      auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
      scalarField1D w_retField{grid1Dptr, std::string{"return field"}};

      using namespace std::placeholders;
      auto &dblArr = w_retField.values();
      std::transform(std::begin(aF1.values()), std::end(aF1.values()),
                     std::begin(dblArr), std::bind(std::multiplies<double>{}, _1, aDbl));

      return w_retField;
    }

	  scalarField1D operator* (double aDbl, const scalarField1D& aF1)
    {
      return aF1*aDbl;
    }

    scalarField1D::iterator scalarField1D::begin()              { return std::begin(*m_gridPointValues); }
    scalarField1D::const_iterator scalarField1D::cbegin() const { return std::begin(*m_gridPointValues); }
    scalarField1D::iterator scalarField1D::end()                { return std::end(*m_gridPointValues); }
    scalarField1D::const_iterator scalarField1D::cend() const   { return std::end(*m_gridPointValues); }

    scalarField1D::reverse_iterator scalarField1D::rbegin()     { return reverse_iterator(end());      }

    scalarField1D::const_reverse_iterator scalarField1D::rbegin() const
    {
      return const_reverse_iterator(cend());
    }

    scalarField1D::reverse_iterator scalarField1D::rend()
    {
      return reverse_iterator(begin());
    }

    scalarField1D::const_reverse_iterator scalarField1D::rend() const
    {
      return const_reverse_iterator(cbegin());
    }
} // End of namespace