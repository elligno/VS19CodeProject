
// stl include
#include <algorithm>
// library include
#include "NodalVariables.h"
#include "../vs19_PrintUtils.hpp" // print tuple

namespace vsc19
{
	//
	// Design Note (July 2015)
	//   These methods need a serious clean-up, it's a big mess.
	//   Specially the attribute seems to be mixed up, i am not 
	//   but a closer look will help!! 
	
	// default ctor (these are default values and have no meaning)
	Nodal_Value::Nodal_Value()
	: m_NumOfAttr (   3 ),
	  m_TieNode_No(  -1 ),
    m_nodeIdX   {0,0.},
    m_Node3Data { 0.,0.,0. }
	{
// 		m_nodeIdX.first=0;    // node id
// 		m_nodeIdX.second=0.;  // node x-coord.
// 
// 		std::get<0>(m_Node3Data)=0.; // A 
// 		std::get<1>(m_Node3Data)=0.; // Q
// 		std::get<2>(m_Node3Data)=0.; // H
	}
// 	Nodal_Value::Nodal_Value( nodeIdX aNodeId, int32 aNumOfAttr/*=1*/,
// 		int32 aTieNode/*=-1*/)
// 	 : m_NumOfAttr ( aNumOfAttr ),
// 	   m_TieNode_No( aTieNode   ),
// 	   m_nodeIdX   ( aNodeId   ),
//      m_Node3Data (aNodeId.first,0.,0.)
// //	  m_NodeNumber(   aNodeId.first),
// 	{
// // 		std::get<0>(m_Node3Data)=0.; // A 
// // 		std::get<1>(m_Node3Data)=0.; // Q
// // 		std::get<2>(m_Node3Data)=0.; // H
// 	}

	Nodal_Value::Nodal_Value( nodeIdX aNodeXId, int aNumAttr, tuplevar aTuple, 
		int aTiedNode/*=-1*/)
		: m_NumOfAttr ( aNumAttr  ),
		  m_TieNode_No( aTiedNode ),
		  m_nodeIdX(aNodeXId),
		  m_Node3Data( aTuple)
	{
		// not much to do for now
	}

	// copy ctor
// 	Nodal_Value::Nodal_Value( const Nodal_Value& aOther)
// 	{
// 		m_nodeIdX    = aOther.m_nodeIdX;
// //		m_NodeNumber = aOther.m_NodeNumber;
// 		m_NumOfAttr  = aOther.m_NumOfAttr; 
// 		m_TieNode_No = aOther.m_TieNode_No;	   
// 		m_Node3Data  = aOther.m_Node3Data;  // tuple version
// 	}

  // Don't need that
	// destructor
//	Nodal_Value::~Nodal_Value()
//	{
		// debugging purpose
	//	std::cout << "We are in NValue destructor\n";
//	}

// 	Nodal_Value::Nodal_Value( unsigned aId, int aNumAttr, int aTiedNode/*=-1*/)
// 	: m_nodeIdX.first(aId),
// 	  m_NumOfAttr ( aNumAttr  ),
// 	  m_TieNode_No( aTiedNode )
// 	{
// 		// ...
// 		m_Node3Data( aTuple)
// 	}

	// assignment ctor
// 	Nodal_Value& Nodal_Value::operator= ( const Nodal_Value& aOther)
// 	{
// 		// check for self assignment
// 		if( &aOther != this)
// 		{
// 			m_nodeIdX    = aOther.m_nodeIdX;
// //			m_NodeNumber = aOther.m_NodeNumber;
// 			m_NumOfAttr  = aOther.m_NumOfAttr;
// 			m_TieNode_No = aOther.m_TieNode_No;
// 			//		m_Node2Data=aOther.m_Node2Data;  // deprecated
// 			m_Node3Data  = aOther.m_Node3Data;
// 		}

// 		return *this;
// 	}

	// ___________________________________________________
	//
	//  friend function
	std::ostream& operator<< ( std::ostream& s, Nodal_Value& nv) 
	{
		s << "{";
		// output tie node number to indicate been tied
		if( nv.tie_node_no() == -1) s << nv.node_no() << " | (";
		else s << nv.node_no() << "-" << ( nv.tie_node_no()) << " | {";  
        // last time i used it, it wasn't working, it crash!!!
        PRINT_TUPLE<0,3,double,double,double>::print(s, nv.Values());
        s << ") } ";

		// printing nodal attributes
		// auto checkNumofdim = nv.no_of_dim();
		// for( auto i = 0; i < nv.no_of_dim(); i++)
		// {
		// 	if( i != nv.no_of_dim()-1) s << ( nv[i]) << ", ";
		// 	else s << ( nv[i]) << ") } ";
		// }
		// 	// 		for( int32 i = 0; i < nv.no_of_dim(); i++) 
		// 	// 		{ 
		// 	// 			if(i != nv.no_of_dim()-1) s << ( *nv.Values()) << ", ";
		// 	// 			else s << ( *nv.Values()) << ") } ";
		// 	// 			++nv.Values();
		// 	// 		}
		// 	// 		
		return s;
	}

} // End of namespace