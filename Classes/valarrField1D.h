
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
    class scalarField1D
    {
		public:

            /** Smart pointer over the grid*/ 
		  //  using gridLattice1DPtr = std::shared_ptr<gridLattice1D>;

			/**
			 * @brief alias template
			 * @tparam T type
			 */
//			template<typename T>
//			using dblvarray = std::valarray<T>;

			/**
			 * @brief STL-container like interface (range)
			 */
		//	using dblvarray = std::valarray<double>;
			using value_type = std::valarray<double>::value_type;
			using pointer = value_type *;
			using iterator = pointer;
			using const_pointer = const value_type *;
			using const_iterator = const_pointer;
			using reference = value_type &;
			using const_reference = const value_type &;
			using range_diff_type = ptrdiff_t;

			// Replace all the lines above (next version or release)
			// SFX_DECLARE_CONTAINER( float64=aType)
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;

			/**
			 * @brief Iterator access
			 * @return iterator
			 */
			const_iterator cbegin() const;
			iterator begin();
			const_iterator cend() const;
			iterator end();
			const_reverse_iterator rbegin() const;
			reverse_iterator rbegin();
			const_reverse_iterator rend() const;
			reverse_iterator rend();

		public:
			/**
			 * @brief Construct a field lattice form a grid
			 *
			 * @param grid finite difference grid
			 * @param fieldname name description
			 * @return
			 */
			scalarField1D( const std::shared_ptr<gridLattice1D>& grid,
						std::string fieldname);

            /**
			 * @brief Construct a field lattice form a grid and initial values
			 *
			 * @param grid finite difference grid
			 * @param fieldname name description
			 * @param aInitValues field initial values 
			 * @return
			 */
		//	scalarField1D( const std::shared_ptr<gridLattice1D>& aGrid, std::string aName, 
		//	const std::vector<double>& aInitValues);

#if 0   // all those ctor are implicitly generated (copy, assignment, move copy and assignment)
        /**
         * @brief Construct a new valarr Field object
         * 
         * @param aOther 
         */
	    scalarField1D( const scalarField1D& aOther)=default;
	
		/**
		 * @brief Assign new valarr field object
		 * 
		 * @param aOther 
		 * @return scalarField1D& 
		 */
		scalarField1D& operator= (const scalarField1D& aOther)=default;

		// move semantic support

		/**
		 * @brief Move copy construct
		 * 
		 * @param aOther other field to construct from
		 */
        scalarField1D( scalarField1D&& aOther)=default;

		/**
		 * @brief Move assignment construct
		 * 
		 * @param aOther other field to construct from
		 * @return scalarField1D& new created field  
		 */
        scalarField1D& operator= ( scalarField1D&& aOther)=default;
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
		std::string name() const                       { return m_fieldName; }

		/**
		 * @brief Set the values of the field
		 * 
		 * @param aNewArray 
		 */
		void values( std::valarray<double>& aNewArray);
        
		/**
		 * @brief Unary negate operator
		 * 
		 * @return scalarField1D 
		 */
		scalarField1D operator- () const;

		/**
		 * @brief Unary plus operator
		 * 
		 * @return scalarField1D 
		 */
        scalarField1D operator+ () const;

        /**
         * @brief Math operation on field (add 2 field)
         * 
         * @param aOther 
         * @return vector field
         */
        scalarField1D& operator+= ( const scalarField1D& aOther);

        /**
         * @brief 
         * 
         * @param aOther 
         * @return scalarField1D& 
         */
		scalarField1D& operator-= ( const scalarField1D& aOther);

        /**
         * @brief 
         * 
         * @param aOther scalar field to multiply 
         * @return scalarField1D& 
         */
		scalarField1D& operator*= ( const scalarField1D& aOther);

        /**
         * @brief 
         * 
         * @param aOther 
         * @return scalarField1D 
         */
         scalarField1D& operator*= ( double aDblVal);
         
	     //	scalarField1D operator/= (const scalarField1D& aOther);

      private:
        std::shared_ptr<gridLattice1D>        m_gridLattice;     /**< shared grid*/
        std::shared_ptr<std::valarray<double>> m_gridPointValues; /**< shared array (point values)*/
		std::string                           m_fieldName;       /**< field name*/
    };

    //
	// Math operations on scalar field
	//

	scalarField1D operator+ (const scalarField1D& aF1, const scalarField1D& aF2);
	scalarField1D operator- (const scalarField1D& aF1, const scalarField1D& aF2);
} // End of namespace