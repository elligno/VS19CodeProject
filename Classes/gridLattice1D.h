
#pragma once

// C++ includes
#include <iostream>
#include <sstream>

// boost include 
#include <boost/cast.hpp> // numeric cast

namespace vsc19 
{
	/**
	 * @brief class GridLattice1D describes the domain the PDE will be applied to.
	 */
	class gridLattice1D
	{
	public:
		/**
		 * @brief Construct a new grid Lattice object
		 *
		 */
		// gridLattice1D();
		//  gridLattice1D( const gridLattice1D& aOther) = default;
		//  gridLattice1D& operator= ( const gridLattice1D& aOther) = default;
		//  gridLattice1D( gridLattice1D&& aOther) = default;
		//  gridLattice1D& operator= ( gridLattice1D&& aOther) = default;

		/**
		 * @brief Construct a new grid Lattice object
		 *
		 * initialize the grid with the following syntax:
		 * d=2 [0,1]x[0,10] [1:30]x[1:300] -> [xmin,xmax] [0:99] for std::valarray
		 * element index goes from 0, N-1 
		 * @param aInitStr
		 * Usage std::string{d=1 [0,1] [0:N-1]} in one-dimension for valarray
		 */
		gridLattice1D(std::string&& aInitStr) //rvalue reference
		: m_xmin{},
		  m_xmax{},
		//  m_noPts{},
		  m_division{}
		{
			//just testing!! really not sure about this one!!
			//should i call std::move(aInitStr)
		   // scan(aInitStr); don't work

			// do some reading format
			using namespace std; // allows dropping std:: prefix

			// istrstream is used, although it is deprecated in ISO/IEC
			// 14882:1998(E). however, G++ does not yet support istringstream,
			// so istrstream is used instead.
			//istringstream is(aInitStr.c_str());
            istringstream is{std::move(aInitStr).c_str()};
            
			int dimensions{};

			is.ignore(1, 'd');
			is.ignore(1, '=');
			// get the dimensions
			is >> dimensions;
			if (dimensions != 1)
			{
				cerr << "GridLattice::scan() -- illegal dimensions " << dimensions << endl;
				exit(1); // shall throw an exception
			}

			is.ignore(1, ' ');
			is.ignore(1, '[');

			double r{};
			is >> r;
			m_xmin = r;
			is.ignore(1, ',');

			double s;
			is >> s;
			m_xmax = s;
			is.ignore(1, ']');

			is.ignore(1, ' ');
			is.ignore(1, '[');

			int base;
			is >> base;
			if (base != 0) //NOTE index for std valarray
			{
				std::cout << "base=" << base
						  << " not allowed yet (only 1 works)\n";
				exit(1); // throw an exception!!!
			}

			is.ignore(1, ':');
			int top;
			is >> top;
			m_division = top - base + 1;
			is.ignore(1, ']');
		}

		// ctor from parameters (not used at the moment in our programming environment)
		gridLattice1D(int nx, double xmin, double xmax)
		: m_xmin{xmin},
		  m_xmax{xmax},
	//	  m_noPts{nx},
		  m_division{nx}
		{
		}

		int getNoSpaceDim() const { return 1; } // for this implementation

		double xMin() const { return m_xmin; }
		double xMax() const { return m_xmax; }

		// get the total number of points in the grid.
		int getNoPoints() const { return m_division; }
		double Delta() const; //{ return ( m_xmax - m_xmin)/static_cast<double>(m_division);}
		double getPoint( int index) // not sure about this one!
		{
			// if pass index from 1,...,N this is correct
			 return m_xmin + (Delta() * (index - 1)); 
	    } 
        // using with std container such as valarray (index start at 0 and end at size-1)
		int getBase() const { return 0; }			   // get base values
		int getMaxI() const { return m_division - 1; } // upper limit of array

		// must be private, access denied to client
		// scan parameters from string
		// Read grid with syntax
		//   d=2 [0,1]x[0,10] [1:30]x[1:300]
		//
		// NOTE:
		//  std::istringstream is now supported with VS2015
		//
		void scan( std::string_view init_string)
		{
			using namespace std; // allows dropping std:: prefix

			// istrstream is used, although it is deprecated in ISO/IEC
			// 14882:1998(E). however, G++ does not yet support istringstream,
			// so istrstream is used instead.
			if( nullptr == init_string.data())
			{
				cerr << "gridLattice1D::scan() -- nullptr init_string " << "\n";
			}

			istringstream is(init_string.data());

			// ignore "d="
			is.ignore(1, 'd');
			is.ignore(1, '=');

			// get the dimensions
			//	is >> dimensions;
			//	if (dimensions < 1 || dimensions > MAX_DIMENSIONS) {
			// cerr << "GridLattice1D::scan() -- illegal dimensions " << dimensions
			// 		<< endl;
			// 	cerr << "                       MAX_DIMENSIONS is set to "
			// 		<< MAX_DIMENSIONS << endl;

			// 	exit(1);
			//	}

			// for each dimension, read the min and max values:
			// for (int i = 1; i <= dimensions; i++)
			// {
			is.ignore(1, ' ');
			is.ignore(1, '[');

			double r;
			is >> r;
			m_xmin = r;
			is.ignore(1, ',');

			double s;
			is >> s;
			m_xmax = s;
			is.ignore(1, ']');
			//		}

			// the new version parameters
			// m_min.first = min[0];
			// m_max.first = max[0];
			// if( dimensions == 2)
			// {
			// 	m_min.second = min[1];
			// 	m_max.second = max[1];
			// }

			// for each dimension, read the base and top index values.
			// for (int i = 1; i <= dimensions; i++)
			// {
			is.ignore(1, ' ');
			is.ignore(1, '[');

			int base;
			is >> base;
			if (base != 1)
			{
				std::cout << "base=" << base
						  << " not allowed yet (only 1 works)\n";
			//	exit(1);
			}
			is.ignore(1, ':');
			int top;
			is >> top;
			m_division = top - base + 1;
			is.ignore(1, ']');
			//	}

			// set new version parameters
			// m_division.first=division[0];
			// if (dimensions==2)
			// {
			// 	m_division.second=division[1];
			// }
		}

		int getDivisions() const { return m_division; }

		// access to private member
		friend std::ostream &operator<<( std::ostream &os, gridLattice1D &aGrid)
		{
			auto dimensions = 1;
			// write out the grid in the same syntax as scan reads it:
			os << "d=" << dimensions << "   ";
			//		int i;
			//		for( i = 1; i <= g.dimensions; ++i) {
			os << '[' << aGrid.xMin() << ',' << aGrid.xMax() << ']';
			//			if (i < g.dimensions)  os << 'x';
			//		}
			os << "   ";
			//		for( i = 1; i <= g.dimensions; ++i) {
			os << "[1:" << aGrid.getDivisions() << ']';
			//			if (i < g.dimensions)  os << 'x';
			//		}
			os << std::endl;

			return os;
		}

	private:
		double m_xmin;	/**< minimum x coordinate*/
		double m_xmax;	/**< maximum x coordinate*/
//		int m_noPts;	/**< grid number of nodes*/
		int m_division; /**< number of points*/
	};

	inline double gridLattice1D::Delta() const
	{
		using boost::numeric_cast;
		using boost::numeric::bad_numeric_cast;
		using boost::numeric::negative_overflow;
		using boost::numeric::positive_overflow;

		//return (max[dimension-1] - min[dimension-1])/double(division[dimension-1]);

		// return first dimension parameter as default
		try
		{
			// try numerical cast and catch
		//	 if (dimension<1||dimension>MAX_DIMENSIONS)
		//	 {
			   // default value ...
		    return (m_xmax - m_xmin)/numeric_cast<double>(m_division);
		//	}
			// ...
			//		if (dimension==1)
			//		{
	//		return (m_xmax - m_xmin) / numeric_cast<double>(m_division);
			// }
			// else
			// {
	//		 	return (m_max.second - m_min.second)/static_cast<double>(m_division.second);
			// }
		}
		catch( bad_numeric_cast &bc)
		{
		 	std::cerr << bc.what() << "\n";
		 	return 0.;
		}
	//	return 0.;
	}
} // End of namespace