// C++ includes
#include <cassert>
#include <cctype>  // std::isdigit
#include <fstream>
#include <iostream>
#include <filesystem>
// STL includes
#include <map>
#include <vector>
#include <ranges>
// Boost includes
//#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string/split.hpp>
// App include
//#include "../vs19_FindFile.h"  // utility such as find file
#include "../Classes/vsc19_Directory.h"

namespace vs19 
{
    // C++20 Sentinel (end iterator) range library
    //  Since C++20 operator != is automatically generated.
    //  It is used by the find_if algorithm
    //  By using auto keyword in the operator signature
    //  is implicitly templated method 
    template<auto End>
    struct FirstCharDgt {
          bool operator== (auto pos) const { return *pos==End;}
    };

    void testRngAlgo( std::string_view aFileName)
    {
       namespace rng = std::ranges;
       // is that make sence?
       static_assert(rng::range<decltype(aFileName)>); 

       // would like to find first digit in the file name
       // std ranges algorithm use a sentinel
       auto findResult = rng::find_if(aFileName,
       [] (char aChar) 
       {
          return std::isdigit(aChar);
       });
        
       auto lastOff = aFileName.find_last_of('_');
       auto findPos = rng::distance(aFileName.begin(),findResult);
       rng::subrange w_strRng{aFileName.begin(),FirstCharDgt<'1'>{}};
       auto chkLast = aFileName.back(); 
       aFileName.remove_suffix(4); //".txt" we have filename with no extension
      
        for( std::counted_iterator pos{aFileName.begin(),findPos}; 
        pos!=std::default_sentinel;
        ++pos) {
            std::cout << "Character is: " << *pos << '\n';
        }
        
        // range algorithm
        rng::for_each( w_strRng,
        [] (auto aChar) 
        {
            std::cout << "Character is: " << aChar << '\n';
        });
    }

    bool contains( const auto& cont, std::string_view str2cmp)
    {
        // str.find() (or str.contains(), since C++23) can be used as well
        return std::search(cont.begin(), cont.end(), str2cmp.begin(), str2cmp.end()) != cont.end();
    }

    // DamBreak simulator environment (using Directory and Env varaibles)
    void DebugFolderAndFiles()
    {
        namespace fs = std::filesystem;
        using namespace std::literals::string_literals;      // operator 's'
        using namespace std::literals::string_view_literals; // operator 'sv'

        // directory test
        auto w_algoPath = vsc19::Directory::GetEnvVariablePath("ALGOVALIDATE_ROOT"sv);
        std::cout << "Path for this system variable is :" << w_algoPath.lexically_normal().string() << '\n';
        // sanity check (algovalidation)
        auto algoExist = fs::exists(w_algoPath);
        assert(fs::status(w_algoPath).type() == fs::file_type::directory);
        vsc19::Directory::setDirectoryRoot(vsc19::Directory::GetEnvVariablePath("TESTVS19_ROOT"sv));
        vsc19::Directory::SetCurrentDir(fs::current_path()); // not too sure 
        assert(fs::equivalent(fs::current_path(),vsc19::Directory::GetDirectoryRoot()));
        // just a check
        auto w_curdPath = fs::current_path();

        // auto ch123 = fs::is_directory(w_curdPath);
        // auto w_validPath = w_curdPath / "AlgoValidation"sv; // add sub path
        // auto aaa = fs::status(w_validPath).type();
        // auto unkn = (aaa == fs::file_type::not_found);
        // auto dirtr = fs::is_directory(w_validPath);
        // auto chkres = fs::is_regular_file(w_validPath);
        
        // this folder is based on current date
        // if the folder doesn't exist, we create it and open file (dbg file)
        w_algoPath /= "Result"sv;        // ditto
        w_algoPath.concat("20250210"sv); // format is the following: Result20250210
        // check (not created yet)
        if( fs::status(w_algoPath).type() == fs::file_type::not_found)
        {
            // create it!
            try
            {
                fs::create_directories(w_algoPath);
            }
            catch (const fs::filesystem_error &fse)
            {
                std::cerr << fse.what() << '\n';
                std::cerr << fse.path1() << '\n'; // not sure what it is!
            }
            // not sure if it should be created at the startup 
            // Dbg logger will probably take care of this (open/close)
            // maybe a call to DbgLogger.setfilenam, make more sense!!
            std::ofstream w_dbgFile{"AlgoName"};
        }

//        auto isVald = fs::is_directory(w_algodPath);

        // DamBreak simulator environment we retrioeve algo name
        // such as: EMcNeil1D_algo from SimulationManager
        // then add a version number "_1"
        auto w_algoName{"EMcNeil1D_algo"s}; // Simulation::instance().getAlgoName();
        //w_algoName.append("_1");
        fs::path w_dbgFilePath = w_algoPath/"w_algoName"s;
        w_dbgFilePath.concat("_1"s);
        w_dbgFilePath.replace_extension(".txt");
#if 0
        auto w_algoPath = w_validPath;               // assign new path
        w_algoPath.append("EMcNeil1D_algo_1.txt"sv); // added file name to the path
        auto unkn2 = (fs::status(w_algoPath).type() == fs::file_type::not_found);
        std::ofstream w_file2Create{w_algoPath}; // file created
        assert(w_file2Create.is_open());         // sanity check
        w_file2Create.close();
        w_algoPath.replace_filename("EMcNeil1D_algo_2.txt"sv);
        // assert(fs::status(w_algoPath).type() == fs::file_type::not_found);
        std::ofstream w_fileCreated{w_algoPath}; // file created
        assert(w_fileCreated.is_open());         // sanity check
        w_fileCreated.close();
        auto unkn3 = (fs::status(w_algoPath).type() == fs::file_type::regular);
        auto bbb = fs::status(w_algoPath).type(); // should be unknown
        assert(w_algoPath.has_filename());
#endif
        // check folder content
        fs::directory_iterator dir_iter(w_algoPath);
        fs::directory_iterator dir_end{};
        //auto nbFiles = std::distance(dir_iter, dir_end);

        // retrieve file version number and increment it
        if (auto w_foundIt = w_dbgFilePath.filename().string().find_last_of("_");
            w_foundIt != std::string::npos)
        {
            for (const auto &dirEntry : fs::directory_iterator(w_algoPath))
            {
                if (dirEntry.path() == w_algoPath / "EMcNeil1D_algo_1.txt"sv)
                {
                    // found one "_"
                    auto w_Fname = dirEntry.path().filename().string();
                    auto w_fNamebeg = dirEntry.path().filename().string().cbegin();
                    std::advance(w_fNamebeg, w_foundIt + 1);  // version number location
                    const char w_char2look = *w_fNamebeg;     // what we are looking for
                    auto versionNo = std::atoi(&w_char2look); // convert to an integer
                    do                                        // search for last version saved
                    {
                        // replace version number by upgraded one
                        ++versionNo; // next version
                        w_Fname.replace(w_foundIt + 1, 1, std::to_string(versionNo));
                        w_dbgFilePath.replace_filename(w_Fname);
                    } while (fs::status(w_dbgFilePath).type() != fs::file_type::not_found);
                    break;
                }
                // create it!! couldn't find it
                std::ofstream w_file2Create{w_dbgFilePath}; // file created
                assert(w_file2Create.is_open());         // sanity check
                w_file2Create.close();
            }//for-loop
        } // if

        assert(fs::status(w_algoPath).type() == fs::file_type::not_found);

        std::cout << "File name changed: " << w_algoPath.filename().string() << '\n';
    }

    // ************************ IMPORTANT **************************
    //   VS15 project in Dsn namespace SimulationUtilities contains functions to create folder and files
    //   would be nice to refactor those utilities with std::filesystem and present this in my blog.
    //   Need to check, but those functions are part of VS19 project. 
    //   Blog is already started and will be published in mid february.

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

    // see note above about a function "CreateFolederAndFile" from pour programming env.
    std::filesystem::path createDir( std::filesystem::path&& aPth2Mv) 
    {
        namespace fs = std::filesystem;

        // create a folder (what is the current path?)
        fs::path w_testDircreate{std::move(aPth2Mv)}; // where does it create?
        bool checkExist = fs::exists(w_testDircreate);
        // check for dir
        if( !fs::exists(w_testDircreate))
        {
            // set error code on error
            std::error_code ec{}; 
            try
            {
                // if doesn't exist, otherwise does nothing
                // maybe check if dir exist?
                if( !create_directories(w_testDircreate, ec))
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

        return w_testDircreate;
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

    // testing modification of a path
    // remove file name ... 
    // replace file name ...
    // assign new path ...
    // append ...
    // concat ...
    // operator /, /=, +=, ...
    // ... whatever it is!!!
    void modifyPath( const std::filesystem::path& aPath)
    {
        namespace fs = std::filesystem; // more readable
        using namespace std::literals;

        // variable use for the test (not valid) char8_t has changed since C++17
        constexpr auto w_path2Test{R"(D:\RootDir\JeanTest)"sv}; // string_view
        const fs::path w_validPath{R"(E:\JeanProjects\TestVS2019)"sv};
        
        // initialized path
        auto w_pat2Assign = aPath; // path (should a valid path)
        std::cout << "Just initiaized a path: " << w_pat2Assign << "\n"; 

        // assign
        w_pat2Assign = w_validPath; // set a new path
        std::cout << "Just assigned a path: " << w_pat2Assign << "\n"; 

        if( auto pathType = fs::status(w_pat2Assign).type(); pathType == fs::file_type::directory)
        {
            std::cout << "Our new created path is a directory\n";
            // does it exist?? yes it exist, be careful!!! don't overwrite
            if( !fs::exists(w_pat2Assign))
            {
                // since it doesn't exist shall create it!! 
                try
                {
                    fs::create_directory(w_pat2Assign);
                }
                catch( const std::filesystem::filesystem_error& fse)
                {
                    std::cerr << "EXCEPTION: " << fse.what() << '\n';
                    std::cerr << "path1:" << fse.path1().string() << '\n';
                }
            }
        }//if
        
        // append
        fs::path w_pathFronVws = w_path2Test; // from a string view
        auto pathExist = fs::exists(w_pathFronVws); // false
        auto checkValid = fs::is_empty(w_pathFronVws); // what happen if it doesnn't exist?
        std::cout << "Path initialized form strig_view is: " << w_pathFronVws << '\n';
        auto ch123 = w_pathFronVws.append("tmp1"); //shall append?? ch123/tmp1 ?
        auto ch321 = w_pathFronVws.append("/tmp2"); //shall append?? ch321/tmp2 ?

        // another operator /= (depend if it's an absolute path) 
        // difference between        
        auto patOp1 = ch321/"tmp12";   // result is? aa/tmp12?
        auto patOp2 = ch321/"/tmp12";  // aa/tmp12/tmp12
        patOp1 /= patOp2; // add sub path
        // what is the difference between /= and +=?
        patOp2 += ch321; // result??
        patOp2 += "aei"s; // call path ctor which takes a string or string_view  
        // concatenate pathg operation
        fs::path w_concatPath{w_path2Test};
        auto retPath = w_concatPath.concat("contdir"s);

        // replace file name (no file name, add one)
        auto pathCtor{w_validPath};
        if( !pathCtor.has_filename()) // no file name
        {
            // add a file name
            auto newPatWilthFile = pathCtor.replace_filename("JeanFile.txt"); // no file
            auto samePath = (newPatWilthFile == pathCtor); // equality operator
            auto remfile = newPatWilthFile.remove_filename(); // no more file name 
            auto isItTrue = remfile.has_filename(); //false
            auto patWilthNewFile = pathCtor.replace_filename("JeanNewFile.txt"); // already a file
        }

        switch( fs::status(aPath).type())
        {
        case fs::file_type::regular:
            /* code */
            break;
        case fs::file_type::unknown:
            break;
        case fs::file_type::not_found:
            break;
        default:
        std::cerr << "Couldn't process this file\n";
            break;
        }
    }

    // Elligno debug folder environment
    void createResultFolder( const std::filesystem::path& aDirResult)
    {
        using namespace std::literals;

        namespace fs = std::filesystem;

        // check it its exist
        if( !fs::exists(aDirResult))
        {
            // create it if not already there
            if( !fs::create_directory(aDirResult)) 
            {
                std::cerr << "Couldn't create result directory\n";
            }

            // check if empty (file format in this folder is: EMcNeil1D_mod_algo_1.txt
            //  auto w_testFileExist = w_newCurrPath/"EMcNeil1D_mod_algo_1.txt"s;)
            auto chk123 = fs::is_empty(aDirResult);
        } else { 
            // file is already created and may contains result files
            auto fileExist = fs::exists(aDirResult/"EMcNeil1D_mod_algo_1.txt"s);
        }

        // file name to start with for testing (directory already created)
        auto w_fileNamePath = "EMcNeil1D_mod"s; // no extension, completed later

        //auto fileStus = fs::status(w_fileNamePath).type();
        // check if file exist (if not empty) 
         // where i took that piece of code? Qt project I guess!!
        // check directory content
        if( fs::exists(aDirResult) &&
            fs::is_directory(aDirResult))
        {
            // contains file?
            const fs::directory_iterator end{}; // end iterator
            const fs::directory_iterator dir_iter{aDirResult}; // begin iterator
            auto folder_dist = std::distance(dir_iter, end); // 
            if( folder_dist == 0) // empty, no files
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
                // mix of files and directories
                //      std::vector<int> w_numberAlgo; w_numberAlgo.reserve(10);
                fs::path filePath{};
                short versionNo = 0;
                for( const fs::directory_entry &e :
                     fs::directory_iterator(aDirResult))
                {
                    if( fs::is_directory(e))
                        continue;
                    filePath = e.path(); // current file path for this entry
                    auto tst = e.is_regular_file(); //check exist and is regular
                    
                    if( filePath.has_filename())
                    {
                        std::cout << "File name is: " << filePath.filename() << "\n";

                        // using file system library
                        auto fileNameNoExt = e.path().stem();          // file name no extension
                        auto lastDgts = fileNameNoExt.string().back(); // retrieve last digits
                        // is digits? for now assume files in this folder have the format
                        auto isit = std::isdigit(lastDgts);
                        auto toInt = std::atoi(&lastDgts);             // convert to int
                        versionNo = toInt;
                        continue;
                    } // if
                } // for-loop
                // set new version number (versionNo)
            }
        }
    }

    // testing new features of C++17 file system 
    void testStdFileSystem() 
    {
        namespace fs = std::filesystem; // some clean stuff
        using namespace std::literals;

        // what is the current path, location where exec is? NO!!
        // NOTE it return path of the current file (.../TestCPP17)
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

        // is that good practice to use string_view?
        fs::path w_currrentWrkDirectory{R"(E:\JeanProjects\TestVS2019)"sv};
        auto w_currDir = fs::current_path();
        std::cout << "Current path at start up is: " << w_currDir.string() << "\n";

        // sanity check 
        auto numElemFolder = 
        std::distance( fs::directory_iterator{w_currrentWrkDirectory},
        fs::directory_iterator{}); // (shall return 22 as January 31 2025)

        // status operations (no system call which is more efficient)
        auto pathStatus{fs::status(w_currDir)}; // didn't know i could do that!
        if( fs::is_directory(pathStatus))
        {
            std::cout << "this path is a directory\n";
        }
        else if( fs::is_regular_file(pathStatus))
        {
            std::cout << "this path is a regular fiel\n";
        }
        else if( fs::is_other(pathStatus))  // not a file known by the system
        {
            std::cout << "we have some weird file here\n";
        }
        else { /*do something but i do know what!!!*/ }
        
        // set working directory or currenth path
        fs::current_path(w_currrentWrkDirectory);
        auto checkcurrPath = fs::current_path(); 

        // this is nice to know, many times we need such comparison
        if( fs::equivalent(w_currPath, checkcurrPath)) // point to same file?? no
        {
            std::cout << "Default current path and new current path are not the same\n";
        }
        if( !pathsAreEqual(w_currPath, checkcurrPath))
        {
            std::cout << "Default current path and new current path are not equal\n";
            std::cout << "Path directory is: " << w_currPath.lexically_normal().string() << '\n';
            std::cout << "Path directory is: " << w_currPath.lexically_normal() << '\n';
        }
        if( !pathsAreEqual(w_currrentWrkDirectory, checkcurrPath)) // these are the same
        {
            std::cout << "Default current path and new current path are not equal\n";
        }
        
        // what it does exactly?
        //fs::absolute();   line below throw an exception
        //auto fullPath = fs::canonical(fs::path{"test/results"s}); make path absolute or full path?

        // just check if move semantic is supported (looks like its working)
        auto w_tmpJan25 = createDir(fs::path{"tmp/Jan25"});  // tmp directory to store result
        auto w_newCurrPath = w_currrentWrkDirectory/w_tmpJan25; // "// \" 
        w_newCurrPath = w_newCurrPath.lexically_normal();  // become all \\ separator (normalize)
        auto checkEmptyness = fs::is_empty(w_newCurrPath); // not sure if its checking emptiness of the folder?

        // file name to start with for testing (directory already created)
        //auto w_fileNamePath = "EMcNeil1D_mod"s; // no extension, completed later
        //auto fileStus = fs::status(w_fileNamePath).type();
        
        // testing if already exist and emptiness, also add, remove file and directory
        auto w_testFileExist = w_newCurrPath/"EMcNeil1D_mod_algo_1.txt"s;
        //auto fileStus = fs::status(w_testFileExist).type();
        if( auto fileStus = fs::status(w_testFileExist).type(); 
        fileStus == fs::file_type::not_found)
        {
            std::cout << "We didn't found data file\n";

            // looks like we need to create a first file
            std::ofstream w_first2Create{w_testFileExist};
            // sanity check
            if( !w_first2Create.is_open())
            {
                std::cout << "Couldn't create file to store result\n";
            }
            w_first2Create.close();

            std::cout << "File created has been closed\n";

            // let's check about empty!!
            auto checkEmptyness = fs::is_empty(w_newCurrPath); // still true, now 
            if( !fs::is_empty(w_newCurrPath))
            {
                std::cout << "Looks like it is not empty finally\n";

                // remove file only
                fs::remove(w_testFileExist);
            }

            // check again about emptyness
            auto checStillEmpty = fs::is_empty(w_newCurrPath); // still true, now 
            if( fs::is_empty(w_newCurrPath))
            {
                std::cout << "Looks like it is empty (call fs::remove))\n";

                // remove file only (deleted only 'Jan25' folder)
                // NOTE 2 version : 
                //  1 remove(path/file) file or empty directory
                //  2 remove_all(path) remove path and recursively files and all subdirectories
                //  see N. Josuttis C++17 The Complete Guide (section 20.4.4)
                fs::remove(w_newCurrPath);

                // does it still exist?
                auto w_itExist = fs::exists(w_newCurrPath);
                // what we do next? maybe some tests about removing file or directory?
                if( fs::exists(w_newCurrPath))
                {
                    fs::remove(w_newCurrPath);
                }
            }
        }//if not_found
        
        // where i took that piece of code? Qt project I guess!!
        // check directory content
        auto dbgDir = w_currrentWrkDirectory/"DbgFeb25"sv;
        // file name to start with for testing (directory already created)
        auto w_partFileName{"EMcNeil1D_mod_algo_"s};
        fs::path w_fileNamePath = dbgDir/"EMcNeil1D_mod_algo_1"sv; // no extension, completed later
        // ... 
        if( !fs::exists(dbgDir))
        {
            assert( fs::create_directory(dbgDir)); // sanity check

            // add file to this folder
        }
        // contains file?
        if( fs::exists(w_fileNamePath))
        {    
            // const fs::directory_iterator end{};
            // const fs::directory_iterator dir_iter{dbgDir};
            // auto folder_dist = std::distance(dir_iter, end);
            // if( folder_dist == 0) // empty, no files
            // {
            //     // corresponding file name to simulation result
            //     // w_fileNamePart = "EMcNeil1D_mod"; //m_activeAlgo;
            //     w_fileNamePart.append("_algo");
            //     w_fileNamePart.append("_");
            //     w_fileNamePart.append("1"); // default
            //     w_fileNamePart += ".txt";   // file extension
            // }
            // else // not empty, contains something
            // {
            //     auto i = 0; // for what?? files counter
            //     for ( const fs::directory_entry &e :
            //          fs::directory_iterator(dbgDir))
            //     {
            //         if (fs::is_directory(e))
            //             ++i;
            //     }
            //     if (i == folder_dist)
            //     { // all directory (contains only directory)
            //       // do something here
            //         w_fileNamePart.append("_algo");
            //         w_fileNamePart.append("_");
            //         w_fileNamePart.append("1"); // default
            //         w_fileNamePart += ".txt";   // file extension
            //     }
            //     else
           // else if() { // mix of files and directories
                    //      std::vector<int> w_numberAlgo; w_numberAlgo.reserve(10);
                    // fs::path filePath{};
                    // for (const fs::directory_entry &e :
                    //      fs::directory_iterator(dbgDir))
                    // {
                    //     if (fs::is_directory(e))
                    //         continue;
                    //     filePath = e.path();
                    //     if( filePath.has_filename())
                    //     {
                    //         std::cout << "File name is: " << filePath.filename() << "\n";
                    //         continue;
                    //     } // if
                    // } // for-loop
                    
                    fs::directory_iterator begIter(dbgDir);
                    fs::directory_iterator endIter{};
                    auto nbFile = std::distance(begIter,endIter);
                    fs::path filePath{"EMcNeil1D_mod_algo_"sv};
                    std::vector<int> w_vecInt; w_vecInt.reserve(nbFile);
           //         w_vecInt.push_back(1);
                    std::map<int,std::string> w_mapKeyFileName;
                    for( const fs::directory_entry &e :
                          fs::directory_iterator(dbgDir))
                     {
                        if( e.path().has_filename())
                        {
                            auto str1 = e.path().filename().string();
                            //auto str2 = "EMcNeil1D_mod_algo"s;
                            if( contains(str1, "EMcNeil1D_mod_algo"))
                            {
                                auto lastChar = e.path().string().back();
                                auto lastDgt =  std::atoi(&lastChar);
                                w_vecInt.push_back(lastDgt);
                                //w_vecInt.pusk_back(2/*std::atoi(&e.path().string().back())*/);
                            }
                        }
                     }//for-loop
                     auto maxNo = *std::max_element(w_vecInt.cbegin(),w_vecInt.cend());
                     maxNo+=1;
                     auto &updatdInt = filePath.string().back();
                     updatdInt = *(std::to_string(maxNo).data());    
                    
                    // using file system library
                    // auto fileNameNoExt = filePath.stem(); // file name no extension
                    // short countChar{};
                    // auto rBeg = filePath.stem().string().rbegin();
                    // do
                    // {
                    //     countChar++;
                    // } while (std::isdigit(*rBeg++));
                    // if( countChar==1)
                    // {
                    //     auto lastDgts = fileNameNoExt.string().back(); // retrieve last digits
                    //     auto toInt = std::atoi(&lastDgts); // convert to int
                    //     toInt+=1; // increment 
                    //     auto &updatdInt = fileNameNoExt.string().back();
                    //     updatdInt = *(std::to_string(toInt).data());    
                    // }
                    // else {
                    //     std::string w_tmp( fileNameNoExt.string().begin(), std::prev(fileNameNoExt.string().end(),countChar));
                    //     w_tmp.append(std::to_string(2)); // temporary fix
                    //     fileNameNoExt.string().swap(w_tmp);
                    // } // more than digit (e.g.)
                    // filePath.replace_filename(fileNameNoExt.string());
                    // if (!filePath.has_extension())
                    // {
                    //     filePath.replace_extension("txt")
                    // }

                    // long w_numbr{};
                    // auto fileNamePtr = filePath.stem().string().c_str();
                    // while(*fileNamePtr && !std::isdigit(*fileNamePtr++))
                    // {
                    //     std::cout << "not a digit\n";
                    // }
                    // sscanf(fileNamePtr,"%d",&w_numbr);

           //         std::count_if();

                    // auto lastDgts = fileNameNoExt.string().back(); // retrieve last digits
                    // auto toInt = std::atoi(&lastDgts); // convert to int
                    // toInt+=1; // increment 
                    // auto &updatdInt = fileNameNoExt.string().back();
                    // updatdInt = *(std::to_string(toInt).data());
                    // fileNameNoExt = fileNameNoExt.replace_extension(filePath.extension());
                    // auto chk = fileNameNoExt.filename();
#if 0
                    std::vector<std::string> w_nameExt;
                    w_nameExt.reserve(2); // SSO
                    boost::split( w_nameExt, filePath.filename().string(),
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
#endif
                //}else
            //}else
        } // if bfs exist
#if 0
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
#endif
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