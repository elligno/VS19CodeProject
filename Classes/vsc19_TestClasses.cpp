
#include <iostream>
#include <ranges>   //C++20 ranges library
#include "TestTemplVS19.hpp"

namespace vsc19 
{
   // some basic tests
    void testClasses()
    {
        std::cout << "Try to make it compile at least\n";
        std::array<int,5> w_array2Foo {1,2,3,4,5}; // initialize data
        // w_vec2Foo.reserve(5);
        // w_vec2Foo.push_back(1);
        // w_vec2Foo.push_back(2);
        // w_vec2Foo.push_back(3);
        // w_vec2Foo.push_back(4);
        // w_vec2Foo.push_back(5);
        // // generic lambda C++ 20
        int w_arrayType[3];
        w_arrayType[0]=4;
        w_arrayType[1]=2;
        w_arrayType[2]=8;
        class A{};
        auto chck = std::is_array_v<A[3]>;
        A w_arrA[3];
        std::vector w_vecInt {1,2,3,4,5};
        call2Foo(w_vecInt); // passing an lvalue 
        std::cout << "Looks like we have made the call succesfull\n";
    }
} // End of namespace