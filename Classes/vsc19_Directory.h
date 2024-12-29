
#pragma once

// C++ include
#include <string>
#include <filesystem> 

namespace Sfx 
{
	/** Directory class exposes static methods for creating, moving, 
	 *  and enumerating through directories and subdirectories.
	 *
     *    File System library for the implementation (eventually std)
	 */
	// Design Note
	//  Some of the stuff in the SimulationUtility.h shall be moved 
	//  in this class. There is so many place where we need to find
	//  a file or a folder, it deserve to be placed 
	//  The functionalities shall be implemented with the boost file 
	//  system library. 
  // Next version (VS19), C++17 has a new functionality std file system
  // which is similar to the boost file system 
  //  (See N. Josuttis book "C++17 The Complete Guide")
  //
	// Static class
	// refactoring based on the std::filesystem
	class Directory
	{
	public:
    using bfspath = std::filesystem::path;

    // to be completed
		struct DirectoryInfo {};
	public:
		// returns the environment variable path
		static std::string GetEnvVariablePath( const std::string &aEnvVsr = "TESTVS15_ROOT");
		
		// Returns the current application's working directory
		static std::filesystem::path GetCurrentDir() { return std::filesystem::current_path(); }

		// Retrieves the parent directory of the specified path,
		// including both absolute and relative paths as a string value
		static std::string GetParent();
		
		// Returns the volume information, root information, or both for the specified path.
		static std::string GetDirectoryRoot();
		
		// not usre too??
		static void setDirectoryRoot(std::filesystem::path aRootpath);
		static std::string GetWorkingDirectory();
		
		// not sure is that make sense???
		static void SetCurrentDir(std::string_view aCurrentDir) { std::filesystem::current_path(aCurrentDir);}
		// Allows you to determine if a specific directory exists.
		// The path parameter is permitted to specify relative or absolute
		// path information. Relative path information is interpreted as
		// relative to the current working directory
		// ...

		// static void SetEnvVariablePath( const std::string& aEnvPath);
		static bool exists(const std::filesystem::path &aDestination);

		// returns number of subdirectories
		static unsigned int GetNumSubDir( const std::string &aPath);

		static void SetWorkingDirectory( const std::string &aWorkDir);

		static std::filesystem::path getWorkingDbgDir() { return m_dbgDirPath; }

		// should i pass an argument?? i do not know
		static void setDbgWorkingDir(boost::filesystem::path aDbgWrkPath /*=m_validationDir*/);
		static bool createDir();

		// check if file exist in dbg working folder
		static bool dbgFileExist( const std::string &aDbgFilename);
		static std::string getWrkDbgFile() { return m_dbgWrkFile /*(m_dbgWrkDir + std::string("\\") + m_dbgWrkFile)*/; }
		static void setWrkDbgFile( const std::string &aDbgFilename);

		// return current date time windows format
		static std::string currentDateTime();

	protected:
        static std::string getDateFormat();
	private:
		//		static std::string m_envPath; 
		// really need those?
		static std::string m_rootDir;
		static std::string m_currentDir;
		static std::string m_workingDir;
		static std::string m_dbgWrkDir;
		static std::string m_dbgWrkFile;
		// static std::string m_validationDir; +std::string("\\Result") + emcil::getDateFormat()
		static std::filesystem::path m_dbgDirPath;
	};
} // End of namespace