#pragma once

// C++ includes
#include <tuple>
#include <optional>
// App include
#include "valarrField1D.h"

using float64 = double;
using uint32 = unsigned int;

namespace vsc19
{
  /**
   * @brief StateVectorField class is a 2-component scalarField1D 
   * representing state variables over the grid. It describe the state 
   * of the physical system that has a value for each point in space and time.
  */
  class StateVectorField 
  {
  public:
    /**
     * @brief Aliases to make code readable 
    */
    using gridLattice1DPtr = std::shared_ptr<gridLattice1D>;
    using fieldLatticePtr = std::shared_ptr<scalarField1D>;
    using StateVector = std::pair<fieldLatticePtr, fieldLatticePtr>; // U=(A,Q)

  public:
    /**
     * @brief field component
    */
    enum class eFieldComponent {A,Q};
  public:
    /**
     * @brief Default field construct
     * @param aName field name decription
     * @param aGrid string representtion of the grid
    */
    StateVectorField( std::string aName, std::string aGridPrm="");
    /**
     * @brief construct from tuple values 
     * @param aName field name description
     * @param aGridprms (nb grid points, min, max)
    */
    StateVectorField( std::string aName, const std::tuple<uint32, float64, float64>& aGridprms);
    // : m_name{ std::move(aName) },
    //   m_gridPrms{"d=1 [0,1] [0:99]"},
    //   m_grid1D{nullptr},
    //   m_A{ nullptr },
    //   m_Q{ nullptr }
    // {
    //   m_grid1D.reset(new Sfx::gridLattice1D(std::get<0>(aGridprms), std::get<1>(aGridprms), std::get<2>(aGridprms)));
    //   m_A = std::make_shared<Sfx::scalarField1D>(m_name,m_grid1D);
    // }
    /**
     * @brief Compatibility with legacy code
     * @param aName field name description
     * @param aStateVec pair of scalar field
    */
    explicit StateVectorField( const StateVector& aStateVec) 
    : m_name{ std::string{"pair-like"}},
      m_gridPrms{ "d=1 [0,1] [0:99]" },
  //    m_grid1D{ nullptr },
      m_A{aStateVec.first},
      m_Q{aStateVec.second}
    {}
    bool reset( const gridLattice1DPtr& aGrid2Reset);
    /**
     * @brief name
     * @return field name description
    */
    std::string name() const { return m_name; }
    /**
     * @brief grid dimension represented as string 
     * @return grid parameters as string
    */
    std::string gridPrms() const { return m_gridPrms; }
    /**
     * @brief maximum 
     * @param aFieldComp field component 
     * @return maximum value of the field  
    */
    std::optional<float64> max(eFieldComponent aFieldComp) 
    {
      switch (aFieldComp)
      {
      case eFieldComponent::A:
        return std::optional<float64>(m_A->values().max());
        break;
      case eFieldComponent::Q:
        return std::optional<float64>(m_Q->values().max());
      default:
        return std::nullopt;
        break;
      }
    }
    /**
     * @brief finite difference grid
     * @return grid lattice
    */
		// gridLattice1D& grid()             { return *m_grid1D; }
		// const gridLattice1D& grid() const { return *m_grid1D; }
		/**
		 * @brief A-component grid point values
		 * @return array of values
		*/
		std::valarray<double>& Avalues() { return m_A->values(); }
		/**
		 * @brief A-component grid point values
		 * @return array of values
		*/
		const std::valarray<double>& Avalues() const { return m_A->values(); }
    /**
     * @brief Q-component grid point values
     * @return array of values 
    */
    std::valarray<double>& Qvalues() { return m_Q->values(); }
		/**
		 * @brief Q-component grid point values
		 * @return array of values
		*/
		const std::valarray<double>& Qvalues() const { return m_Q->values();}
    /**
     * @brief A-component of vector field
     * @return reference to smart pointer
    */
    fieldLatticePtr getA() const { return m_A; }
    /**
     * @brief A-component
     * @return scalarField1D
    */
    scalarField1D getAfield() const { return *m_A; }
    /**
     * @brief A-component
     * @return scalarField1D
    */
    scalarField1D& getAfield()      { return *m_A; }
    /**
     * @brief Q-component of vector field
     * @return smart pointer pointed-to object
    */
    fieldLatticePtr getQ() const   { return m_Q; }
    /**
     * @brief Q-component
     * @return scalarField1D
    */
    scalarField1D getQfield() const { return *m_Q; }
    /**
     * @brief Q-component 
     * @return scalarField1D
    */
    scalarField1D& getQfield()      { return *m_Q; }
    /**
     * @brief support to legacy code compatibility
     * @return pair of scalar field (StateVector type in legacy code)
    */
    explicit operator StateVector() const noexcept { return {m_A,m_Q}; }
    /**
     * @brief check if vector field is null
     * @return true/false on success
    */
    bool isNull() {return m_A==nullptr && m_Q==nullptr;}
  private:
    std::string m_name;         /**< field name*/
    std::string m_gridPrms;     /**< grid parameters*/
//    gridLattice1DPtr m_grid1D;  /**< grid lattice*/
    fieldLatticePtr m_A;        /**< A-component*/
    fieldLatticePtr m_Q;        /**< Q-component*/
  };
} // End of namespace