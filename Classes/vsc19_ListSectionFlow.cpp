
// C++ include
#include <cassert>
#include <functional> //mem_fn
// app include
#include "vsc19_ListSectionFlow.h"

namespace vsc19
{
  // =================================
  //       ListSectionFlow
  // =================================

  // ListSectionFlow::ListSectionFlow( std::shared_ptr<Classvs15> aClazz15)
  // : m_NbSections(5),
  //   m_subj(aClazz15) // copy ctor (ref count is ?)
  // {
  //  // m_subj = new Classvs15;
  //   m_listOfSect.reserve(m_NbSections);
  //   createListSectionsFlow();

  //   std::cout << "ListSectionFlow def ctor\n";
  // }
  
  ListSectionFlow::ListSectionFlow( ListSectionFlow&& aOther)
  {
    std::cout << "ListSectionFlow move copy ctor\n";

    m_NbSections = aOther.m_NbSections;

    // what about copy? swapping resources i do not think we need a copy?
    m_listOfSect = std::move(aOther.m_listOfSect); // does it leave the vector with nullptr? yes
    //m_subj = aOther.m_subj;   should i do that, may experiment a dangling pointer?
                             // no, we swap resources 

    // we make it smart pointer for this reason, now it becomes 
  //  m_subj = std::move(aOther.m_subj);

    // not sure about this
    aOther.m_NbSections = 0;
    aOther.deleteIt(); // really need to do that? delete all elements in the list since we move above??
    //aOther.m_subj = nullptr;  set subject to nullptr, yes!! done with move assignment 
  }

  ListSectionFlow::ListSectionFlow( const ListSectionFlow& aOther)
  {
    std::cout << "ListSectionFlow copy ctor\n";

    m_NbSections = aOther.m_NbSections;

    // NOTE
    //  make deep copy not a shallow copy, for this reason we do the following
    // i think i need to allocate section flow and use copy ctor of section flow
    // By using a container of shared_ptr, we wouldn't have to do a deep copy
    // like this, actually we will call the copy of the shared_ptr which would 
    // increment the count on the pointed-to object 
    m_listOfSect.reserve(aOther.m_listOfSect.size());
    auto begList = aOther.m_listOfSect.cbegin();

    // deep copy?? i do think so, create a brand new object by copy
    while( begList != aOther.m_listOfSect.cend())
    {
      // call push_back() from vector
      add( new SectionFlow(*(*begList))); // call copy ctor
      ++begList; // next in the list
    }

    // by doing this, copy pointer by value (std::vector copy ctor) not copying the whole data
    // i think we end up with dangling pointer if the source is deleted since we hold pointer 
    //m_listOfSect = aOther.m_listOfSect;  not sure, do i need to copy the vector content?

    // Same concern, by assigning pointer like this, is it safe? dangling pointer?
    //m_subj = aOther.m_subj; // ??? by deleting the source we end up with dangling pointer
    // not anymore, by declaring the subject as smart pointer, we can now assign it 
    // with concern with dangling since we increment ref count, got deleted when ref count = 0
  }

  ListSectionFlow& ListSectionFlow::operator= ( const ListSectionFlow& aOther)
  {
    std::cout << "ListSectionFlow assignment ctor\n";

    // check for self-assignment
    if( this != &aOther)
    {
      m_NbSections = aOther.m_NbSections;
      if( !m_listOfSect.empty())
      {
        deleteIt(); // delete all pointer in the list
      }
      if( m_listOfSect.size() != 0) // calling deleteIt delete pointer, not removing element, clear remove all elements
      {
        m_listOfSect.clear();
      }

      // optimize by avoiding re-allocation when container grow
      m_listOfSect.reserve(aOther.m_listOfSect.size());
      // not really all we do here is to copy pointer by value, we have to do a deep copy
      //std::copy( aOther.m_listOfSect.cbegin(), aOther.m_listOfSect.cend(), std::back_inserter(m_listOfSect));
      auto begList = aOther.m_listOfSect.cbegin();
      // deep copy?? i do think so, create a brand new object by copy
      while( begList != aOther.m_listOfSect.cend())
      {
        // call push_back() from vector
        add( new SectionFlow(*(*begList))); // call copy ctor
        ++begList; // next in the list
      }
      // how to make sure that we don't end up with a dangling pointer?
    //  m_subj = aOther.m_subj; // ??? by deleting the source we end up with dangling pointer
      // Smart pointer assignment now, no more worry
    }

    return *this;
  }

  ListSectionFlow& ListSectionFlow::operator= ( ListSectionFlow&& aOther)
  {
    std::cout << "ListSectionFlow move assignment ctor\n";

    // check for self-assignment
    if (this != &aOther)
    {
      m_NbSections = 0;
      if (!m_listOfSect.empty())
      {
        deleteIt(); // delete all pointer in the list
      }
      if (m_listOfSect.size() != 0) // sanity check
      {
        m_listOfSect.clear();
      }

      // leave the other in constructible state? not sure
      m_NbSections = aOther.m_NbSections;
      m_listOfSect = std::move( aOther.m_listOfSect);
    //  m_subj = std::move( aOther.m_subj);

      // ?? not sure what you mean
      aOther.m_NbSections = 0;
      aOther.deleteIt();       // really need to do that? delete all elements in the list since we move above??
      //aOther.m_subj = nullptr;  set subject to nullptr
    }

    return *this;
  }

  // not completed, case where size!=0 i call clear,
  // i don't know if i should do it
  bool ListSectionFlow::deleteIt()
  {
    if (!m_listOfSect.empty())
    {
      // might as well to delete it
      auto w_begList = m_listOfSect.begin();
      while (w_begList != m_listOfSect.end())
      {
        SectionFlow* w_sect2del = *w_begList;
        if (nullptr != w_sect2del)
        {
          delete w_sect2del;
          w_sect2del = nullptr;
        }
        ++w_begList;
      }

      // sanity check (check size, actually size still the same
      // i just deleted the pointer but did not remove them from the list)
      std::vector<SectionFlow*>::size_type w_size = m_listOfSect.size();
      if( w_size != 0)
      {
        m_listOfSect.clear(); // ???
                              // check if size is zero
        if (m_listOfSect.size() != 0)
        {
          return false;
        }
      } // if
    } // if

    // NOTE
    //  By declaring now the subject as pointer, we make sure we don't have such a situation
    //  We delete pointer but we have assigned another resource to this (without smart pointer)
    //  Nice case of dangling pointer, anyway there is no point to delete the pointer, since
    //  it is an assignment!!!
//     if( nullptr != m_subj)
//     {
//       delete m_subj;
//       m_subj = nullptr;
//     }

    return true;
  }

   void ListSectionFlow::createListSectionsFlow()
  {
//     double w_xCurr = aDbdata->x_min(); // x-coordinate of the node
//     auto nbSect = aDbdata->nbSections();
    //assert( nbSect == anbSections);

     auto w_xCurr = 0.;
    // set each section flow 
    for( auto j = 0; j < m_NbSections; ++j) // check the loop number of elements
    {
      // ...
      add( new SectionFlow(j, w_xCurr));
//       m_listOfSect[j]->setB(aDbdata->getWidth());
//       m_listOfSect[j]->setN(aDbdata->getManning()[j]);
//       m_listOfSect[j]->setZ(aDbdata->getBathy()[j]);
      // sanity check
//       assert(true == m_listOfSect[j]->isUnitWidth());
//       assert(true == m_listOfSect[j]->isFrictionLess());
//       assert(true == m_listOfSect[j]->isFlatBottom());

      // ready for next section
      w_xCurr +=0.1;
    }

    // hard coded (debugging purpose)
    assert( m_NbSections == size());

    // debugging purpose (print all information about a section)
    std::for_each( getList().cbegin(), getList().cend(),
      std::mem_fn( &SectionFlow::printSectionInfo));

//    return boost::optional<int>();
  }

  void ListSectionFlow::add(SectionFlow* aSectF)
  {
    // check for capacity and and size
    m_listOfSect.push_back(aSectF);
  }

  ListSectionFlow::~ListSectionFlow()
  {
    if (!m_listOfSect.empty())
    {
      // delete pointer from the list
      deleteIt();
      m_listOfSect.clear(); // empty list 
    }
  }

#if 0
  // is it completed (i think what we do here,
  // we update sections from 0 to 99, domain of 100 nodes)
  // 101 node is the ghost node set by BC
  void ListSectionFlow::update()
  {
    // be careful with "auto" keyword type deduction
    // here we have vector<SectFlow*>::iterator, 
    // Scot Meyers mention to use it whenever you can, as well const iterator
    auto w_beg = m_listOfSect.cbegin();
    auto w_end = m_listOfSect.cend() - 1; // only computational domain (ghost node not include)

                                          // cast base class to derived for type checking
                                          // Shall use the boost polymorphic downcast utility
                                          // which throw an exception on bad_cast
                                          // checking Observer type
    if (EMcNeil1d_f* w_typeOk = dynamic_cast<EMcNeil1d_f*>(m_subj))
    {
      // retrieve state of the subject
      StateVector w_fieldVec = w_typeOk->getState();
      std::vector<double> w_A;
      w_A.reserve(w_fieldVec.first->values().size());
      w_fieldVec.first->values().to_stdVector(w_A);

      // do something with the state 
      unsigned i(0);
      while (w_beg != w_end) // computational domain 
      {                      // but we need also the extra section
        SectFlow* w_currSect = *w_beg++;
        w_currSect->setH(HydroUtils::Evaluation_H_fonction_A(w_A[i++], w_currSect->B(), w_currSect->Z()));
      }
    }
    else if (EMcNeil1d_mod* w_typeOk = dynamic_cast<EMcNeil1d_mod*>(m_subj))
    {
      StateVector w_fieldVec = w_typeOk->getState();
      std::vector<double> w_A;
      w_A.reserve(w_fieldVec.first->values().size());
      w_fieldVec.first->values().to_stdVector(w_A);

      // do something with the state 
      unsigned i(0);
      while (w_beg != w_end) // computational domain 
      {                      // but we need also the extra section
        SectFlow* w_currSect = *w_beg++;
        w_currSect->setH(HydroUtils::Evaluation_H_fonction_A(w_A[i++], w_currSect->B(), w_currSect->Z()));
      }
    }
    else if (TestEMcNeilVec* w_typeOk = dynamic_cast<TestEMcNeilVec*>(m_subj))
    {
      StateVector w_fieldVec = w_typeOk->getState();
      std::vector<double> w_A;
      w_A.reserve(w_fieldVec.first->values().size());
      w_fieldVec.first->values().to_stdVector(w_A);

      // do something with the state 
      unsigned i(0);
      while (w_beg != w_end) // computational domain 
      {                      // but we need also the extra section
        SectFlow* w_currSect = *w_beg++;
        w_currSect->setH(HydroUtils::Evaluation_H_fonction_A(w_A[i++], w_currSect->B(), w_currSect->Z()));
      }
    }
    else if (TestBcSectF* w_typeOk = dynamic_cast<TestBcSectF*>(m_subj))
    {
      StateVector w_fieldVec = w_typeOk->getState();
      std::vector<double> w_A;
      w_A.reserve(w_fieldVec.first->values().size());
      w_fieldVec.first->values().to_stdVector(w_A);

      unsigned i{};
      while (w_beg != w_end)
      {
        SectFlow* w_currSect = *w_beg++;
        w_currSect->setH(HydroUtils::Evaluation_H_fonction_A(w_A[i++], w_currSect->B(), w_currSect->Z()));
      }
    }
    else if (TestNewAlgo* w_typeOk = dynamic_cast<TestNewAlgo*>(m_subj))
    {
      StateVector w_fieldVec = w_typeOk->getState();
      std::vector<double> w_A;
      w_A.reserve(w_fieldVec.first->values().size());
      w_fieldVec.first->values().to_stdVector(w_A);

      unsigned i{};
      while (w_beg != w_end)
      {
        SectFlow* w_currSect = *w_beg++;
        w_currSect->setH(HydroUtils::Evaluation_H_fonction_A(w_A[i++], w_currSect->B(), w_currSect->Z()));
      }
    }
    // A,B,Z
    // 	   std::vector<SectFlow*>::iterator w_beg = m_listOfSect.begin();
    // 	   std::vector<SectFlow*>::iterator w_end = m_listOfSect.end();
    // 	   unsigned i(0);
    // 	   while( w_beg!=w_end) // computational domain 
    // 	   {                    // but we need also the extra section
    // 		   SectFlow* w_currSect = *w_beg++;
    // 		   w_currSect->setH( HydroUtils::Evaluation_H_fonction_A( w_U1[i++], w_currSect->B(), w_currSect->Z()));
    // 	   }

    // 	   for (SectFlow* w_hVal : m_listOfSect)
    // 	   {
    // 		   std::cout << w_hVal->H() << "\n";
    // 	   }
    // debugging purpose (print all information about a section)
    // 	   std::for_each( m_listOfSect.cbegin(),m_listOfSect.cend(),
    // 		   std::mem_fn( &emcil::SectFlow::H));
  }
#endif
} // End of namespace