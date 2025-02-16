
#include "gridLattice1D.h"

namespace vsc19 
{
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
} // End of namespace