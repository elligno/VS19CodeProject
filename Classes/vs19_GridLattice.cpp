
// C++ includes
#include <cstdio>
#include <cstdlib>
//#include <strstream>
#include <sstream>

// Application include
#include "vs19_GridLattice.h"

using namespace std;

namespace vsc19 {

	// static const int GridLattice::MAX_DIMENSIONS = 2;

	// _______________________________________________________________
	//
	GridLattice::GridLattice()
    : dimensions{2}
	{
	//	dimensions = 2;
		for( int32 i = 1; i <= MAX_DIMENSIONS; ++i) {
			min[i-1] = 0;  max[i-1] = 1;  division[i-1] = 2;
		}
	}

	// _______________________________________________________________
	//
	//   Added by Jean Belanger (January 26, 2007)
	//
	GridLattice::GridLattice( int32 nx, float64 xmin, float64 xmax)
	{
		dimensions = 1;
		max[0]      = xmax;  //   max[1]      = ymax;
		min[0]      = xmin;  //   min[1]      = ymin;
		division[0] = nx;    //   division[1] = ny;

		// create the grid node with the attributes (coordinates) 
	}

	// _______________________________________________________________
	//
	GridLattice::GridLattice( int32 nx, int32 ny, float64 xmin, 
		                        float64 xmax, float64 ymin, float64 ymax)
	{
		dimensions = 2;
		max[0]      = xmax;     max[1]      = ymax;
		min[0]      = xmin;     min[1]      = ymin;
		division[0] = nx;       division[1] = ny;
	}

	// _______________________________________________________________
	//
	float64 GridLattice::xMin( int32 dimension) const
	{ return min[dimension - 1];}

	// _______________________________________________________________
	//
	float64 GridLattice::xMax( int32 dimension) const
	{ return max[dimension - 1];}

	// _______________________________________________________________
	//
	float64 GridLattice::xMin() const
	{ return min[dimensions - 1];}

	// _______________________________________________________________
	//
	float64 GridLattice::xMax() const
	{ return max[dimensions - 1];}

	// _______________________________________________________________
	//
	float64 GridLattice::yMin() const
	{ return min[dimensions - 1];}

	// _______________________________________________________________
	//
	float64 GridLattice::yMax() const
	{ return max[dimensions - 1];}

	// _______________________________________________________________
	//
	int32 GridLattice::getNoPoints() const 
	{
		int32 return_value = 1;

		for( int32 i = 0; i != dimensions; ++i)
			return_value *= division[i];

		return return_value;
	}

	// _______________________________________________________________
	//
	// Read grid with syntax
	//   d=2 [0,1]x[0,10] [1:30]x[1:300]

	void GridLattice::scan( const string & init_string)
	{
		// allows dropping std:: prefix
		using namespace std;  

		// istrstream is used, although it is deprecated in ISO/IEC
		// 14882:1998(E). however, G++ does not yet support istringstream,
		// so istrstream is used instead.
		//istrstream is( init_string.c_str()); deprecated
		istringstream is( init_string.c_str());
		// ignore "d="
		is.ignore(1, 'd');  
		is.ignore(1, '=');

		// get the dimensions
		is >> dimensions;
		if( dimensions < 1 || dimensions > MAX_DIMENSIONS) {
			cerr << "GridLattice::scan() -- illegal dimensions " << dimensions 
				   << endl;
			cerr << "                       MAX_DIMENSIONS is set to " 
				   << MAX_DIMENSIONS << endl;
			exit(1);
		}

		// for each dimension, read the min and max values:
		for( int32 i = 1; i <= dimensions; i++) {
			is.ignore(1, ' '); 
			is.ignore(1, '[');

			float64 r;  is >> r;  min[i-1] = r;
			is.ignore (1, ',');

			float64 s;  is >> s;  max[i-1] = s;
			is.ignore(1, ']');
		}

		// for each dimension, read the base and top index values.
		for( int32 j = 1; j <= dimensions; j++) {
			is.ignore(1, ' '); is.ignore(1, '[');

			int32 base; is >> base;
			if( base != 1) {
				std::cout << "base=" << base 
					<< " not allowed yet (only 1 works)\n";

				exit(1);
			}

			is.ignore(1, ':');
			int32 top;  is >> top;
		//	division[i-1] = top - base + 1; original
			division[j-1] = top - base + 1;
			is.ignore(1,']');
		}
	}//End of namespace

	// _______________________________________________________________
	//
	std::ostream& operator<< ( std::ostream& os, GridLattice& aGrid)
	{
		// write out the grid in the same syntax as scan reads it:
		os << "d=" << aGrid.dimensions << "   ";
		//int32 i;

		for( auto i = 1; i <= aGrid.dimensions; ++i) {
			os << '[' << aGrid.xMin(i) << ',' << aGrid.xMax(i) << ']';
			if (i < aGrid.dimensions)  os << 'x';
		}

		os << "   ";
		for (auto i = 1; i <= aGrid.dimensions; ++i) {
			os << "[1:" << aGrid.getDivisions(i) << ']';
			if (i < aGrid.dimensions)  os << 'x';
		}

		os << endl;

		return os;
	}
}  // End of namespace
