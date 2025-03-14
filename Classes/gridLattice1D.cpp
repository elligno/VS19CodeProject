
#include "gridLattice1D.h"

namespace vsc19 
{
	gridLattice1D::gridLattice1D(int32 nx, float64 xmin, float64 xmax)
   : m_gridStrRep{""},
	 m_xmin{xmin},
	 m_xmax{xmax},
	//	  m_noPts{nx},
	 m_division{nx}
	{
		// NOTE shall build string representaion of the grid parameters
	}
	gridLattice1D::gridLattice1D(std::string&& aInitStr) //rvalue reference
	: m_gridStrRep{std::move(aInitStr)},
	  m_xmin{},
	  m_xmax{},
	//  m_noPts{},
	  m_division{}
	{
		// do some reading format
		using namespace std; // allows dropping std:: prefix

		// istrstream is used, although it is deprecated in ISO/IEC
		// 14882:1998(E). however, G++ does not yet support istringstream,
		// so istrstream is used instead.
		//istringstream is(aInitStr.c_str());
		//istringstream is{std::move(aInitStr).c_str()};
		istringstream is{m_gridStrRep.c_str()};
		
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

		float64 r{};
		is >> r;
		m_xmin = r;
		is.ignore(1, ',');

		float64 s;
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
					  << " not allowed for valarray (only 0 works)\n";
			exit(1); // throw an exception!!!
		}

		is.ignore(1, ':');
		int top;
		is >> top;
		m_division = top - base + 1;
		is.ignore(1, ']');
	}

    // ...
    void gridLattice1D::scan( std::string_view init_string)
	{
		using namespace std; // allows dropping std:: prefix

		// istrstream is used, although it is deprecated in ISO/IEC
		// 14882:1998(E). however, G++ does not yet support istringstream,
		// so istrstream is used instead.
		if (nullptr == init_string.data())
		{
			cerr << "gridLattice1D::scan() -- nullptr init_string " << "\n";
		}

		istringstream is(init_string.data());

		// ignore "d="
		is.ignore(1, 'd');
		is.ignore(1, '=');
		is.ignore(1, ' ');
		is.ignore(1, '[');

		float64 r;
		is >> r;
		m_xmin = r;
		is.ignore(1, ',');

		float64 s;
		is >> s;
		m_xmax = s;
		is.ignore(1, ']');
		is.ignore(1, ' ');
		is.ignore(1, '[');

		int base;
		is >> base;
		if (base != 1)
		{
			std::cout << "base=" << base
					  << " not allowed yet (only 1 works)\n";
		}
		is.ignore(1, ':');
		int top;
		is >> top;
		m_division = top - base + 1;
		is.ignore(1, ']');
	}

	std::ostream &operator<<( std::ostream &os, gridLattice1D &aGrid)
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
} // End of namespace