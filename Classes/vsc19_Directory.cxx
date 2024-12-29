//Window include
//#include <Windows.h>
// C++ include 
#include <iostream>
// App includes
#include "vsc19_Directory.h"
//#include "Sfx_Utility.h"
#include "vsc19_EnvVarMgr.h"

#pragma warning(disable:4800)

namespace { // no internal linkage
  bool FindFile( const std::filesystem::path& directory, /* root directory to start with */
    std::filesystem::path& path, /* location path of the searched file */
    const std::string& filename) // file name to look for
  {
    bool found = false;

    const std::filesystem::path file = filename;
    const std::filesystem::recursive_directory_iterator end;
    const std::filesystem::recursive_directory_iterator dir_iter(directory);

    // use auto and lambda function &file input argument of the lambda
    const auto it = std::find_if( dir_iter, end,
      [&file]( const std::filesystem::directory_entry& e)
    {
      return e.path().filename() == file;
    });

    if( it != end)
    {
      path = it->path();
      found = true;
    }

    return found;
  }
} //anonymous namespace

namespace Sfx 
{
	// initialize static variables
	//std::string Directory::m_envPath="";
  std::string Directory::m_rootDir    = "";
	std::string Directory::m_currentDir = "";
	std::string Directory::m_workingDir = "";
  std::string Directory::m_dbgWrkDir  = "";
  std::string Directory::m_dbgWrkFile = "";
//  std::string Directory::m_validationDir = "AlgoValidation" + std::string("\\Result") + Directory::getDateFormat();
  std::filesystem::path Directory::m_dbgDirPath {};

	std::string Directory::GetEnvVariablePath( const std::string& aEnvVsr /*= "TESTVS15_ROOT"*/)
	{
#if 0
    // retrieve the environment variable path
    char *pValue;
    size_t len;
    errno_t err1 = _dupenv_s(&pValue, &len, aEnvVsr.c_str());

    if (err1)
      //Sfx::ErrorMsg( "Cannot retrieve the environment variable");
      std::cerr << "Cannot retrieve the environment variable\n";

    printf("ENV_PATH = %s\n", pValue);
    std::string w_envPath; // null string 
    w_envPath.append(pValue);
    free(pValue);

#endif // 0

		// just testing stuff
//		emcil::EnvVariablesManager w_checkOSVar;
	//	w_envPath = w_checkOSVar.GetOSVariable(aEnvVsr.c_str());

	//	std::cout << 

		// returns the environment var. path
		//return w_envPath;
    return std::getenv(aEnvVsr.c_str());
	}
	
	// _____________________________________________________
	//
  bool Directory::exists( const std::filesystem::path& aDestination)
  {
    namespace fs = std::filesystem;
    if( fs::exists(aDestination))  // check if destination exist?
    {                               // date is same or different
      std::cerr << "Destination directory "
        << aDestination.string()
        << " already exists." << '\n';

      return true;
    }
    return false;
  }

	// ______________________________________________________
	//
	// Query the current folder (std file system or std file system)
	std::string Directory::GetCurrentDir()
	{
    namespace fs = std::filesystem;
 		return fs::current_dir();
	}

	// _________________________________________________________
	//
	void Directory::SetCurrentDir( const std::string& aDir)
	{
		// env. path has not been set
//		if( m_envPath.empty())
// 		{
// 			// set to a default 
// 			//SetEnvVariablePath( "ELGO_PROJECTS"); original in VS2005 project
// 			SetEnvVariablePath( "ELGO_ROOT");
// 		}
		m_currentDir.append(aDir);
	}

	// __________________________________________________________
	//
	// may be use of the std string algorithm library to trim 
	// and split the string. Nice application of std file system
  // with directory iterator (need to look at it)  
	unsigned int Directory::GetNumSubDir( const std::string &aPath)
	{
		// not implemented yet
		// use the find method of the string, we are looking for "\"
		// which separate each directory in the path
		// or use the std string library (split method) and use the 
		// range iterator
		return 0;
	}

	std::string Directory::GetDirectoryRoot()
	{
		// find the root directory of the solution, 
		// then it shall return environment variable TESTVS15_ROOT 
		return GetEnvVariablePath(); // not sure, but for now
	}

  void Directory::setDirectoryRoot( std::filesystem::path aRootpath)
  {
    m_rootDir = aRootpath.string();
  }

  // __________________________________________________________
	//
	std::string Directory::GetWorkingDirectory()
	{
		// There is no member variable for this class since 
		// its a static class. It shall just return the info.
		// In this case we need to return the project directory
		// that contains "Results" folder.

		// check if has been already set, if not initialize it
// 		if( m_workingDir.empty())
// 		{
// 			// retrieve the environment variable path (default) 
// 			m_workingDir+= m_currentDir + std::string("Results");
// 				// original in VS2005 project
// 				//std::string("\\DamBreakProblem\\Result_dec09");
// 		}
		// returns the working path directory 
		return m_workingDir;
	}

	// __________________________________________________________
	//
	void Directory::SetWorkingDirectory( const std::string& aWorkDir)
	{
    if (!m_workingDir.empty())
    {
      m_workingDir.clear();
    }
    m_workingDir = aWorkDir;

		// check if has been already set, if not initialize it
#if 0
    if (m_workingDir.empty())
    {
      // retrieve the environment variable path (default) 
      m_workingDir += m_currentDir + aWorkDir;
      // original in VS2005 project
      //std::string("\\DamBreakProblem\\Result_dec09");
    }

#endif // 0
  }

  // not completed yet, in progress
  void Directory::setDbgWorkingDir( std::filesystem::path aDbgDirPath/*="AlgoValidation" + std::string(R"(\\Result)") + getDateFormat()*/)
  {
    namespace bfs = std::filesystem;

#if 0
    // project path (TestDBpp)
    m_dbgDirPath = bfs::canonical(bfs::current_path());
    // adding validation dir and Result dir to the path
    m_dbgDirPath /= m_validationDir; //bfs::path{"AlgoValidation" + std::string(R"(\\Result)") + getDateFormat()}; //aDbgWrkPath; 
    // setting string format 
    m_dbgWrkDir = m_dbgDirPath.string();
#endif // 0
    m_dbgDirPath = aDbgDirPath;

    // project root path
  //  m_dbgWrkDir = bfs::canonical( bfs::current_path()).string();
    std::cout << "Current debugging working path is: " << m_dbgDirPath << "\n";
  }

  bool Directory::createDir()
  {
    namespace bfs = std::filesystem;

//     std::cerr << "Destination directory "
//       << Sfx::Directory::getWorkingDbgDir().string()
//       << " do not exists, create it." << '\n';

    // Create the destination directory (working directory)
    if (!bfs::create_directory(Sfx::Directory::getWorkingDbgDir()))
    {
      std::cerr << "Unable to create destination directory"
        << Sfx::Directory::getWorkingDbgDir().string() << '\n';
      return false;
    }
    return true;
  }

  bool Directory::dbgFileExist( const std::string & aDbgFilename)
  {
    std::filesystem::path w_DirContainingFile; // path that contain file

    // find file by recursive procedure
    if( FindFile( m_dbgDirPath, w_DirContainingFile, aDbgFilename))
    {
      return true;
    }
    return false;
  }

  void Directory::setWrkDbgFile( const std::string& aDbgFilename)
  {
    if( !m_dbgWrkFile.empty())
    {
      m_dbgWrkFile.clear();
    }

    if( m_dbgWrkFile.capacity()==0)
    {
      m_dbgWrkFile.reserve(50);
      m_dbgWrkFile = aDbgFilename;
    }
    else
    {
      m_dbgWrkFile = aDbgFilename;
      m_dbgWrkFile.shrink_to_fit();
    }

#if 0
    // SSO (Short String Optimization)
    auto capact = 50;
    assert( m_dbgWrkFile.capacity() < capact);
    m_dbgWrkFile.reserve(50); // avoid allocating memory ()
    assert( m_dbgWrkFile.capacity() > capact);

    m_dbgWrkFile = aDbgFilename;
#endif
   // return m_dbgWrkDir + std::string("\\") + m_dbgWrkFile;
  }

  // use std chrono
  std::string Directory::currentDateTime()
  {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s( &tstruct, &now);

    // for more information about date/time format
    strftime( buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    // return value optimization
    return std::string(buf);
  }

  // format is: year-month-day
  // NOTE use std::chrono instead, portable
  std::string Directory::getDateFormat()
  {
    // retrieve the current date and time for simulation result 
    const std::string wDate = currentDateTime();
    // position of the search character, want to strip the time for now
    std::size_t w_Found = wDate.find_first_of(".");
    std::string w_DateStr(wDate);
    std::string::iterator w_ConstIter = w_DateStr.begin();
    w_ConstIter += w_Found;
    // erase time part of the string
    w_DateStr.erase(w_ConstIter, w_DateStr.end());

    return w_DateStr;
  }
} // End of namespace