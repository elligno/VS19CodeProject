#pragma once

// C++ includes
#include <string>
#include <fstream> // for ofstream
#include <memory>  // for smart pointer
#include <cstdio>  // for remove()

namespace vsc19 
{
  /**
   * @brief Ensure that a temporary file gets removed when 
   *  when last reference to it gets destroye.
   * @Usage 
   *   std::shared_ptr<std::ofstream> w_fp { new std::ofstream{"tmpFile.txt"},
          FileDeleter{"tmpFile.txt"}};
  */
  class FileDeleter 
  {
  private:
    std::string m_filename; /**< file name */

  public:
    /**
     * @brief Ctor 
     * @param aFilename name of the file owned 
    */
    FileDeleter( const std::string& aFilename) : m_filename{aFilename} {}
    /**
     * @brief 
     * @param aFilePtr 
    */
    void operator() ( std::ofstream* aFilePtr)
    {
      aFilePtr->close();               // close file
      std::remove(m_filename.c_str()); // delete file
    }
  };
} // End of namespace
