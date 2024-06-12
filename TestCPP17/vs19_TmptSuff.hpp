#pragma once

namespace vs19 
{
  /**
   * @brief Variable template define constant
   *  Usage
   *    ... to be completed
  */
  template<auto v>
  struct myAppConstant
  {
    static constexpr auto T = v;
  };

  // declare variable for the app
  using myAppCte = myAppConstant<101>; 
} // End of namespce
