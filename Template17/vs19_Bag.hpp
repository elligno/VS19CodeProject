#pragma once

namespace vsc19 { template<typename> class BagIterator; }
namespace vsc19 
{
  /**
   * @brief Bag structure taken from "C++ Black Belt"
   * @tparam T element of the bag (not unique element)
  */
  template<typename T>
  class Bag
  {
  public:
    /**
     * @brief Create a bag of pointer
     * @param aNext number element in the bag
     * @param aSiz  capacity of the bag
     * @param owner owner of the bag
    */
    Bag( bool owner = true) : m_ownner{ owner }, m_element{nullptr}, m_next{ 0 }, m_size{ 0 } {}
    
    /**
     * @brief Copy ctor 
     * 
     * @param aOther Bag to copy from
    */
    Bag( const Bag& aOther);

    /**
     * @brief Assignment ctor 
     * 
     * @param aOther Bag to copy from
     * @return new assigned bag
    */
    Bag& operator= ( const Bag& aOther);

    /**
     * @brief destructor
    */
    ~Bag();

    /**
     * @brief 
     * @return 
    */
    int lenght() const { return m_next; }
   
    /**
     * @brief Add an element to the bag 
     * @param aElem element to be added
     * @return nothing
    */
    void add( T* aElem);

    /**
     * @brief Element access operator 
     * 
     * @param aIdx bag index
     * @return bag element
    */
    T* operator[] (int aIdx) const { return m_element[aIdx]; }

    /**
     * @brief friend template class Bag_terator
     * @tparam Bag element 
    */
    template<typename> friend class BagIterator;

  private:
    bool m_owner;                          /**< owner of the bag*/
    T**  m_element;                        /**< bag element type*/
    int  m_next;                           /**< current size*/
    int  m_size;                           /**< capacity*/
    static constexpr int m_increment = 20; /**< resize increment*/
  };

  /**
   * @brief Iterator for Bag structure
   * 
   * @tparam T Bag Element type 
  */
  template<typename T>
  class BagIterator 
  {
  public:
    BagIterator( Bag<T>& aBag) : m_bag{ aBag }, m_currentIdx{0} {}
    ~BagIterator() = default;
    BagIterator( const BagIterator&) = delete;
    BagIterator& operator= ( const BagIterator&) = delete;
    T* operator -> () { return m_bag.m_element[m_currentIdx]; } // not sure about this one
    T* top() const { return m_bag.m_element[m_currentIdx=0]; }
    T* end() const { return m_bag.m_element[m_currentIdx = m_bag.m_next - 1]; }
    T* current() const { return m_bag.m_element[m_currentIdx]; }
    bool more()  const { return (m_currentIdx < m_bag.m_size); }
    int index()  const { return m_currentIdx;}
    int lenght() const { return m_bag.lenght(); }
    // pre-increment
    BagIterator& operator++ () { ++m_currentIdx;  return *this; }
    BagIterator& operator-- () { --m_currentIdx;  return *this; }
    // post-increment
    const BagIterator operator++ (int) { BagIterator w_tmp = *this; ++m_currentIdx; return w_tmp; }
    const BagIterator operator-- (int) { BagIterator w_tmp = *this; --m_currentIdx; return w_tmp; }
  private:
    Bag<T>& m_bag;     /**< Bag container*/
    int m_currentIdx;  /**< Iterator index*/
  };
  
  template<typename T>
  Bag<T>::~Bag()
  {
    // delete all element of the bag 
    if( nullptr != m_element)
    {
      for( auto i = 0; i < length(); ++i)
      {
        delete m_element[i];
      }

      delete [] m_element;
      m_element = nullptr;
      m_next = m_size = 0;
    }
  }

  template<typename T>
  void Bag<T>::add(T* aElem) 
  {
    // check size (at initialization)
    //if( m_size == 0)
    //{
    //  m_size += m_increment;
    //  // allocate memory
    //  m_element = new T* [m_size];
    //}
    if( m_next >= m_size) // full 
    {
      // re-allocate memory
      m_size += m_increment;
      // allocate memory for new array
      T* w_tmp = new T* [m_size];
      if( m_next)
      {
        // copy existing element in tmp
        for( auto i = 0; i < m_next; ++i)
        {
          w_tmp[i] = m_element[i];
        }
        if( m_element)
        {
          // delete original array (pointer)
          delete [] m_element;
          m_element = nullptr;
        }
      }//if

      // assign to new re-sized array (pointer)
      m_element = w_tmp;
    }//if

    // add elem to the bag  
    m_element[m_next++] = aElem;
  }

  template<typename T>
  Bag<T>::Bag( const Bag<T>& aOther)
  {
    m_element = new T* [aOther.m_size];
    m_owner = false;
    m_next = aOther.m_next;
    m_size = aOther.m_size;

    // copy element 
    std::copy( aOther.m_element, aOther.m_element+aOther.m_size, m_element);
  }

  template<typename T>
  Bag<T>& Bag<T>::operator= (const Bag<T>& aOther)
  {
    // check self-assignment
    if( aOther != *this)
    {
      for( auto i = 0; i < length(); i++)
      {
        delete m_element[i];
      }
      delete[] m_element;

      m_element = new T * [aOther.m_size];
      std::copy(aOther.m_element, aOther.m_element + aOther.m_size, m_element);

      m_owner = false;
      m_next = aOther.m_next;
      m_size = aOther.m_size;
    }

    return *this;
  }
} 
// End of namespace
