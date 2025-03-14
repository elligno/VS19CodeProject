
#pragma once

// C++ includes
#include <cassert>
#include <tuple>
// App include
#include "../vs19_AppConstant.hpp"

namespace vs19 
{
  // apply on a tuple
  template<typename... Ts>
  std::ostream& operator<< (std::ostream& os, std::tuple<Ts...> const& theTuple)
  {
    std::apply
    (
        [&os]( const Ts&... tupleArgs)
        {
            os << '[';
            std::size_t n{0};
            ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
            os << ']';
        }, theTuple
    );
    return os;
  }
  
  // just testing variadic template could be used NodalValue
  // in the DamBreak++ Wave Simulator tool box
  template <typename... Params> // parameter pack
  struct jbTpl
  {
    bool m_tiedNode=false;       // tied node (half-open bondary)
    std::tuple<Params...> m_tpl; // pack expansion

    using value_type = std::tuple_element_t<1, decltype(m_tpl)>;
    
    jbTpl() = default;           // to be used in a collection (ctor size)
    //jbTpl(Params... Prms) : m_tiedNode{false}, m_tpl(Prms...) {}
    jbTpl(Params... Prms, bool tied=false) : m_tiedNode{tied},m_tpl(Prms...) {}
    
    auto size() const     { return std::tuple_size_v<decltype(m_tpl)>; }

    // auto nodeData() const {
    //   if (isGNode()) return std::get<1>(m_tpl);
    //   return std::make_tuple(std::get<1>(m_tpl),std::get<2>(m_tpl),std::get<3>(m_tpl)); 
    // }
    
    auto nodeIdx() const  { return std::get<0>(m_tpl); }

    // default both node at each end is tied
    auto tieNodeIdx() const
    {
      if(isTiedNode()) {
        return std::get<0>(m_tpl);
      } else {
        return -1;
      }
    }
    /**  @brief 
     *   
    */
    auto isGhostNode() const
    {
      assert(std::get<0>(m_tpl) < vsc19::EMCNEILNbSections::value);
      return std::get<0>(m_tpl) == vsc19::DIM::value;
    }
    /**  @brief 
     *   tuple implement '<=>' spaceship operator
    */
    bool isGNode() const {return (std::tuple_size_v<decltype(m_tpl)>) == 2;}

    /**  @brief 
     *   
    */
    bool isTiedNode() const
    {
      assert(std::get<0>(m_tpl) < vsc19::EMCNEILNbSections::value); // default prototype
      return m_tiedNode;
    }

    /**  @brief logical operator ==,!=, <,<=,>,>= data member
     *   tuple implement '<=>' spaceship operator
     */
    auto operator<=>(const jbTpl &aOther) const = default;

    // attributes
    // bool m_tiedNode;
    // short m_nodeIdx;
    // short m_tieNodeIdx;
  };//jbtpl

  // pattern creation
  template <typename... Params>
  struct jbTpl1
  {
    std::tuple<short, Params...> m_tplPatrn;
  };

  template <typename... Params>
  struct jbTpl2
  {
    std::tuple<std::unique_ptr<Params>...> m_tpluniqPtr; // intantiate for each arg in the pack
  };

// Pattern is the following:
// if the parameter pack contains types: int, float,int
// pair<string,int>, pair<string,float>, pair<string,int> result
// tuple<pair<string,int>, pair<string,float>, pair<string,int>>
template<typename... Prms>
struct TestTplPtrn
{
  // create a pattern base on the pack expansion
  std::tuple<std::pair<std::string,Prms>...> m_tplPrmsPtrn;
};

// let compiler deduce the return type
template<typename... Ts>
auto createPairTpl() // not sure about this one!!!
{
  // not sure about this one
  return make_tuple(TestTplPtrn<Ts...>);
}

} // End of namespace