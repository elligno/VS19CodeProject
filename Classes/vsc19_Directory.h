
#pragma once

// C++ includes
#include <string>
#include <filesystem>  //C++17 feature

namespace vsc19
{
	// Design Note
	//  Some of the stuff in the SimulationUtility.h shall be moved 
	//  in this class. There is so many place where we need to find
	//  a file or a folder, it deserve to be placed 
	//  The functionalities shall be implemented with the boost file 
	//  system library. 
	// Static class
	// refactoring based on the std::filesystem
	
	/** @brief Directory class exposes static methods for creating, moving, 
	 *  and enumerating through directories and subdirectories.
	 *
     *    File System library for the implementation (eventually std)
	 */
	class Directory
	{
	public:
    using bfspath = std::filesystem::path;

    // to be completed
		struct DirectoryInfo {};
	public:
		// returns the environment variable path
		static std::filesystem::path GetEnvVariablePath( std::string_view aEnvVsr = "TESTVS19_ROOT");
		
		// Returns the current application's working directory
		static std::filesystem::path GetCurrentDir(); 

        // ...
		static void SetCurrentDir( const std::filesystem::path& aDir2Set);

		// Retrieves the parent directory of the specified path,
		// including both absolute and relative paths as a string value
		//static std::string GetParent();
		
		// Returns the volume information, root information, or both for the specified path.
		static std::filesystem::path GetDirectoryRoot();
		
		// not usre too??
		static void setDirectoryRoot( const std::filesystem::path& aRootpath);

		//  .. to be completed
		static std::filesystem::path GetWorkingDirectory();
		
		// not sure is that make sense??? it sure is!!!
		static void SetCurrentDir( std::string_view aCurrentDir);

		// Allows you to determine if a specific directory exists.
		// The path parameter is permitted to specify relative or absolute
		// path information. Relative path information is interpreted as
		// relative to the current working directory
		// ...

		// static void SetEnvVariablePath( const std::string& aEnvPath);
		static bool exists( const std::filesystem::path &aDestination);

		// returns number of subdirectories
		// directory iterator and check type of each element found in directory
		//  is_directory()??  
		static unsigned int GetNumSubDir( const std::filesystem::path& aPath); // not implemented
        
		static void SetWorkingDirectory( const std::filesystem::path& aWorkDir);

		static std::filesystem::path getWorkingDbgDir() { return m_dbgDirPath; }

		// should i pass an argument?? i do not know
		static void setDbgWorkingDir( const std::filesystem::path& aDbgWrkPath);
        
		//  ??? really?
		static bool createDir( const std::filesystem::path& aDir);

		// check if file exist in dbg working folder
		static bool dbgFileExist( const std::filesystem::path& aDbgFilename);

		static std::filesystem::path findFile( const std::filesystem::path& aRootDir,
		 std::string_view aFile2Find);

		// ...
		static std::filesystem::path getWrkDbgFile() { return m_dbgWrkFile;}

		// .. to be completed
		static void setWrkDbgFile( std::string_view aDbgFilename);

        // retrn number of elements in the directory
        static unsigned getDbgDirNumElements() 
		{
			// create dir iter (end and begin and return end-beg)
			std::filesystem::directory_iterator begDir{m_dbgDirPath};
			std::filesystem::directory_iterator endDir{};

			return std::distance(begDir,endDir);
	    } 
        
		//return true/false found file (not implemented)
        static bool contains( const std::filesystem::path& aDbgFilename) 
		{
			return false;
		} 

	protected: // not sure about these 2, shall be free functions
        static std::string getDateFormat(); // why protected?
		// return current date time windows format
		static std::string currentDateTime();
	private:
		// really need those?
		static std::filesystem::path m_rootDir;
		static std::filesystem::path m_currentDir;
		static std::filesystem::path m_workingDir;
	//	static std::filesystem::path m_dbgWrkDir;
		static std::filesystem::path m_dbgWrkFile; // maybe declare as path?? not sure??
		// static std::string m_validationDir; +std::string("\\Result") + emcil::getDateFormat()
		static std::filesystem::path m_dbgDirPath;
		static std::filesystem::path m_validationDir;

		// comparing path with equivalent (require that both path exist!!!)
		// NOTE all comparison operators are supported for path '<=>" totally ordered"
		static bool pathsAreEqual( const std::filesystem::path &aPth1,
								   const std::filesystem::path &aPth2);
	};
} // End of namespace