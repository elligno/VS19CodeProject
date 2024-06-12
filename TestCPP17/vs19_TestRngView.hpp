#pragma once

// C++20 includes
#include <ranges>      // C++20 feature
#include <algorithm>   // ranges algorithm C++20
// STL include
#include <vector>
// 
#include "../Template17/vs19_myFirstRngView.hpp"
#include "../Template17/vs19_myFirstRng.hpp"

//
// NOTE 
//  As far as i my undestanding,
namespace vs19 
{
  template <typename T> struct scalar_t { T t{}; /* no begin/end */ };
  static_assert(not std::ranges::range< scalar_t<int> >);

  //// Boost range library
  //template <class T, class A>
  //class VectorView : public std::ranges::view_interface<VectorView<T, A>> {
  //public:
  //  VectorView() = default;
  //  VectorView(const std::vector<T, A>& vec) :
  //    m_begin(std::ranges::next(vec.cbegin(), 1)), // similar to boost iterator range "advance_begin(1)" 
  //    m_end(std::ranges::prev(vec.cend(), -1))     // similar to boost iterator range "advance_end(-1)"
  //  {}  // .. to be completed
  //  auto begin() const { return m_begin; }
  //  auto end()   const { return m_end;   }
  //private:
  //  typename std::vector<T, A>::const_iterator m_begin{}, m_end{};
  //};

  // HLL algorithm 
  template<typename ArrayType>
  void hllAlgorithm( const ArrayType& aStdVec)
  {
    static_assert( std::ranges::range< range_t<std::vector<int>> >);

    // if with initialization and follow by conditon since C++17
    // Also compile-time if statement
    if constexpr (range_t<ArrayType> rng; std::ranges::range<decltype(rng)>) 
    { 
      // type is a range, that is provides a begin iterator and end sentinel
      std::cout << "ArrayType is a range with the following number of elements " << std::ranges::size(aStdVec) << "\n";

      //std::ranges::size(aStdVec);

      VectorView w_vecView(aStdVec); // Class Template Argument Deduction?? CTAD
      //  std::ranges::sort(w_vecView);
      auto val = w_vecView.back();
      w_vecView.front();
      auto begIter = w_vecView.begin();
      auto nbElems = w_vecView.size();

      const auto w_minElem = std::ranges::minmax_element(w_vecView);

      for( const auto& val : w_vecView)
      {
        std::cout << "Value is: " << val << '\n';
      }

      if( w_vecView.begin() != w_vecView.end())
      {
        std::cout << "Range not empty\n";
      }
    }
  }

} // End of namspace
