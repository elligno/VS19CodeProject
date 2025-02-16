#include <cassert>
// C++ includes
#include <iostream>
// App includes
#include "vsc19_Directory.h"
#include "vsc19_EnvVarMgr.h"
#include "../vs19_FindFile.h"

#pragma warning(disable:4800)

#if 0
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
#endif //0

namespace vsc19 
{
	// initialize static variables
	//std::string Directory::m_envPath=""; NO string!! easy to use correctly, hard to use incorrectly!!!! 
  std::filesystem::path Directory::m_rootDir    = "TESTVS19_ROOT";
	std::filesystem::path Directory::m_currentDir = "";
	std::filesystem::path Directory::m_workingDir = "";
 // std::filesystem::path Directory::m_dbgWrkDir  = "";
  std::filesystem::path Directory::m_dbgWrkFile = "";
  // not sure about that crap!!!
//  std::string Directory::m_validationDir = "AlgoValidation" + std::string("\\Result") + Directory::getDateFormat();
  std::filesystem::path Directory::m_dbgDirPath = "";
  std::filesystem::path Directory::m_validationDir = "AlgoValidation";

	std::filesystem::path Directory::GetEnvVariablePath( std::string_view aEnvVsr /*= "TESTVS15_ROOT"*/)
	{
    // retrieve the environment variable path
    char *pValue{nullptr};
    size_t sz{};
   
    if( _dupenv_s( &pValue, &sz, aEnvVsr.data()) == 0 && pValue != nullptr)
    {
      printf("ENV_PATH = %s\n", pValue);
    }

    std::string w_envPath{""}; // null string 
    w_envPath.append(pValue); // not sure what it does?
    free(pValue);

    //return std::getenv(aEnvVsr.c_str());
    return std::filesystem::path{w_envPath};
	}
	
	// _____________________________________________________
	//
  bool Directory::exists( const std::filesystem::path& aDestination)
  {
    namespace fs = std::filesystem;
    if( fs::exists(aDestination))  // check if destination exist?
    {                               // date is same or different
      std::cout << "Destination directory "
        << aDestination.string()
        << " already exists." << '\n';

      return true;
    }
    return false;
  }

	// ______________________________________________________
	//
	// Query the current folder (std file system or std file system)
	std::filesystem::path Directory::GetCurrentDir()
	{
 		//return std::filesystem::current_dir();
    if (!m_currentDir.empty())
    {
      return m_currentDir;
    }
    else { return std::filesystem::path{" "}; }
	}

	// _________________________________________________________
	//
	void Directory::SetCurrentDir( const std::filesystem::path& aDir)
	{
    std::filesystem::current_path(aDir);
		m_currentDir = std::filesystem::current_path(); 
	}

	// __________________________________________________________
  //
	// Nice application of std file system with directory iterator (need to look at it)  
	unsigned int Directory::GetNumSubDir( const std::filesystem::path& aPath)
	{
    // Directory iterator and check file type
    namespace fs = std::filesystem;
    unsigned countDir{};
    for (const fs::directory_entry &e :
         fs::recursive_directory_iterator(aPath))
    {
      if (fs::is_directory(e))
        countDir += 1;
      else
        continue;
    } // for-loop

    return countDir; // no sub directory
  }

  std::filesystem::path Directory::GetDirectoryRoot()
	{
		// find the root directory of the solution, 
		// then it shall return environment variable TESTVS15_ROOT 
    vsc19::EnvVariablesManager w_envVarMgr;
    if (w_envVarMgr.IsOSVariableSet(m_rootDir.string().c_str()))
    {
      return m_rootDir;
    }
    else {
		  return std::filesystem::path{""};
    }
	}

  void Directory::setDirectoryRoot( const std::filesystem::path& aRootpath)
  {
    //m_rootDir = aRootpath.string();
    //m_rootDir = std::filesystem::current_path(aRootpath);

    vsc19::EnvVariablesManager w_envVarMgr;
    if( w_envVarMgr.IsOSVariableSet(m_rootDir.string().c_str()))
    {
      m_rootDir = w_envVarMgr.GetOSVariable("TESTVS19_ROOT");
    }

		m_rootDir = aRootpath;
  }

  // __________________________________________________________
	//
	std::filesystem::path Directory::GetWorkingDirectory()
	{
		// returns the working path directory 
		return m_workingDir;
	}

	// __________________________________________________________
	//
	void Directory::SetWorkingDirectory( const std::filesystem::path& aWorkDir)
	{
    // assign new path 
    m_workingDir = aWorkDir;
  }

  // file folder format is: Resultyyyymmdd
  void Directory::setDbgWorkingDir( const std::filesystem::path& aDbgDirPath/*std::string(R"(\\Result)") + getDateFormat()*/)
  {
    namespace fs = std::filesystem;

    m_dbgDirPath = m_workingDir/m_validationDir; //bfs::path{"AlgoValidation" + std::string(R"(\\Result)") + getDateFormat()}; //aDbgWrkPath; 
    assert(fs::exists(m_dbgDirPath));
    m_dbgDirPath += aDbgDirPath; //std::string("\\Result") + w_currDate

    if( !fs::exists(m_dbgDirPath))
    {
      // set error code on error
      std::error_code ec{};
      try
      {
        // if doesn't exist, otherwise does nothing
        // maybe check if dir exist?
        if (!create_directories(m_dbgDirPath, ec))
        {
          std::cout << "Can't create directories, already exist" << m_dbgDirPath.string() << "\n";
          std::cout << "Error: " << ec.message() << "\n";
        }
      }
      catch (const std::filesystem::filesystem_error &fse) // derived from std exception
      {
        std::cerr << fse.what() << '\n';
        std::cout << "     path: " << fse.path1() << "\n";
      }
    }

    //std::string w_validationDir = "AlgoValidation";
		//std::string w_dir2look( std::string("\\") + w_validationDir);
		//w_curDirString += w_dir2look + std::string("\\Result") + w_currDate;

    // debug folder path
    std::cout << "Current debugging working path is: " << m_dbgDirPath << "\n";
  }

  // ???
  bool Directory::createDir( const std::filesystem::path& aDir)
  {
    namespace fs = std::filesystem;

//     std::cerr << "Destination directory "
//       << Sfx::Directory::getWorkingDbgDir().string()
//       << " do not exists, create it." << '\n';

    // Create the destination directory (working directory)
    if( !fs::create_directory(Directory::getWorkingDbgDir()))
    {
      std::cerr << "Unable to create destination directory"
        << Directory::getWorkingDbgDir().string() << '\n';
      return false;
    }
    return true;
  }

  bool Directory::dbgFileExist( const std::filesystem::path& aDbgFilename)
  {
    if( pathsAreEqual( m_dbgDirPath, aDbgFilename))
    {
      return std::filesystem::exists(aDbgFilename); // path that contain file
    }
    return false;
  }

  std::filesystem::path Directory::findFile( const std::filesystem::path& aRootDir, std::string_view aFile2Find)
  {
    std::filesystem::path w_DirContainingFile{};

      // find file by recursive procedure
    if( vs19::FindFile( aFile2Find, w_DirContainingFile, aFile2Find))
    {
      return w_DirContainingFile;
    }
      return std::filesystem::path();
  }

  void Directory::setWrkDbgFile( std::string_view aDbgFilename)
  {
    m_dbgWrkFile = m_dbgDirPath/aDbgFilename;
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

  void Directory::SetCurrentDir( std::string_view aCurrentDir) 
	{
	  m_currentDir = aCurrentDir; // set a new path	 
		std::filesystem::current_path(m_currentDir);
	}

  // std::filesystem::path Directory::getWorkingDbgDir()
  // { return m_dbgWrkDir; }

  // bool Directory::dbgFileExist( const std::filesystem::path& aDbgFilename)
  // {
  //   return false;
  // }

	bool Directory::pathsAreEqual( const std::filesystem::path &aPth1,
								  const std::filesystem::path &aPth2)
	{
			namespace fs = std::filesystem;
			return fs::exists(aPth1) && fs::exists(aPth2) ? fs::equivalent(aPth1, aPth2)
														  : aPth1.lexically_normal() == aPth2.lexically_normal();
	}
} // End of namespace