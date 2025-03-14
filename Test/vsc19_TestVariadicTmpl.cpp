// C++ includes
#include <iostream>
#include <cassert>
#include <cctype>
#include <tuple>
#include <type_traits> //is_invocablle
#include <concepts> // C++20 
// ...
#include <functional>
// STL include
#include <vector>
// App include
//#include "../Template17/vs19_Utilities.hpp"
#include "../Template17/vs19_NodeVarTmpl.hpp"  // Nodal_Value
// boost includes
#include <boost/iterator/transform_iterator.hpp>

#if 0
// Reference
//  https://johnfarrier.com/breaking-down-c20-callable-concepts/

// C++20 concepts
template <std::invocable F>
void call_if_callable(F func) {
    std::cout << "Callable!n";
    func();
}

void callIfCallable( std::invocable auto func)
{
  std::cout << "Callable!n";
  func();
}

// A simple, pure function
double squareRoot(double x) {
    return std::sqrt(x);
}

// Template function that requires a RegularInvocable
template <std::regular_invocable<double> T>
double computeAndPrint(T&& callable, double value) {
    double result = callable(value);
    std::cout << "Result: " << result << "n";
    return result;
}

// Using the function that accepts a std::regular_invocable
  //  computeAndPrint(squareRoot, 16.0);  Valid, as squareRoot is side-effect free and consistent

    // Lambda example
 //   auto lambda = [](double x) { return x * x; };
 //   computeAndPrint(lambda, 4.0); // Also valid for the same reasons

// A function with side effects, making it non-regular invocable
void nonRegularFunction(int a) {
    static std::default_random_engine engine;
    std::uniform_int_distribution<int> dist(1, 10);
    int randomValue = dist(engine);
    std::cout << "Function called with " << a << ", random value: " << randomValue << "n";
}

// Template function that requires an Invocable
template <std::invocable<int> T>
void invokeWithArg(T&& callable) {
    callable(42);
}

// Using the function that accepts a std::invocable
// invokeWithArg(nonRegularFunction);  Valid as an invocable, but not regular_invocable

// Function template using std::invocable
template<std::invocable F>
void myFunction(F func) {
    std::cout << "Invoking function...n";
    func(); // Invoke the callable
}
#endif //0

// Lambda function
    auto printMessage = []() {
        std::cout << "Hello, C++20 Concepts!n";
    };

    // Pass the lambda to the function template
   // myFunction(printMessage);

    // Function pointer
    void (*funcPtr)() = []() { 
        std::cout << "Function pointer called.n"; 
    };

    // Pass the function pointer to the function template
    //myFunction(funcPtr);

    // Uncommenting the following line will cause a compile-time error
    // myFunction(5); // Error: '5' does not satisfy std::invocable

namespace vsc19 
{
    using Nodal_Value = vs19::jbTpl<unsigned /*id*/, double /*A*/, double /*Q*/, double /*H*/>;
    using Constraint_Node = vs19::jbTpl<unsigned /*id*/, double /*A*/, double /*Q*/, double /*H*/>;
    using GNode = vs19::jbTpl<unsigned /*id*/, double /*x-coord*/>;

    void funcInt(int aVal) {std::cout << "Value is: " << aVal <<'\n'; }
    
    void mkFrmTpl(int aInt1, int aInt2, int aInt3) noexcept // doesn't throw
    {
      aInt1 = 1+aInt2;
      std::cout << aInt3 << "\n";
    }
    
    void mkFrmNodl(int aInt1, double aDbl, double aDbl1, double aDbl2) 
    {
      std::cout << "testing invoke on function taking" << "\n";
    }

    void invokeWithNodalVar( Nodal_Value aTestNvar)
    {
      // shall take a look on laptop vscode project about a utility to print tuple
      // it use std::apply, not what for 
      std::cout << "Testing passing arg of Tmplt struct\n";
    }

    // Just testing the basic of variadic template
    void testSomeBasicTmpl()
    {
        // struct with variadic template attribute
        vs19::jbTpl<int,int,int> w_jeanCheck; // 3 elements
        w_jeanCheck.m_tpl = {1,2,3};

       //using checkpl = jbTpl<int,int,float>;
       auto nbElem = w_jeanCheck.size(); // std::tuple_size_v<decltype(checkpl)>; // C++17 template
       
       using tpl = vs19::jbTpl<int,int,float>; 
       using tpl0 = std::tuple_element<0,decltype(w_jeanCheck.m_tpl)>;
     //  static_assert( std::is_same<tpl0, int>::type); not a template function, not done at compile-time, but not sure
       // compare type
       std::cout << std::boolalpha << std::is_same_v<tpl0,int>  << '\n'; 

       auto vv = std::get<1>(w_jeanCheck.m_tpl);

       if( nbElem > 3)
       {
          std::cout << "Exceed number of elements of the tupe\n";
       }

       // structured binding to retrieve tuple element
       auto [e1,e2,e3] = w_jeanCheck.m_tpl;  // C++17 "tie" equivalent
       std::apply(mkFrmTpl,w_jeanCheck.m_tpl);
    }

    // Nodal variable concept (DamBreak++ Wave Simulator toolbox)
    void NodalVarTest()
    {
   //   using MyNdal = vs19::jbTpl;

      // struct with tup[le attribute]
      // vsc19::jbTpl<float,double,double> defTpl;
      vs19::jbTpl<unsigned, float, double, double> testTplCtor(1, 1.f, 2.5, 4.2);
      auto tplSiz = testTplCtor.size();
      auto [id, A, Q, H] = testTplCtor.m_tpl; // C++17 structured binding
      using tpl1 = std::tuple_element_t<1,decltype(testTplCtor.m_tpl)>;
      static_assert(std::is_same_v<tpl1, float>);
      static_assert(std::is_same_v<std::tuple_element_t<1, decltype(testTplCtor.m_tpl)>, float>);
      using Ndatatype = std::tuple_element_t<1,decltype(testTplCtor.m_tpl)>;
      std::pair<Ndatatype,Ndatatype> w_chkType = std::make_pair(1.f,2.f);

    //  using Nodal_Value = vs19::jbTpl<unsigned /*id*/, double /*A*/, double /*Q*/, double /*H*/>;
      std::vector<Nodal_Value> w_Uh; // nodal value at grid node (ndf: number of degree freedom)
      w_Uh.reserve(3); // Since C++17 return a ref (placement new)
      auto& chk = w_Uh.emplace_back(0, 1., 2.5, 4.2);   // id,A,Q,H
      w_Uh.emplace_back(1, 2.4, 0.5, 14.2); // ditto
      w_Uh.emplace_back(2, 2., 20.5, 1.2);
      assert(3 == w_Uh.size());
      // check some values (structured binding)
      auto [id0, Am, Qm, Hm] = w_Uh[0].m_tpl; // tuple 
      assert(!w_Uh[0].isTiedNode());
      auto valDbl = std::get<1>(w_Uh[0].m_tpl);   // A-value
      w_Uh[0] = Nodal_Value{3, 0., 0.5, 4.2};     // assign ctor
      w_Uh[0] = {3, 0.1, 0.45, 41.2};             // aggregagte tuple-like API
      auto [id3, Am1, Qm1, Hm1] = w_Uh[0].m_tpl;  // structured binding
     // auto w_dat = w_Uh[0].nodeData();

      //tied node (ghost node) test basic operator support
      Nodal_Value w_tiedNode(DIM::value,Am1,Qm1,Hm1,true); // ghost node
      Nodal_Value w_cpyNdalV = w_tiedNode; // cpy construct
      assert(w_cpyNdalV.isTiedNode()); // must equal
      w_tiedNode = w_Uh[0]; // assign ctor node_id=3
      assert(!w_tiedNode.isTiedNode()); // that's right!!!
      auto cmpNdl = w_cpyNdalV <=> w_tiedNode; // equal node (greater >0)
      // sanity check comparison category C++20 
      if( auto cmpNdl = w_cpyNdalV <=> w_tiedNode; cmpNdl!=0)
      {
        std::cout << "These 2 nodal value type are not equal\n";
      }
      
      // move semantic??? yes it is supported!!
      static_assert(std::is_move_constructible_v<Nodal_Value>);
      static_assert(std::is_move_constructible_v<decltype(w_cpyNdalV.m_tpl)>);
      //  static_assert(std::is_trivially_move_constructible_v<Nodal_Value>);
      Nodal_Value w_mvNode = std::move(w_tiedNode);
      auto myTpl = w_cpyNdalV.m_tpl;
      auto mvTpl  = std::move(myTpl);
      //auto w_mvNode = std::move(w_cpyNdalV).m_tpl;
      auto asd = w_cpyNdalV.m_tpl;
      auto&& w_dlaydMv = std::move(w_tiedNode);
      
      GNode w_rightNode{0,0.,true}; // this node is not a tied anymore
      auto isGnodeTied = w_rightNode.isTiedNode();
      int int1=1,int2=2,int3=3;
      using funcNoExcpt = void (*)(int,int,int) noexcept;
      // see N. Josuttis book Move Semantic p. 130
      funcNoExcpt myFunc = std::move(mkFrmTpl); // assign a pointer-to-function
      
      // testing type trait (compile time evaluation) need to use static assert
      // assert that this function can be invoked and not throwing any exception 
      static_assert( std::is_nothrow_invocable_v<funcNoExcpt,decltype(int1),decltype(int2),decltype(int3)>);
      std::invoke(myFunc,int1,int2,int3); // invoke the function 

      // basic tests make_from_tuple?? what does it do? std::make_tuple(1,2.,3.2,.4)
      auto w_nvar17 = std::make_from_tuple<Nodal_Value>(std::make_tuple(1,2.,3.2,.4));
      
      // is it possible? it sure is!!!
      invokeWithNodalVar(std::make_from_tuple<Nodal_Value>(std::make_tuple(1,2.,3.2,.4)));

      // testing extracting values from U_h as we usually do
      //auto w_getA =  lambda function (doesn't)
      std::function<double(const Nodal_Value &)> w_getA =
          []( const Nodal_Value &aVal) -> double
          { return std::get<1>(aVal.m_tpl); };

      // std::function<double(const Nodal_Value &)> w_getQ =
      //     [](const Nodal_Value &aVal) -> double { return aVal[1]; };

      // std::function<double(const Nodal_Value &)> w_getH =
      //     [](const Nodal_Value &aVal) -> double { return aVal[2]; };

      //   switch (aNvalComp) {
      //   case NodalValComp::A: {
      // using delegate function
      using namespace boost;
      std::vector<double> aVec2Fill(3);
      //aVec2Fill.reserve(3);
      aVec2Fill.assign( make_transform_iterator(w_Uh.begin(), w_getA),
                        make_transform_iterator(w_Uh.end(), w_getA));

      // auto vec0 = aVec2Fill[0];
      // auto vec1 = aVec2Fill[1];
      // auto vec2 = aVec2Fill[2];
      std::cout << "Value of U_h for A variable are: \n";
   //   vsc19::printAddspace10<double, double, double>(aVec2Fill[0], aVec2Fill[1], aVec2Fill[2]);
      //  }
      // sanity check
      assert(3 == aVec2Fill.size());

      //testing variadic template pattern (2 pair fo string,int,float)
      // number of tuuple element is 2!!
      vs19::TestTplPtrn<int,float> w_testPtrn;
      w_testPtrn.m_tplPrmsPtrn = {{std::string{"pair1"},1},{std::string{"pair2"},2.f}};
    }
} // End of the namespace