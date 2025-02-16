
#pragma once

// C++ includes
#include <filesystem>
#include <fstream>     // ofstream
// STL include
#include <vector>
// Package include
#include "vsc19_Singleton.hpp"

namespace vsc19 
{
	/** @brief This class is just a wrapper around E. McNeil debugging and result file 
     *   write to file information to be used for debugging and visualizing). 
	 *  Usage:
	 *   as global instance of the program
	*/  
  	class DbgLogger final : public Singleton<DbgLogger>  
	{
		using pairstdvec = std::pair<std::vector<double>, std::vector<double>>;
		// using  tupleidd = std::tuple<int,double,double>;
	public:
		// denied copy and copy assignment
		DbgLogger(const DbgLogger &) = delete;
		DbgLogger &operator=(const DbgLogger &) = delete;

		/**open file for writing (file name as default) */
		void open (const std::filesystem::path &aFilename);
		bool isOpen() const {return m_opened; } // check file status
		void close();					   // close when finish writing
		void write2file( std::string_view aMsg, int aVal); // what this one for?
		void write2file_p( pairstdvec &&aPairVec);

		void write2file( pairstdvec &&aPairVec); //deprecated

        void setDbgFileName(const std::filesystem::path &aFilename) 
		{ m_fileName=aFilename;}

		/** return the filename */ 
		std::string name() const 
		{ 
			if( m_fileName.has_filename())
			{
				return m_fileName.filename().string(); 
			}
			return std::string{""};
		}
        /** default ctor access*/ 
		friend class Singleton<DbgLogger>;
	protected:
		/** Client have no choice to use the static method instance()
		 * which manage the creation of the sole instance.
		*/
		DbgLogger();
	private:
		bool m_opened;					  /**< file status*/
		std::ofstream m_dbgFile;		  /**< file stream*/
		std::filesystem::path m_fileName; /**< file name*/
	};
} // End of namespace