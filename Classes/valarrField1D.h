
#pragma once

// C++ includes
#include <memory> // smart pointer
#include <string>
#include <valarray>
#include <algorithm>
#include <functional>

// boost include
//#include <boost/operators.hpp>
// App include

#include "gridLattice1D.h"

namespace vsc19
{
	/**
	 * @brief scalarField based on std numerical container for fast floating point operation.
	 *    Type that represent a function over a grid in a finite difference approximation.
	 *    std valarray support modern C++ move semantic as well math operation supported 
	 *    by such a function. 
	 */
    class valarrField //: boost::addable<valarrField>  operator+
    {
 	public:
		/**
		 * @brief Construct a field lattice form a grid
		 * 
		 * @param grid finite difference grid
		 * @param fieldname name description 
		 * @return 
		 */
		valarrField( const std::shared_ptr<gridLattice1D>& grid, 
			         std::string fieldname);

#if 0 //all those ctor are implicitly generated 
        /**
         * @brief Construct a new valarr Field object
         * 
         * @param aOther 
         */
	    valarrField( const valarrField& aOther)=default;
	
		/**
		 * @brief Assign new valarr field object
		 * 
		 * @param aOther 
		 * @return valarrField& 
		 */
		valarrField& operator= (const valarrField& aOther)=default;

		// move semantic support

		/**
		 * @brief Move copy construct
		 * 
		 * @param aOther other field to construct from
		 */
        valarrField( valarrField&& aOther)=default;

		/**
		 * @brief Move assignment construct
		 * 
		 * @param aOther other field to construct from
		 * @return valarrField& new created field  
		 */
        valarrField& operator= ( valarrField&& aOther)=default;
#endif //if 0

		/**
		 * @brief Enable access to grid-point values
		 * 
		 * @return std::valarray<double>& 
		 */
		      std::valarray<double>& values()        { return *m_gridPointValues; }
		const std::valarray<double>& values() const  { return *m_gridPointValues; }

		/**
		 * @brief Enable access to the grid
		 * 
		 * @return gridLattice1D& 
		 */
		       gridLattice1D& grid()                   { return *m_gridLattice; }
		const  gridLattice1D& grid() const             { return *m_gridLattice; }

        /**
         * @brief Field name description
         * 
         * @return std::string 
         */
		std::string name() const                     { return m_fieldName; }

		/**
		 * @brief Set the values of the field
		 * 
		 * @param aNewArray 
		 */
		void values( std::valarray<double>& aNewArray);
        
		/**
		 * @brief Unary negate operator
		 * 
		 * @return valarrField 
		 */
		valarrField operator- () const;

		/**
		 * @brief Unary plus operator
		 * 
		 * @return valarrField 
		 */
        valarrField operator+ () const;

        /**
         * @brief Math operation on field (add 2 field)
         * 
         * @param aOther 
         * @return vector field
         */
        valarrField& operator+= ( const valarrField& aOther);

        /**
         * @brief 
         * 
         * @param aOther 
         * @return valarrField& 
         */
		valarrField& operator-= ( const valarrField& aOther);

        /**
         * @brief 
         * 
         * @param aOther 
         * @return valarrField& 
         */
		valarrField& operator*= ( const valarrField& aOther);

        /**
         * @brief 
         * 
         * @param aOther 
         * @return valarrField 
         */
	//	valarrField operator/= (const valarrField& aOther);

     //   valarrField operator+ ( const valarrField& aOther);
      private:
        std::shared_ptr<gridLattice1D>         m_gridLattice;     /**< shared grid*/
        std::shared_ptr<std::valarray<double>> m_gridPointValues; /**< shared array (point values)*/
		std::string                            m_fieldName;       /**< field name*/
    };

    //
	// Math operations on scalar field
	//

	valarrField operator+ (const valarrField& aF1, const valarrField& aF2);
} // End of namespace