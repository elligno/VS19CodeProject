
#pragma once
// C++ includes
#include <vector>
#include <algorithm>
//#include <memory>
//#include <iterator>
// App include
#include "vsc19_SectionFlow.h"

using float64 = double;

namespace vsc19
{
  // Usage:
  //   This concept of forward list implement traversing from top to bottom
  //   Global instance or sole instance of this class since
  //   it can't be any more than one list of section flow ON.
  //  Iterate through the collection with out knowing  
  //   ListListSectionFlow w_test;
  // 	BOOST_FOREACH(ListListSectionFlow::ListIterator::reference w_sect, w_test.getEnumerator())
  // 	{
  // 		w_sect.setH(12);
  // 		//std::cout << str << "\n";
  // 	}
  class ListSectionFlow final
  {
  public:
    /** alias */
    using vecpPhysObj  = std::vector<SectionFlow>;
    using vec_sizetype = vecpPhysObj::size_type;
    using vec_valtype  = vecpPhysObj::value_type;
    using vec_iter     = vecpPhysObj::iterator;
    using vec_citer    = vecpPhysObj::const_iterator;

    /** some utility method*/
    vec_iter begin() { return m_listOfSect.begin(); }
    vec_iter end() { return m_listOfSect.end(); }
    vec_citer begin() const { return m_listOfSect.cbegin(); }
    vec_citer end()   const { return m_listOfSect.cend(); }

  public:
    /** pass by pointer since we want a reference semantic
    * we are not using the boost concept pointer container
    * or even the smart pointer (shared pointer C++11) ??
    */
    void add( SectionFlow&& aSectF);
    void add(const SectionFlow& aSect2Add);

    /** */
    bool contains( const SectionFlow& aSectF2Find)
    {
      // use the find algorithm, of the stl ()
      if( std::find( m_listOfSect.begin(), m_listOfSect.end(), aSectF2Find) != m_listOfSect.end())
      {
        // found something
        return true;
      }
      return false;
    }
    // Design Note: should i return a reference?
    SectionFlow& operator[] (vec_sizetype aIdx)
    {
      return m_listOfSect[aIdx];
    }
    const SectionFlow& operator[] (vec_sizetype aIdx) const
    {
      return m_listOfSect[aIdx];
    }

    bool isEmpty() const { return m_listOfSect.empty(); }
    vec_sizetype size() const { return m_listOfSect.size(); }

    // returns make_iterator_range (read only)
    vecpPhysObj getList() const
    {
      return m_listOfSect;
    }

    // i am not user that i want the user (or let the user to modify the section flow list)
    vecpPhysObj getList()
    {
      return m_listOfSect;
    }
    /** return if all sections are frictionless*/
    bool isFrictionLess() const
    {
      return (std::all_of(m_listOfSect.cbegin(), m_listOfSect.cend(),
        [](const SectionFlow& aListSectionFlow)
      {
        return aListSectionFlow.N() == 0.;
      }));
    }

    /** return if all sections are flat bed*/
    bool isFlatBed() const
    {
      return (std::all_of(m_listOfSect.cbegin(), m_listOfSect.cend(),
        [](const SectionFlow& aListSectionFlow)
      {
        return aListSectionFlow.Z() == 0.;
      }));
    }

    /** returns f all sections are unit width*/
    bool isUnitWidth() const
    {
      return (std::all_of(m_listOfSect.cbegin(), m_listOfSect.cend(),
        [](const SectionFlow& aListSectionFlow)
      {
        return aListSectionFlow.B() == 1.;
      }));
    }

    std::vector<double> getZ() const 
    {
      std::vector<double> w_z; // do i need to reserve?
      w_z.reserve( m_listOfSect.size());
      std::transform( m_listOfSect.cbegin(), m_listOfSect.cend(),
                      std::back_inserter(w_z),
                      []( const SectionFlow& aSectionFlow)
                      {
                         return aSectionFlow.Z();
                     });
      return w_z;
    }

    // ??? what for?
    std::vector<double> getH() const
    {
      std::vector<double> w_H; // reserve?
      return w_H;
    }

    // update section parameter (water level)  
    // Anyway list of section is now part of the 
    //  physical system and the system has the state variables (A,Q).
    void update( const std::vector<float64>& aHvec) 
    {
      for( auto& w_SectF : m_listOfSect)
      {
        // water level update
        w_SectF.setH(aHvec[w_SectF.getId()]);
      }
    }
  private:
    vecpPhysObj m_listOfSect;
  };
} // End of namespace 
