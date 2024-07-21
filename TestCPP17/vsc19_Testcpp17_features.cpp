//std C++ includes
#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <set>
// C++17 includes
#include <filesystem>
#include <optional>
#include <variant>
#include <any>

namespace vsc19 
{
    std::optional<double> getOptionalRet() { return std::optional<double>(2.);}

    auto getPaitValue()  { return std::make_optional(std::pair<int,int>{1,3});}

    void testOptional() 
    {
        struct testStruct // aggregate, can be initalized with curly brace
        {
            float m_flt;
            int m_int;
            auto operator <=> (const testStruct& aOther) const = default;
        }; // since C++17 can inherit from 

        // create optional object
        std::optional<int> w_opt{std::nullopt};
        auto valOpt = w_opt.has_value(); // shall return a bool
        // value() throw an exception if no value available
        if( w_opt.has_value())
        {
            // value() return by reference
            std::cout << std::format( "We have an optional value of {} for this test", w_opt.value());
        }
         
        if( w_opt != std::nullopt)
        {
            /* code */
        }
        else { // is null value ("*" return by reference)
            std::cout << std::format("We optional value is: {} \n", *w_opt); 
        }

        // type deduction?
        std::optional w_opt2{2};
        auto check = w_opt2.has_value();
        w_opt2 = std::nullopt; // no more value
        // could use reset()
        check = w_opt2.has_value();
        auto val = *w_opt2; // doesn't throw exception, undefined behavior

        using namespace std::literals;
        auto w_strv = "afdf"sv;
        std::optional w_optstrv{w_strv};

        // fallback but not sure if i could use it
        auto fback = w_optstrv.value_or("Fallback");

        // do not work, compile error == (Need to define spaceship operator <=>)
        // initialize optional struct ctor (these 2 are the same)
        std::optional<testStruct> w_optStruct({1.f,2});
        std::optional<testStruct> w_optStruct1( std::in_place, 1.f, 2);
        const auto fltVal = w_optStruct->m_flt;
        const auto fltVal1 = w_optStruct1->m_flt;
        auto isEqual = w_optStruct==w_optStruct1;
        
        // check for equality
   //     if( w_optStruct == w_optStruct1)
   //     {
    //        std::cout << std::format("Both construct of optional are the same");
   //     }
        
        if( w_optStruct.has_value())
        {
            std::cout << std::format( "Optional struct values are {} and {}", w_optStruct->m_flt, w_optStruct->m_int);
        }
        else {
            w_optStruct.emplace(2.3f,7); // reset value if not any
        }

        // since value() return a reference (getOptionalRet) temp object do not extend life)
        // destroyed and we have reference (extend life of value()) but temp object deleted
        auto w_refRet = getOptionalRet().value(); // return by ref 
        std::cout << "Value returned by reference is: " << w_refRet << "\n";

        // define lambda compare
        auto scmpt = [] ( const int& arg1, const int& arg2)
        {
            return std::abs(arg1) < std::abs(arg2);
        };
        // why we need to use decltype?
        std::optional<std::set<int,decltype(scmpt)>> w_optSet{std::in_place,{2,4,1,8,23}, scmpt};
    }
    void testVariant() 
    {
        // want to test vector of variant (set alternative value for each element of the vector)
        std::vector<std::variant<int, float, std::string>> w_vecVariant{5};
        w_vecVariant[0] = std::string{"JeanTestVAriant"}; // alternative value
        w_vecVariant[1] = 1; //ditto
        w_vecVariant[2] = 1.f; //ditto
        w_vecVariant[3] = 1.45f; //ditto
        w_vecVariant[4] = "jbtest"; // convert to std::string (implicit)

        //visitor () apply an operation to the alternative value
    }
    void testAny() {}
    void testFileSys() {}
} // End of namespace