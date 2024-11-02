#include <stdio.h>
#include <string_view>
#include <filesystem>
#include <optional>
// app include
#include "../Template17/vs19_Utilities.hpp" //id17

namespace vsc19 
{
    // file system path as arg (not a string)
    // Right, for now we pass a string_view (cheap to copy)
    bool LoadData( std::string_view aFileName)
    {
        // Shall be revised. We shall use ifstream instead of FILE
        FILE *w_InputFile = nullptr;
        char w_readPrm[16];
        float w_valPrm{};

        // raw string
        using namespace std::literals;
        const auto w_filePath = R"(E:\JeanProjects\TestVS2019)"s;
        //const auto w_pathFile = w_filePath + "\\" + aFileName;
       // std::cout << "Params of run path: " << w_pathFile;

        // File system!!!!!
        namespace fs = std::filesystem;
        fs::path w_file2Load {w_filePath};
        w_file2Load /= aFileName;
        auto retVal = w_file2Load.lexically_normal();
        
/*         //std::ifstream w_testFile;
        // Clear the log file contents ("ParamOfRun.txt")
        if (( errno_t w_err = fopen_s(&w_InputFile, w_file2Load , "r")); w_err == 0)
        {
            // NOTE shall return std::optional
            if (nullptr == w_InputFile)
            {
                return false;
            }
            // read parameters of run from file
            while (m_ReadPar)
            {
                fscanf_s( w_InputFile, "%s%f", w_readPrm,                           // fmt: string.float
                         static_cast<unsigned int>(sizeof(w_readPrm)), &w_valPrm); // safe version

                // fscanf( w_InputFile, "%s%f", w_readPrm, &w_valPrm);
                //			if( Sfx::id( wPar, "mL"         ))m_L          = w_valPrm;
                if( auto val = vsc19::id17(w_readPrm, "mDt"); val.has_value()) {
                     auto m_TimeStep = w_valPrm
                }
            }
        }
        else
        {
            std::cerr << "Cannot open file for reading simulation parameters.\n";
        }
/*         
                if (vsc19::id17(w_readPrm, "mDx"))
                    auto m_Dx = w_valPrm;
                if (Sfx::id(w_readPrm, "mCFL"))
                    auto m_CFL = w_valPrm;
                if (Sfx::id(w_readPrm, "mNbSections"))
                    auto m_NbSections = static_cast<int>(w_valPrm);
                if (Sfx::id(w_readPrm, "mNbIterMax"))
                    auto m_NbIterMax = static_cast<int>(w_valPrm);
                //		if( id( par,"Nts"))Nts=(int)w_valPrm;
                if (Sfx::id(w_readPrm, "end"))
                    m_ReadPar = static_cast<int>(w_valPrm);
            } // while-loop
        } // if
        else
        {
            // Sfx::ErrorMsg("Cannot open file.\n");
            std::cerr << "Cannot open file for reading simulation parameters.\n";
        }

        // Opening file for reading simulation parameters.

        // Close file for writing
        if (nullptr != w_InputFile)
        {
            fclose(w_InputFile);
        }
        return true;
 */  
return false;

}

    // void testSomeUtilities() 
    // {
    //     // temp fix for debugging
    //     auto isLoaded = LoadData( std::string_view{"MyFileName"});
    // }

} // End of namespace