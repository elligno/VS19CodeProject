
#pragma once

#include <iostream>
#include <system_error> // error code??

// Some test with error code C++11 feature
// We should be able to convert from our enum to std::error_code:
//  std::error_code ec = FlightsErrc::NonexistentLocations;
//  Reference
//  https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/ 
//
// All this need to be tested. Also there is another feature call error category
// i am not sure what for. 
//  My understanding of the make_error_code its a converter that takes enumerator
//  and convert it as error code (error code system added to). Something that is 
//  usefull in many situations. For example in our code may have solution that do 
//  not exist, or found , for whatever reason. I must take a look at this closely. 
//  What comes up to my mind ODESolver when solving equations ... to be completed.
//
namespace vsc19 
{
    // numeric value 0 must not represent an error situation. 0 represents a success in any error domain (category).
    // we did not start the enumeration of FlightsErrc with 0. This in turn implies that we can create the enumeration 
    // that does not correspond to any of the enumerated values:
    // FlightsErrc fe {};
    enum class FlightsErrc
    {
        // no 0
        NonexistentLocations = 10, // requested airport doesn't exist
        DatesInThePast,            // booking flight for yesterday
        InvertedDates,             // returning before departure
        NoFlightsFound = 20,       // did not find any combination
        ProtocolViolation = 30,    // e.g., bad XML
        ConnectionError,           // could not connect to server
        ResourceError,             // service run short of resources
        Timeout,                   // did not respond in time
    };

    // NOTE
    // This error_code constructor is meant to be a customization hook for plugging custom error enumerations into the system. 
    // In order to plug FlightsErrc, we have to make sure that:
    // 1. std::is_error_code<Errc>::value returns true,
    // 2. Function make_error_code taking FlightsErrc is defined and accessible through argument-dependent lookup (ADL).
    // This is one of these situations where declaring something in namespace std is legal
    namespace std
    {
        template <>
        struct is_error_code_enum<FlightsErrc> : true_type {};
    }

    // just need to declare function overload make_error_code in the same namespace as enum FlightsErrc:
    enum class FlightsErrc;
    std::error_code make_error_code(FlightsErrc);\

    // And this is all that other parts of the program/library need to see, and what we have to provide in the header file. 
    // The rest is the implementation of function make_error_code and we can put it in a separate translation unit (a .cpp file).
    // With this in place, we can make an impression that FlightsErrc is an error_code:
    std::error_code ec = FlightsErrc::NoFlightsFound;
    assert (ec == FlightsErrc::NoFlightsFound);
    assert (ec != FlightsErrc::InvertedDates);

    void TestErrorCoe() 
    {
        //std::errc::read_only_file
        // Possible use of this (handle file system error code)
        // using try/catch
        try
        {
            if( !create_directory(p))
            {
                std::cout << p << "already exists\n";
            }
        }
        catch( const std::filesystem::filesystem_error& e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
            std::cout << "   path:   " << e.path1() << '\n';
        }        
    }
} // End of namespace
