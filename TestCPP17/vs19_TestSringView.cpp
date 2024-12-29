
#include <iostream>
#include <sstream>  //istringstream
#include <charconv> // std::from_char
#include <optional> // C++17 feature
#include <string_view> // ditto

namespace vs19 
{
    // passing string view by value cheap to copy
    void strvwsFunc(std::string_view aStrvws)
    {
        // check N. Josuttis book C++17 about string view
        // good practice to verify because calling data on an 
        // empty string undefined behavior
        if( aStrvws.empty()) // check for emptyness before calling 
        {
            return; // nothing else to do
        }

        std::cout << "String view as arg to print" << aStrvws << '\n';
    }

    // not sure if it works for floating point, i know for int it does
    // tested and its works perfectly!!!
    std::optional<float> toFloat( std::string_view sv) // by-value cheap to copy
    {
        float val{};
        // read character sequence into the float
        // ptr:: pointer to the first character
        auto [ptr,ec] = std::from_chars(sv.data(), sv.data()+sv.size(),val);
        // if we have an error code, return no value
        if (ec != std::errc{}) // different from zero, mean error
        {
            return std::nullopt;
        }
        return val; // return value
    }

    // think i have tested and its working!!
    void testStringView() 
    {
        // just some tests of a feature sould use with grid lattice
        // ctor take a string_view to define the grid parameters   
        using namespace std::literals;
        strvwsFunc("JeanTestStrvs"sv); // is it good to do that?
        std::istringstream w_is{"teststrvws"sv.data()}; //don't work!!

        // try to convert to float and print the result if possible
        for(auto s : {"1.2632","0.2571","3.0100","abc","2.8"})
        {
            std::optional<float> of = toFloat(s);
            if (of) {
                std::cout << "convert '" << s << "' to float: " << *of << '\n';
            }
            else {
                std::cout << "can't convert '" << s <<"' to float\n";
            }
        }
    }
} // End of namespace