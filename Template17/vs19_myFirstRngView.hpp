#pragma once

#include <ranges> // C++ 20

namespace vs19 
{
  // Boost range library
  template <class T, class A>
  class VectorView : public std::ranges::view_interface<VectorView<T, A>> {
  public:
    VectorView() = default;
    VectorView( const std::vector<T, A>& vec) :
      m_begin(std::ranges::next(vec.cbegin(), 1)), // similar to boost iterator range "advance_begin(1)" 
      m_end(std::ranges::prev(vec.cend(), -1))     // similar to boost iterator range "advance_end(-1)"
    {}  // .. to be completed
    auto begin() const { return m_begin; }
    auto end()   const { return m_end; }
  private:
    typename std::vector<T, A>::const_iterator m_begin{}, m_end{};
  };
} // End of namespace
