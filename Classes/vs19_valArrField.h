#pragma once

// numeric array fast floating pint operation
#include <valarray>
#include <vector>

// App include
#include "vs19_GridLattice.h"

namespace vsc19 
{
  /**
    * @brief The class "valArrField" is an abstraction of the numerical
    *        concept of a field, a scalar field over a grid.
  */
  class valArrField 
  {
  public:
    using gridlatticeptr = std::shared_ptr<GridLattice>;

   /**
    * @brief alias template
    * @tparam T type
   */
    template<typename T>
    using dblvarrayT = std::valarray<T>;

    /**
     * @brief STL-container like interface
    */
    using dblvarray = std::valarray<double>;
    using value_type = dblvarray::value_type;
    using pointer = value_type*;
    using iterator = pointer;
    using const_pointer = const value_type*;
    using const_iterator = const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using range_diff_type = ptrdiff_t;

    // Replace all the lines above (next version or release)
    //		SFX_DECLARE_CONTAINER( float64=aType)
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /**
     * @brief Iterator access 
     * @return iterator
    */
    const_iterator cbegin() const;
    iterator begin();
    const_iterator cend() const;
    iterator end();
    const_reverse_iterator rbegin() const;
    reverse_iterator rbegin();
    const_reverse_iterator rend() const;
    reverse_iterator rend();

    typedef typename std::iterator_traits<iterator>::value_type value_type;
   // typedef std::bidirectional_iterator<iterator> iterator_category;
   // typedef std::difference_iterator<iterator> difference_type;
  //  typedef value_type* pointer;
  //  typedef value_type& reference;


  public:
    /**
     * @brief 
     *   
     * @param aGrid 
     * @param aName 
    */
    // Design Note: maybe pass by value and move on it??
    // maybe pas as tring view
    valArrField( const gridlatticeptr& aGrid, std::string aName);
    /**
     * @brief 
     * @param aGrid 
     * @param aName 
     * @param aValue 
    */
    valArrField(const gridlatticeptr& aGrid, std::string aName, double aValue);
    /**
     * @brief 
    */
    valArrField(const gridlatticeptr& aGrid, std::string aName, const std::vector<double>& aValue);
    /**
     * @brief 
     * @tparam ForeignVector must provide data() return a pointer to raw data
     * @param aGrid field grid
     * @param aName name
     * @param aValue 
    */
    template<typename ForeignVector>
    valArrField( const gridlatticeptr& aGrid, std::string aName, const ForeignVector& aValue);

    // Design Note
    // Should we pass by value?
 /*   valArrField( std::shared_ptr<GridLattice> aGrid, std::string aName)
    {
      m_gridLattice = aGrid;
      m_name = aName;
    }*/

    // no user defined copy, assignment operator ctor and dtor
    // move semantic supported

    // enable access to grid-point values: (field values)
          dblvarray& values()       { return *m_gridValues; }
    // NOTE
    // According C++17 new rules for Copy Elison maybe i should return by value.
    // Also, compiler may decide to move instead of copy, not too sure about this.
    // Check N. Josuttis "Move Semantics: The complete Guide"
    const dblvarray& values() const { return *m_gridValues; }

    // enable access to the grid:
          GridLattice& grid()       { return *m_gridLattice;; }
    // See NOTE above
    const GridLattice& grid() const { return *m_gridLattice;; }

    /**
     * @brief returns the field name
     * @return string 
    */
    std::string name() const { return m_name; }

    /**
     * @brief Check emptyness
     * 
     * @return true/false
    */
    [[nodiscard]] bool isEmpty() noexcept { return (*m_gridValues).size() == 0; }

    // set the values of the field  
    void values( dblvarray& new_array);

    /**
     * @brief addable field operator
     * @param aOther type to add
     * @return added field
    */
    const valArrField& operator+= ( const valArrField& aOther)
    {
      // valarray built-in operator+=
      *m_gridValues += aOther.values();
      return *this;
    }

    /**
     * @brief Addable scalar operator
     * @param double scalar to be added
     * @preturn added field 
    */
    const valArrField& operator+= ( double aValue)
    {
      //valarray built-in operator+=
      *m_gridValues += aValue;
      return *this;
    }

    /**
     * @brief subtractable field operator
     * @param aOther type to add
     * @return subtracted field
    */
    const valArrField& operator-= ( const valArrField& aOther)
    {
      // valarray built-in operator-=
      *m_gridValues -= aOther.values();
      return *this;
    }

    /**
     * @brief subtract scalar operator
     * @param double scalar to subtract
     * @return subtrated field
    */
    const valArrField& operator-= ( double aValue)
    {
      //valarray built-in operator-=
      *m_gridValues -= aValue;
      return *this;
    }
  private:
    std::shared_ptr<GridLattice> m_gridLattice; /**< finite-diffference grid */
    std::shared_ptr<dblvarray>   m_gridValues;  /**< field values */
    std::string m_name;                         /**< filed name */
  };

  // Design Note
  //   See Scott Meyer's "Effective Modern C++" item #25
  //   Use std::move references, std::forward rferences
  // 
    
  // return std::move(aLhs) to avoid copy (aLhs use as a storage 
  // to hold sum of the matrices). move lhs into return value
  // (lhs is move into the function's return value)
  // NOTE since C++11 valarray support the move semantic
  
  /**
   * @brief
   * @param aLhs type to add
   * @param aRhs type to add
   * @return
  */
  valArrField operator+ ( valArrField&& aLhs, const valArrField& aRhs);
  valArrField operator+ ( valArrField&& aLhs, double aValue);
  // Math operations
  //valArrField operator+ ( const valArrField& aV1, const valArrField& aV2);  disable this one
  valArrField operator+ ( double aValue,     valArrField&& aRhs);
  valArrField operator+ ( const valArrField& aLhs, const valArrField& aRhs); 

  /**
   * @brief 
   * @param aLhs type to add
   * @param aRhs type to add
   * @return 
  */
  valArrField operator- ( valArrField&& aLhs, const valArrField& aRhs);
  valArrField operator- ( valArrField&& aLhs, double aRhs);
  valArrField operator- ( double aLhs, valArrField&& aRhs);  
  /**
   * @brief 
   * @param aLhs type to add
   * @param aRhs type to add
   * @return added field
  */
  valArrField operator* ( valArrField&& aLhs, const valArrField& aRhs);
  valArrField operator* ( valArrField&& aLhs, double aRhs);
  valArrField operator* ( double aLhs, valArrField&& aRhs);

  /**
   * @brief Printing element of container or a scalar 
   * @param rem ...
   * @param aVector type value to print
   * @param newline return end of line 
  */
  void printValarray( std::string_view rem, // cheap to copy
    const auto& aType2Print, bool newline = false)
  {
    // check if its a scalar by using type trait 
    if constexpr( std::is_scalar_v<std::decay_t<decltype(aType2Print)>>)
      std::cout << rem << " : " << aType2Print;
    else // otherwise its a container
    {
      for( std::cout << rem << " : { "; const auto vecElem : aType2Print)
        std::cout << vecElem << ' ';
      std::cout << '}';
    }
    std::cout << (newline ? "\n" : ";  ");
  }

#if 0  // o = printValarray
  int main()
  {
    std::valarray<int> x, y;

    o("x", x = { 1, 2, 3, 4 }), o("y", y = { 4, 3, 2, 1 }), o("x += y", x += y, 1);
    o("x", x = { 4, 3, 2, 1 }), o("y", y = { 3, 2, 1, 0 }), o("x -= y", x -= y, 1);
    o("x", x = { 1, 2, 3, 4 }), o("y", y = { 5, 4, 3, 2 }), o("x *= y", x *= y, 1);
    o("x", x = { 1, 3, 4, 7 }), o("y", y = { 1, 1, 3, 5 }), o("x &= y", x &= y, 1);
    o("x", x = { 0, 1, 2, 4 }), o("y", y = { 4, 3, 2, 1 }), o("x <<=y", x <<= y, 1);

    std::cout << '\n';

    o("x", x = { 1, 2, 3, 4 }), o("x += 5", x += 5, 1);
    o("x", x = { 1, 2, 3, 4 }), o("x *= 2", x *= 2, 1);
    o("x", x = { 8, 6, 4, 2 }), o("x /= 2", x /= 2, 1);
    o("x", x = { 8, 4, 2, 1 }), o("x >>=1", x >>= 1, 1);
  }
#endif
} // End of namespace
