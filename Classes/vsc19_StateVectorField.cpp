
#include "vsc19_StateVectorField.h"

namespace vsc19 
{
    StateVectorField::StateVectorField( std::string aName, std::string aGridPrm/*=""*/)
    : m_name{std::move(aName)}, 
      m_gridPrms{ aGridPrm },
      m_grid1D{nullptr},
      m_A{nullptr},
      m_Q{ nullptr }
    {}

    StateVectorField::StateVectorField( std::string aName, const std::tuple<uint32, float64, float64>& aGridprms)
    : m_name{ std::move(aName) },
      m_gridPrms{"d=1 [0,1] [0:99]"}, // default value?? not sure about this!!!
      m_grid1D{nullptr},
      m_A{ nullptr },
      m_Q{ nullptr }
    {
        // initialize grid from nbpts, min/max grid extend 
      m_grid1D.reset(new gridLattice1D(std::get<0>(aGridprms), std::get<1>(aGridprms), std::get<2>(aGridprms)));
      m_A.reset( new scalarField1D{ m_grid1D, std::string{ "A-Values" } });
      m_Q.reset( new scalarField1D{ m_grid1D, std::string{ "Q-Values" } });
    }
    bool StateVectorField::reset( const gridLattice1DPtr& aGrid2Reset)
    {
      if(m_A)
      {
        m_A = nullptr; // release ownership
      }
      if(m_Q)
      {
        m_Q = nullptr; // release ownership
      }
      if (m_grid1D)
      {
        m_grid1D=nullptr; //ditto
      }
      m_grid1D = aGrid2Reset;
      // take ownership of the pointer
      m_A.reset( new scalarField1D{ m_grid1D, std::string{ "A-Values" } });
      m_Q.reset( new scalarField1D{ m_grid1D, std::string{ "Q-Values" } });

      return (m_A!=nullptr && m_Q!=nullptr);
    }
} // End of namespace