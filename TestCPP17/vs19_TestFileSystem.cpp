
#include <filesystem>

namespace vs19 
{
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
        auto w_myLoc {R"(E:\JeanProjects\TestVS2019)"sv};

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
        
    }
} // End of namespace