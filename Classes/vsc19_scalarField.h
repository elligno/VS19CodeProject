#pragma once

// C++ includes
#include <string>
#include <memory>    // shared pointer
// library includes
#include "vsc19_RealNumArray.h"
#include "vsc19_gridLattice.h"

// shall use another namespace (when migrating to VS19 with C++17 support)
// SfxBaseTypes library
namespace vsc19 
{
  /** Class scalarField is an abstraction of the numerical
   *  concept of a field, a scalar field over a grid.
  */
	class scalarField
	{
    using float64 = double;

public:
 //   using gridlatticeptr = std::shared_ptr<GridLattice>;

   /**
    * @brief alias template
    * @tparam T type
   */
  //  template<typename T>
   // using numarray = RealNumArray<T>;

    /**
     * @brief STL-container like interface
    */
    using numarray = RealNumArray<double>;
    using value_type = numarray::value_type;
    using pointer = value_type*;
    using iterator = pointer;
    using const_pointer = const value_type*;
    using const_iterator = const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using range_diff_type = ptrdiff_t;

    // Replace all the lines above (next version or release)
    //		SFX_DECLARE_CONTAINER( float64=aType)
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

	private:
		std::shared_ptr<gridLattice> grid_lattice;       /**< */
		std::shared_ptr<numarray>    grid_point_values;  /**< */
		std::string                  fieldname;          /**< */
	
  public:
    // need it from vector field class
    //scalarField() = default;

    // copy ctor
    //scalarField( const scalarField& aOther) = default;

    // assignment ctor ?? default will do i guess.
    //scalarField& operator= ( const scalarField& aOther) = default;

    // destructor
    //~scalarField() = default;

		// make a field from a grid and a fieldname
		scalarField( const std::shared_ptr<gridLattice>& aGrid, 
			std::string aFieldName);

		// enable access to grid-point values
		      RealNumArray<double>& values()        { return *grid_point_values; }
		const RealNumArray<double>& values() const  { return *grid_point_values; }

		// enable access to the grid
		      gridLattice& grid()                   { return *grid_lattice; }
		const gridLattice& grid() const             { return *grid_lattice; }

		std::string name() const                    {return fieldname;}

		// set the values of the field  
		void values( RealNumArray<double>& new_array);
    void values( RealNumArray<double>&& new_array); // move semantic

    // printing the field values
    void print( std::ostream& aValues2Print);

    // conversion operator (support to rhs discretization args)
    explicit operator std::vector<double> const () 
    { return std::vector<double> { grid_point_values->to_stdVector()}; }

    // math operations on field
    scalarField& operator+= ( const scalarField& aOther);
    scalarField& operator-= ( const scalarField& aOther);
    scalarField& operator*= ( const float64 aVal);
	};
} // End of namespace