#pragma once

#include <iostream>

namespace vsc19 
{
    // NOTE
    //  Debugging the wave simulator, we need a tool that we could 
    //  use to save intermediate result or any other result in a file
    //  and inspect (check values ...). Want to suppport generic number
    //  of variables, for example flux variables and source term.
    //  Add ctor for what?? 'stream' could be ofstream ... to be completed  
    //   FF1, FF2, S, ...

    /**@brief */
    template<typename stream, typename... Columns>
    struct DbgLogFile 
    { 
        /**@brief */
        void outputLine( const Columns&... aColumns)
        {
            // use fold expression (lambda return by-value as default)
            stream myStrm;
            (myStrm << ... << [](const Columns &aCol) -> decltype(auto)
             {
                 std::cout << ' ';
                 return aCol;
             }(aColumns))
                << '\n';
        }
    };
} // End of namespace