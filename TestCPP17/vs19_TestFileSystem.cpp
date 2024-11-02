// C++ includes
#include <iostream>
#include <filesystem>
// STL include
#include <vector>
// Boost includes
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

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
    void testStdFileSystem() 
    {
        namespace fs = std::filesystem; // some clean stuff

        // what is the current path, location where exec is?
        fs::path w_currPath = fs::current_path();

        using namespace std::literals;
        fs::path w_currrentWrkDirectory{R"(E:\JeanProjects\TestVS2019)"s};
        // create a folder (what is the current path?)
        fs::path w_testDircreate{"data"};
        if (!fs::exists(w_testDircreate))
        {
            // if doesn't exist, otherwise does nothing
            // maybe check if dir exist?
            if (fs::create_directory(w_testDircreate)) 
            {
                std::cout << "Created a directory successfully\n";
            }
        }
 
        // file name for testing
        auto w_fileName = "EMcNeil1D_mod"s;

        // check directory content
        if (fs::exists(w_currrentWrkDirectory) &&
            fs::is_directory(w_currrentWrkDirectory))
        {
            // contains file?
            const fs::directory_iterator end{};
            const fs::directory_iterator dir_iter{w_currrentWrkDirectory};
            auto folder_dist = std::distance(dir_iter, end);
            if (folder_dist == 0) // empty, no files
            {
                // corresponding file name to simulation result
                // w_fileName = "EMcNeil1D_mod"; //m_activeAlgo;
                w_fileName.append("_algo");
                w_fileName.append("_");
                w_fileName.append("1"); // default
                w_fileName += ".txt";   // file extension
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
                    w_fileName.append("_algo");
                    w_fileName.append("_");
                    w_fileName.append("1"); // default
                    w_fileName += ".txt";   // file extension
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
                    w_fileName = std::move(w_newName); // swap resources
                }
            }
        } // if bfs exist

        w_currrentWrkDirectory /= w_fileName;      // full path
        // NOTE: using auto it deduce type as filesystem and not as path
        fs::path ret = w_currrentWrkDirectory.lexically_normal(); // ...
        //     if( bfs::exists(m_currrentWrkDirectory))
        //     {
        //       // increment version number
        //     }

        // sanity check
        if (!w_currrentWrkDirectory.has_extension())
        {
            w_currrentWrkDirectory.replace_extension(".txt");
        }
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