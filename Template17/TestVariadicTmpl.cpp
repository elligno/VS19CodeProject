#include <cassert>
#include <cctype>
#include <tuple>
#include "vs19_Utilities.hpp"

namespace vsc19 
{
    void funcInt(int aVal) {aVal+1;}
    void mkFrmTpl(int aInt1, int aInt2, float aDbl) 
    {
      aInt1 = 1+aInt2;
      std::cout << aDbl << "\n";
    }

    // Just testing the basic of variadic template
    void testSomeBasicTmpl()
    {
        // struct with variadic template attribute
        jbTpl<int,int,float> w_jeanCheck; // 3 elements
        w_jeanCheck.m_tpl = {1,2,3};

       using checkpl = jbTpl<int,int,float>;
       auto nbElem = std::tuple_size_v<checkpl>; // C++17 template
       
       using tpl = jbTpl<int,int,float>; 
       using tpl0 = std::tuple_element<0,decltype(w_jeanCheck.m_tpl)>;
     //  static_assert( std::is_same<tpl0, int>::type); not a template function, not done at compile-time, but not sure

       std::cout << std::boolalpha << std::is_same_v<tpl0, // compare type
                         int> 
         << '\n'; 
       
       //assert( std::is_same_v<tpl0, int>);

       auto vv = std::get<1>(w_jeanCheck.m_tpl);

       if( nbElem > 3)
       {
          std::cout << "Exceed number of elements of the tupe\n";
       }

       //...(tpl)
       // structured binding to retrieve tuple element
       auto [e1,e2,e3] = w_jeanCheck.m_tpl;  // C++17 "tie" equivalent
       std::make_from_tuple<mkFrmTpl>( std::move(w_jeanCheck.m_tpl));
    }
} // End of the namespace