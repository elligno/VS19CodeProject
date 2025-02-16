#include <cassert>
// C++ includes
#include <type_traits> //is_same_v
#include <utility>
#include <tuple>
// C++20 include
#include <format>
// App includes
#include "vsc19_DbgLogger.h"
#include "../vs19_AppConstant.hpp"

namespace vsc19 
{
	// default ctor
	DbgLogger::DbgLogger()
	: m_opened{false},
	  m_dbgFile{},
	  m_fileName{}
	{}

	// closing file
	void DbgLogger::close()
	{
      if (m_dbgFile.is_open())
	  {
		m_dbgFile.close();
		m_opened=false;
	  }
	}

	void DbgLogger::open( const std::filesystem::path& aFilename)
	{
		namespace fs = std::filesystem;
		
		if( m_dbgFile.is_open()) // already opened
		{
			return;
		}

		m_fileName = aFilename;
         
        //auto fileStatus = std::filesystem::status(aFilename).type();
		switch( auto fileStatus = fs::status(aFilename).type(); fileStatus)
		{
		case fs::file_type::regular:
			m_dbgFile.open(aFilename);
			m_fileName = aFilename;
			m_opened = true;
			std::cout << "Debug logger file was opened\n";
			break;
		case fs::file_type::not_found:
			m_opened = false;
			std::cerr << "Debug logger file couldn't be found\n";
			break;
		case fs::file_type::unknown:
			m_opened = false;
			std::cerr << "Debug logger file format not supported (unknown)\n";
			break;
		default:
			m_opened = false;
			std::cerr << "Debug logger file couldn't be opened\n";
			break;
		}
	}

	void DbgLogger::write2file( std::string_view aMsg, const int aVal)
	{
		m_dbgFile << aMsg.data() << aVal << '\n';
	}

    // function is used by StateVectorField as the following:
	//write2file(aField1.asStdVector(),aField2.asStdVector()) prvalue (temporary)
	void DbgLogger::write2file_p( pairstdvec&& aPairVec)
	{
	    // anonymous entity is rvalue reference to aPairVec 
		// vec1 and vec2 alias names of this entity (structured binding C++17)
        auto&& [vec1,vec2] = std::move(aPairVec); 
        // sanity check
		static_assert( std::is_same_v<decltype(vec1), std::vector<double>> == true );
		static_assert( std::is_same_v<decltype(vec2), std::vector<double>> == true );
        //ditto
		assert( DIM::value == vec1.size());
		assert( vec1.size() == vec2.size());
		// these will be pass as args
		std::string u1p{"U1p"}; // just a test
		std::string u2p{"U2p"}; // ditto

        //compile-time format string
        constexpr const char* floatfmt = "{:3.4f}";

		// using C++20 format feature (not completed!!1)
        for( size_t j = 0; j < vec1.size(); j++) 
		{
			//m_dbgFile << std::format("U1p[%d]: %lf   U2p[%d]: %lf\n", vec1[j],vec2[j]) << '\n'; 
			//m_dbgFile << std::format("'{0}'[%d]: %lf   '{1}'[%d]: %lf\n", w_u1p, w_u2p, vec1[j],vec2[j]) << '\n'; 
			m_dbgFile << std::format("'{0}'[%d]:floatfmt   '{1}'[%d]:floatfmt\n", u1p, u2p, vec1[j],vec2[j]) << '\n'; 
		}
	}

    // NOTE I do not think we will need this anymore (DEPRECATED)
	void DbgLogger::write2file( pairstdvec&& aPairVec)
	{
		// for( unsigned j = 0; j < std::get<0>(aTuple); j++) 
		// 	fprintf( FichierDEBUG, "U1[%d]: %lf   U2[%d]: %lf\n", j, std::get<1>(aTuple)[j], 
		// 	j, std::get<2>(aTuple)[j]);

		// fprintf( FichierDEBUG, "\n");
	}
} // End of namespace