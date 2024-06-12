
#pragma once

// C++ includes
#include <iostream>
#include <string>

// Mathematical array
#include "vs19_NumArray.hpp"

namespace vsc19 {

	using int32 = int;
	using float64 = double;

	/**
	 * @brief GridLattice describes the domain the PDE will be applied to
	*/
	class GridLattice {

	  public:
		  // default ctor (2D by default)
		  GridLattice();

		  // 1D grid (Added by Jean Belanger Jan. 26, 2007) 
		  GridLattice( int32 nx, float64 aXmin, float64 aXmax);

		  // destructor
		  ~GridLattice() {/*Nothing to do*/}

		  // parametrized ctor (2D grid)
		  GridLattice( int32 nx,      int32 ny, 
			             float64 xmin_, float64 xmax_, 
			             float64 ymin_, float64 ymax_);

		  /**
		   * @brief returns space dmension
		   * @return int
		  */
		  int32 getNoSpaceDim() const;

		  // Returns x-y min and max values (each dimension)
		  float64 xMin( int dimension) const;
		  float64 xMax( int dimension) const;  
		  
	 	  /**
	 	   * @brief getter
	 	   * @return double
	 	  */
	 	  float64 xMin() const;
		  float64 xMax() const;
		  float64 yMin() const;
		  float64 yMax() const;

		  // get the number of points in each dimension
		  int32 getDivisions( int32 i) const; 

		  // get the total number of points in the grid.
		  int32 getNoPoints() const;       

		  // grid spacing
		  float64 Delta( int32 dimension) const;

		  // 
		  float64 getPoint( int32 dimension, int32 index);

		  // get base values
		  int32 getBase( int32 dimension) const;   
		
		  // upper limit of array
		  int32 getMaxI( int32 dimension) const;   

		  // scan parameters from string
		  void scan( const std::string& init_string);  

		  // stream operator (overload)
		  friend std::ostream & operator << ( std::ostream & os, GridLattice & aGrid);

	  private:

		  // currently limited to two dimensions
	    static constexpr int MAX_DIMENSIONS = 2;

		  // variables defining the size of the grid
		  float64 min[MAX_DIMENSIONS];         // min coordinate values in each dimension
		  float64 max[MAX_DIMENSIONS];         // max coordinate values in each dimension
		  int32   division[MAX_DIMENSIONS];    // number of points in each dimension
		  int32   dimensions;                  // number of dimensions
	};

	// ======================================
	//         Inline definitions 
	// ======================================

	// _____________________________________________________________
	//
	inline float64 GridLattice::Delta( int32 dimension) const
	{
		return (max[dimension-1] - min[dimension-1])/float64(division[dimension-1]);
	}

	// _____________________________________________________________
	//
	inline float64 GridLattice::getPoint( int32 dimension, int32 index)
	{
		return min[dimension-1] + (Delta(dimension)*(index - 1));
	}

	// _____________________________________________________________
	//
	inline int32 GridLattice:: getNoSpaceDim() const
	{ return dimensions;}

	// _____________________________________________________________
	//
	inline int32 GridLattice::getDivisions( int32 i) const
	{ return division[i-1];}

	// _____________________________________________________________
	//
	inline int32 GridLattice::getBase( int32 i) const
	{ return 1;}

	// _____________________________________________________________
	//
	inline int32 GridLattice::getMaxI(int32 dimension) const
	{ return division[dimension-1];}
}
