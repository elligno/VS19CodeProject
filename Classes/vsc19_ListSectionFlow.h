#pragma once
// C++ includes
#include <vector>
#include <algorithm>
#include <memory>
#include <iterator>
// App includes
#include "vsc19_SectionFlow.h"

// namespace vs15 {
//   // forward declaration
//   class Classvs15;
//   class MemoryPage;
// }

namespace vsc19
{
  // don't need an const_iterator, because it is read-only, need to re-work
  // this need to be inside the class (more test to do)
  //	typedef boost::iterator_range<std::list<ListSectionFlow*>::const_iterator> ListIterator;
  // some test types (just experimenting some concept)
  //class IITereable { virtual ListIterator getListIterator() const = 0;};
  //class Observer { virtual void update()=0;};

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
  // 	Design Note:

  // TODO: this class needs a serious refactoring
  //  Observer pattern is a big mess (deprecated])
  class ListSectionFlow final //: public Observer
  {
  public:
    /** alias */
    using vecpPhysObj  = std::vector<SectionFlow*>;
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
    // must provide a default ctor 
    ListSectionFlow() = default;

    // Ctor from subject 
    // NOTE shall use a shared pointer instead, since it will be used
    // by many instance, delete it when not used by any other.
    // Pass by copy? std::shared_ptr<Classvs15>? or by refernce
    // by passing it by reference there is no ref count increment, isn't it?
    // ListListSectionFlow( std::shared_ptr<Classvs15> aClazz15);
    // also change the attribute, it's now a smart pointer. When make a copy
    //  assignment just increment the ref count, this way we make sure we do
    // not end up with dangling pointer. 
   // ListListSectionFlow( std::shared_ptr<Classvs15> aClazz15);

    //		ListListSectionFlow( SemiDiscreteModel* aSubj=nullptr, size_t aNbSections=101);
  //  ListListSectionFlow( MemoryPage* aSettings, Classvs15* aSubj = nullptr/*, size_t aNbSections = 101*/);

    /** copy ctor*/
    ListSectionFlow( const ListSectionFlow& aOther);

    /** copy ctor move semantic*/
    ListSectionFlow( ListSectionFlow&& aOther);

    /** assignment ctor*/
    ListSectionFlow& operator= ( const ListSectionFlow& aOther);

    /** move assignment ctor*/
    ListSectionFlow& operator= ( ListSectionFlow&& aOther);

    /** dtor*/
    ~ListSectionFlow();

    /** pass by pointer since we want a reference semantic
    * we are not using the boost concept pointer container
    * or even the smart pointer (shared pointer C++11) ??
    */
    void add( SectionFlow* aSectF);

    /** */
    bool contains( SectionFlow* aSectF2Find)
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
    SectionFlow* operator[] (vec_sizetype aIdx)
    {
      return m_listOfSect[aIdx];
    }
    const SectionFlow* operator[] (vec_sizetype aIdx) const
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
    vecpPhysObj& getList()
    {
      return m_listOfSect;
    }

    /** return if all sections are frictionless*/
    bool isFrictionLess() const
    {
      return (std::all_of(m_listOfSect.cbegin(), m_listOfSect.cend(),
        [](const SectionFlow* aListSectionFlow)
      {
        return aListSectionFlow->N() == 0.;
      }));
    }

    /** return if all sections are flat bed*/
    bool isFlatBed() const
    {
      return (std::all_of(m_listOfSect.cbegin(), m_listOfSect.cend(),
        [](const SectionFlow* aListSectionFlow)
      {
        return aListSectionFlow->Z() == 0.;
      }));
    }

    /** returns f all sections are unit width*/
    bool isUnitWidth() const
    {
      return (std::all_of(m_listOfSect.cbegin(), m_listOfSect.cend(),
        [](const SectionFlow* aListSectionFlow)
      {
        return aListSectionFlow->B() == 1.;
      }));
    }

    std::vector<double> getZ() const 
    {
      std::vector<double> w_z; // do i need to reserve?
      w_z.reserve( m_listOfSect.size());
      std::transform( m_listOfSect.cbegin(), m_listOfSect.cend(),
                      std::back_inserter(w_z),
                      []( const SectionFlow* aSectionFlow)
                      {
                         return aSectionFlow->Z();
                     });
      return w_z;
    }

    std::vector<double> getH() const
    {
      std::vector<double> w_H; // reserve?
      return w_H;
    }

    // update section parameter (water level) in Observer design pattern
    // should call getState() from Subject
    //  NOTE shall pass as argument the state variable 'A'
    //  void update(const scalarField1D& aA) {H_from_A ...}
    // this way we avoid of binding numerical method with list of section flow
    // example of call update(physicalSystem.getStateVector().first)
    //  something like that!! because list of sections observer of numerical 
    //  method dosn't make sense.  Anyway list of section is now part of the 
    //  physical system and the system has the state variables (A,Q).
    //void update() override;

  private:
    // we could also implement as a vector of shared_ptr<SectionFlow>
    // ref count when copying and assigning
    // shall use the shared_ptr type to make sure the clean-up is done 
    vec_sizetype m_NbSections;
    vecpPhysObj m_listOfSect;
    bool deleteIt();                   // helper function to clean-up the list
    void createListSectionsFlow();
    //std::shared_ptr<Classvs15> m_subj;  subject of Observer pattern
  };
} // End of namespace 
