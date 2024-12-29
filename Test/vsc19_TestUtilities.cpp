//#include <stdio.h>
#include <string_view>
#include <filesystem>
//#include <optional>    
// app include
#include "../Template17/vs19_Utilities.hpp" // 

namespace vsc19 
{
    using dbltpl = std::tuple<double, double, double, double, double>;

    // ...
    using pairtimeline =
      std::pair<double /*Time*/, std::vector<dbltpl> /*line of values*/>;

    std::vector<pairtimeline> gVecTimeValues; /*< store data to be visualized at a
                                                 given time (x-coord/value)*/
                                                 
// Read full result file X:
//   ====================
//   Time: 0.0000 sec.
//   ====================
//
// X         H          U1             U2                 V
// 0.02    0.02         0.02          0.02              0.02
  std::vector<pairtimeline> 
     readDataFullResultFromFile( const std::filesystem::path &aFile2read) 
  {
      bool w_bStartReadVal{false}; // initialize

      // maybe should use std vector because we have the shrinktofit
      // reduce memory usage (capacity)
      // CurvesData2Show w_curvData;
      if (!gVecTimeValues.empty())
      {
          gVecTimeValues.clear(); // should be empty!!!
      }

      gVecTimeValues.reserve(50); // for now will do

      // QTextStream w_fileStream{&aFile2read};
      // std::ifstream w_fileStream{aFile2read,std::ios::};
      std::ifstream w_fileStream{aFile2read, std::ios::in}; // readonly
      if (w_fileStream.is_open())
      { // open file for reading
          std::string line{};
          double w_timeVal{}; // time at each iteration

          // reading line after one
          while (!w_fileStream.eof())
          {
              // w_fileStream.readLineInto(&line);
              std::getline(w_fileStream, line);
              if (w_bStartReadVal)
              {
                  auto w_vecOfTpl = // extract line values (vector of tuple)
                      extractLineValuesTmplt<double, double, double, double, double>(
                          &w_fileStream, std::move(line));

                  w_bStartReadVal = false; // according to the current format

                  // add it to vector of pair pair<Time, values>
                  gVecTimeValues.push_back(
                      {w_timeVal, std::move(w_vecOfTpl)});
                  continue; // ready to continue to next Time: 0.0034 for example
              }

              if (line.empty())
              {
                  continue; // go to next line
              }
              // not empty
              // auto w_lineTrimmed = line.trimmed();
              auto w_firstchar = line.find_first_not_of(' ');
              auto w_lastchar = line.find_last_not_of(' ');
              std::string w_lineTrimmed{std::next(line.cbegin(), w_firstchar),
                                        std::prev(line.cend(), w_lastchar)};
              // replace code below (Qt project)
              std::stringstream w_iss{w_lineTrimmed};
              std::vector<std::string> results(std::istream_iterator<std::string>{w_iss}, // start range
                                               std::istream_iterator<std::string>{});     // end range
              if ((results.front() != std::string{"X"}) && w_bStartReadVal == false)
              {
                  // string splitted token as white space
                  if (results.front() == std::string{"Time:"})
                  {
                      w_timeVal = std::stod(results.front());
                  }
                  continue; // next line
              }
              else
              {
                  // flag to specify we start reading values
                  w_bStartReadVal = true;
                  continue;
              }

#if 0   
      w_lineTrimmed.find('X');
      if (!w_lineTrimmed.contains('X') && w_bStartReadVal == false) {
        // Time: 0.0019 sec.   (file format)
        if (w_lineTrimmed.contains("Time")) {
          // split about ":"
          auto w_timeStr = w_lineTrimmed.split(QString{":"});
          w_timeStr.back().remove(
              QRegularExpression{"[sec]"}); // remove all ocurence of "sec"
          w_timeStr.back().remove(w_timeStr.back().size() - 1,
                                  1); // remove dot at the end (sec.)
          w_timeVal = w_timeStr.back().toDouble(); // contains value + sec.
        }
        continue; // next line
      } else {
        // flag to specify we start reading values
        w_bStartReadVal = true;
        continue;
      }
#endif // 0
          } // while-loop
      } // if

      // acquisition and release resource (symtric)
      //   if (aFile2read.is4Open())
      //     aFile2read.close();

      if (w_fileStream.is_open())
      {
          w_fileStream.close();
      }

      return gVecTimeValues;
  }

  // file system path as arg (not a string)
  // Right, for now we pass a string_view (cheap to copy)
  bool LoadData( std::string_view aFileName)
  {
      // File system!!!!!
      namespace fs = std::filesystem;
      using namespace std::literals;
      fs::path w_rootDir{R"(E:\JeanProjects\TestVS2019)"sv};
      auto w_dataPath{w_rootDir/"data"};
      auto checkRootD = w_dataPath.root_directory();
      if( !fs::exists(w_dataPath))
      {
        // create directory (are we creating data folder?)
        auto succeed = fs::create_directory(w_dataPath);
        if (!succeed)
        {
            return false;
        }
      }

      // C++20 u8 string format (should alwauys use this format)
      std::vector<std::u8string> w_dirEntry;
      w_dirEntry.reserve(30);
      // check directory entry
      for( const auto& entry: fs::directory_iterator(w_dataPath))
      {
        if( entry.status().type() == fs::file_type::regular)
        {
            std::cout << "This entry is a file name: " << reinterpret_cast<const char*>(entry.path().lexically_normal().u8string().c_str()) << '\n';
        } else if (entry.status().type() == fs::file_type::directory)
        {
            std::cout << "This entry is a file name: " << reinterpret_cast<const char*>(entry.path().lexically_normal().u8string().c_str()) << '\n';
        }
        else if (entry.status().type() == fs::file_type::unknown) 
        {
            std::cout << "Unknown entry\n";
        }

        // add to the list 
        w_dirEntry.push_back(entry.path().u8string());
      }
      // default working directory
      fs::path w_wrkDirDef {R"(E:\JeanProjects\TestVS2019\data)"sv};
      auto w_code = w_rootDir.compare(w_wrkDirDef);
      if( fs::equivalent(w_rootDir,w_wrkDirDef))
      {
         std::cout << "We are on the right track\n";
      }
     

      // simulation data file name
      auto w_file2Load = w_rootDir / aFileName;
      std::cout << "Simulation data file is : " << w_file2Load.lexically_normal().string();
      
      //fs::path w_file2Load{w_rootDir};
      //w_file2Load /= aFileName;
      //auto retVal = w_file2Load.lexically_normal();

      // Reading data from simulation result
      auto w_pairTimeValues = readDataFullResultFromFile(w_file2Load);
      if (w_pairTimeValues.size()==0)
      {
        return false;
      }
      
      // extract data (tuple are in reverse order)
      // must perform some manipulation to bring it
      // back as it should be (increasing order)

#if 0 // debugging purpose (don't understand why tuple index are in ereverse order)
  auto curTime0 = m_curvData.m_vecTimeValues[1].first;   // time=0.019
  auto frValues0 = m_curvData.m_vecTimeValues[1].second; // vec values
  // Automatic Template Argument Type Deduction (Universal Reference)
  auto &&tpl48{frValues0[48]}; // lvalue reference
  auto &&tpl49{frValues0[49]}; // ditto
  auto &&tpl50{frValues0[50]}; // ditto
  auto &&tpl51{frValues0[51]}; // ditto
  // mapping of the returned tuple revert index
  // to new tuple in correct order (index increasing)
  auto w_reverted48 = tuple_inverse_idx_order<dbltpl, 5, 4, 3, 2, 1, 0>(
      tpl48, std::index_sequence<5, 4, 3, 2, 1, 0>{});
  // swap tuple element (revert to standard order)
  tpl48.swap(w_reverted48);

  auto w_reverted49 = tuple_inverse_idx_order<dbltpl, 5, 4, 3, 2, 1, 0>(
      tpl49, std::index_sequence<5, 4, 3, 2, 1, 0>{});
  // swap tuple element
  tpl49.swap(w_reverted49);

  auto w_reverted50 = tuple_inverse_idx_order<dbltpl, 5, 4, 3, 2, 1, 0>(
      tpl50, std::index_sequence<5, 4, 3, 2, 1, 0>{});
  // swap tuple element
  tpl50.swap(w_reverted50);

  auto w_reverted51 = tuple_inverse_idx_order<dbltpl, 5, 4, 3, 2, 1, 0>(
      tpl51, std::index_sequence<5, 4, 3, 2, 1, 0>{});
  // swap tuple element
  tpl51.swap(w_reverted51);

  std::cout << "Initial time value is " << curTime0 << '\n';
  auto fr48 = std::get<5>(tpl48);
  auto fr49 = std::get<5>(tpl49);
  auto fr51 = std::get<5>(tpl50);
  auto fr52 = std::get<5>(tpl51);
#endif

        return false;
}

} // End of namespace