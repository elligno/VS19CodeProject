// C++ includes
#include <fstream>
#include <iostream>
#include <filesystem>
// STL include
#include <vector>
// Boost includes
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
// App include
#include "../vs19_Utilities.hpp"  // utility such as find file

namespace vs19 
{
    // Tests to do and experiment library features
    // open a file (new file in a folder)
    // loop path element
    // check extension
    // replace extension
    // create directories/directory
    // remove file
    // rename file
    // check file status
    // if (/* condition */)
    // {
    //     /* code */
    // }
    //
    // Testing the new c++17 file system
    // NOTE maybe take a copy of SfxDirectory of the VS15 app
    // and rewrite it with file system library. This kind of 
    // class should be part of the DamBreak++ Simulator 
    // Also Environment Variables manager, its all together
    //

     // comparing path with equivalent (require that both path exist!!!)
     // NOTE all comparison operators are supported for path '<=>" totally ordered"  
     bool pathsAreEqual( const std::filesystem::path& aPth1,
     const std::filesystem::path& aPth2)
     {
        namespace fs = std::filesystem;
        return fs::exists(aPth1) && fs::exists(aPth2) ? fs::equivalent(aPth1,aPth2) 
        : aPth1.lexically_normal() == aPth2.lexically_normal();
     }

    // printing path element (this veruy helpfull in many situations)
    void printPath(const std::filesystem::path& aPath)
    {
        std::cout << "path element of \"" << aPath.string() << "\":\n";
        for (std::filesystem::path elem : aPath)
        {
            std::cout << "   \"" << elem.string() << '"'; 
        }
        std::cout << '\n';
    }

    // testing new features of C++17 file system 
    void testStdFileSystem() 
    {
        namespace fs = std::filesystem; // some clean stuff
        using namespace std::literals;

        // what is the current path, location where exec is?
        fs::path w_currPath = fs::current_path();
        // sanity check
        if( status(w_currPath).type() == fs::file_type::directory)
        {
            std::cout << '"' << w_currPath.string() << "\" is a directory containing: \n";
            for( const auto& e : std::filesystem::directory_iterator(w_currPath))
            {
                std::cout << "  " << e.path().string() << '\n';
            } 
        }
        
        // ...
        //auto checkOSenv = std::getenv();

        // is that good practice to use string_view?
        fs::path w_currrentWrkDirectory{R"(E:\JeanProjects\TestVS2019)"sv};
        auto w_currDir = fs::current_path();
        std::cout << "Current path at start up is: " << w_currDir.string() << "\n";

        // status operations (no system call which is more efficient)
        auto pathStatus{fs::status(w_currDir)}; // didn't know i could do that!
        if (fs::is_directory(pathStatus))
        {
            std::cout << "this path is a directory\n";
        }
        else if (fs::is_regular_file(pathStatus))
        {
            std::cout << "this path is a regular fiel\n";
        }
        else if(fs::is_other(pathStatus))  // not a file known by the system
        {
            std::cout << "twe have some weird file here\n";
        }
        else { /*do something but i do know what!!!*/ }
        
        // set working directory or currenth path
        fs::current_path(w_currrentWrkDirectory);
        auto checkcurrPath = fs::current_path(); 
        // this is nice to know, many times we need such comparison
        if( fs::equivalent(w_currPath, checkcurrPath)) // point to same file??
        {
            std::cout << "Both path effer tpo the same file\n";
        }

        // create a folder (what is the current path?)
        fs::path w_testDircreate{"Data/Nov24"};
        // check for dir
        if( !fs::exists(w_testDircreate))
        {
            // set error code on error
            std::error_code ec{}; 
            try
            {
                // if doesn't exist, otherwise does nothing
                // maybe check if dir exist?
                if (!create_directories(w_testDircreate, ec))
                {
                    std::cout << "Can't create directories, already exist" << w_testDircreate.string() << "\n";
                    std::cout << "Error: " << ec.message() << "\n";
                }
            }
            catch( const std::filesystem::filesystem_error& fse) // derived from std exception
            {
                std::cerr << fse.what() << '\n';
                std::cout << "     path: " << fse.path1() << "\n";  
            }
        }
 
        // file name for testing (directory already created)
        auto w_fileNamePath = w_testDircreate/"EMcNeil1D_mod"s; // no extension

        // check directory content
        if( fs::exists(w_currrentWrkDirectory) &&
            fs::is_directory(w_currrentWrkDirectory))
        {
            // contains file?
            const fs::directory_iterator end{};
            const fs::directory_iterator dir_iter{w_currrentWrkDirectory};
            auto folder_dist = std::distance(dir_iter, end);
            if (folder_dist == 0) // empty, no files
            {
                // corresponding file name to simulation result
                // w_fileNamePath = "EMcNeil1D_mod"; //m_activeAlgo;
                w_fileNamePath.append("_algo");
                w_fileNamePath.append("_");
                w_fileNamePath.append("1"); // default
                w_fileNamePath += ".txt";   // file extension
            }
            else // not empty, contains something
            {
                auto i = 0; // for what?? files counter
                for ( const fs::directory_entry &e :
                     fs::directory_iterator(w_currrentWrkDirectory))
                {
                    if (fs::is_directory(e))
                        ++i;
                }
                if (i == folder_dist)
                { // all directory (contains only directory)
                  // do something here
                    w_fileNamePath.append("_algo");
                    w_fileNamePath.append("_");
                    w_fileNamePath.append("1"); // default
                    w_fileNamePath += ".txt";   // file extension
                }
                else
                { // mix of files and directories
                    //      std::vector<int> w_numberAlgo; w_numberAlgo.reserve(10);
                    fs::path filePath{};
                    for (const fs::directory_entry &e :
                         fs::directory_iterator(w_currrentWrkDirectory))
                    {
                        if (fs::is_directory(e))
                            continue;
                        filePath = e.path();
                        if (filePath.has_filename())
                        {
                            std::cout << "File name is: " << filePath.filename() << "\n";
                            continue;
                        } // if
                    } // for-loop

                    std::vector<std::string> w_nameExt;
                    w_nameExt.reserve(2); // SSO
                    boost::split(w_nameExt, filePath.filename().string(),
                                 boost::is_any_of("."));          // without extension
                    const auto w_lastDigit = w_nameExt[0].back(); // last digit of file name
                    auto w_toInt = std::atoi(&w_lastDigit);       // char to integer
                    w_toInt += 1;                                 // w_numberAlgo.back(); w_toInt+=1;
                    auto &w_updatdInt = w_nameExt[0].back();
                    // w_updatdInt = *const_cast<char *>(std::to_string(w_toInt).c_str());
                    w_updatdInt = *(std::to_string(w_toInt).data());
                    auto w_newName = w_nameExt[0]; // update version number
                    w_newName += std::string{"."} + w_nameExt[1];
                    // avoid copying long string without SSO (Small String Optimization)
                    w_fileNamePath = std::move(w_newName); // swap resources
                }
            }
        } // if bfs exist

        w_currrentWrkDirectory /= w_fileNamePath;      // full path
        if (!w_currrentWrkDirectory.empty())
        {
            auto fileNameNoExt = w_currrentWrkDirectory.stem(); // return file name without extension
            assert(fileNameNoExt.has_extension()==false);
        }

        // NOTE: using auto it deduce type as filesystem and not as path
        fs::path ret = w_currrentWrkDirectory.lexically_normal(); // ...
        //     if( bfs::exists(m_currrentWrkDirectory))
        //     {
        //       // increment version number
        //     }

        // sanity check
        if( !w_currrentWrkDirectory.has_extension())
        {
            w_currrentWrkDirectory.replace_extension(".txt");
        
        }
        // something that we may wan in many occasion (sanity check)
        auto checkType = fs::status(w_fileNamePath).type();
        assert(fs::file_type::regular == checkType);
        std::ofstream dataFile{w_fileNamePath};

        // Path modification (see C++17 N Josuttis page 226)
        // it is something that we have to do in many situations
        // for example we work in a given folder, then want to change
        // to another, change file name, remove file name of, using string_view etc...
    }

    void DamBreakfstest() 
    {
            // set a namespace for clarity
    namespace fs = std::filesystem;

    // format is  based on date "Resultyyyy-mm-dd" in folder "AlgoValidation"
    auto w_curDirString = fs::canonical( fs::current_path()).string();
    std::cout << "Current path for this test is: " << w_curDirString.c_str() << "\n";

    //TODO: Design Note: taken into account that "AlgoValidation" directory already exist
    // In the case it doesn't we have an error. 
    // A test must be done before going any further if directory exist, otherwise create it 
    // NOTE below we hard code the directory for algorithm validation
 //   std::string w_validationDir = "AlgoValidation";
//     std::string w_dir2look( std::string("\\") + m_validationDir);
//     w_curDirString += w_dir2look + std::string("\\Result") + emcil::getDateFormat();

    // this is a relative path
   // bfs::path w_dbgWrkDir{ m_validationDir + std::string("\\Result")};

 //     emcil::Logger::instance()->OutputSuccess(std::format("Creating folder and file to save simulation").data());

      // for some reason this don't work!! need unit test!!! check VS15 app unit test project
   //   auto checkPath = std::string{ Sfx::EnvVariablesManager::GetOSVariable(std::string{ "ALGOVALIDATE_ROOT" }.c_str()) };

      // set default working directory path
      fs::path w_algoPath{ w_curDirString + std::string{"\\AlgoValidation"}};
   //   fs::path w_algoValidation { w_algoPath.string() + std::string("\\Result") + emcil::getDateFormat() };
      
 //     Sfx::Directory::setDbgWorkingDir(w_algoValidation);

      // check if destination exist? (date is same or different)
    //   if( !fs::exists( Sfx::Directory::getWorkingDbgDir()))  
    //   {
    //     // add code here                                     
    //   }
    }
} // End of namespace

#if 0
        //+++++++++++++++++++++++++++
        // just testing
        auto w_fnameNoExt = filePath.stem();
        auto lastDigit = w_fnameNoExt.string().back();
        auto w_to2Int = std::atoi(&lastDigit);
        w_to2Int += 1;
        auto newOne = *(std::to_string(w_to2Int).data());
        auto foundOne = std::find_if(w_fnameNoExt.string().begin(),
                                     w_fnameNoExt.string().end(),
                                     [](char c) { return std::isdigit(c,std::locale()); });

        if (foundOne != w_fnameNoExt.string().end()) {
          // found one
          auto distFromBeg =
              std::distance(foundOne, w_fnameNoExt.string().end());
          if (dist > 1) {
          }
        }
        auto str = w_fnameNoExt.string();
        auto beg = str.begin();
        while (*beg && !isdigit(*beg))
          beg++;
        if (*beg) {
          // beg==end means only one digit
          // otherwise more than one
          // distance from end distance(beg,str.end())
          // distance
          // auto distFrmBeg = std::distance(str.begin(),beg);
          // replace last digit
          std::replace(str.begin(), str.end(), lastDigit, newOne);
        }
        //+++++++++++++++++++++++++++++++++
#endif