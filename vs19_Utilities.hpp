
#pragma once

#include <filesystem>

namespace vs19 
{
  /**
   * @brief Recursively find the location of a file on a given directory
   * @param directory Path of starting directory
   * @param path Path of founded file name 
   * @param fileName Name of the file to look for
   * @return true/false according ... 
  */
  bool FindFile( const std::filesystem::path& directory,
                 std::filesystem::path& path,
                 const std::string& fileName)
  {
    bool found = false;

    const std::filesystem::path file = fileName;
    const std::filesystem::recursive_directory_iterator end;
    const std::filesystem::recursive_directory_iterator dir_iter(directory);

    // Using anonymous function for searching algo
    const auto iter = std::find_if(dir_iter, end,  // range 
      [&file]( const std::filesystem::directory_entry& e)
      {
        return e.path().filename() == file;
      });

    if( iter != end)
    {
      path = iter->path();
      found = true;
    }
    return found;
  }
} // End of namespace