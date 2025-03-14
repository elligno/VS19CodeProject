
#pragma once

//C++ include
#include <ranges>
// App include
#include "..\Classes\valarrField1D.h"

namespace vsc19 
{
   /// @brief Testing some concept in our programming environment
   ///  Inherit from ... to enable our scalarField type to become 
   ///  a view (chdeap to copy/move).
   class scalarField1DView :  // view enabled
      public std::ranges::view_interface<scalarFiled1D>
   {
    using scalarField1D::begin;
    using scalarField1D::end;

    public:

    // ctor??
    scalarField1DView()=default;
    // copy ctor (range ctor)
    
    //??? not sure about these 2
    auto begin() const { return m_begin; }
    auto end()   const { return m_end; }
    
    private:
    scalarField1D::const_iterator m_begin{}, m_end{};
   };
} // End of namespace