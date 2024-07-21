#pragma once

// C++ include
#include <iostream>
// STL includes
#include <vector>
#include <tuple>
#include <list>
// boost includes
#include <boost/assert.hpp>
// Dsn library
//#include "../Dsn/SfxLib/Dsn_PrintUtils.hpp" // PrintTuple

namespace vsc19
{
	// Concept
	// -------
	//   Nodal_Value type is a model of node variable, in a simulation
	//   there a set of variables associated with mesh node, and Nodal_Value
	//   represent those node variables.
	
	// nodal type that hold  variable value
	class Nodal_Value  
	{		
  public:
    enum eVarType {A=0, Q=1, H=2};
	public:
		// typedefs
		using int32 = int;
		using float64 = double;
		using nodeIdX = std::pair<int32,float64>;                 // represent node Id and coordinate
		using tuplevar = std::tuple<float64/*A*/, float64 /*Q*/, float64 /*H*/>; // represent (A,Q,H)
		template<typename... Params>
		using nodeDataType = std::tuple<Params...>;
	public:		
		// default ctor (i am not sure about this one!!)
		Nodal_Value(); // set attribute values?
		//Nodal_Value( nodeIdX aNodeXId, int aNumOfAttr=1, int aTieNode=-1);
		Nodal_Value( nodeIdX aNodeXId, int aNumAttr, tuplevar aTuple, int aTiedNode=-1);

        // This will contains all information about a node (id,x,values,tied node)
		// <unsigned, double, double,double,double, signed>
		// GNode: id,x, tied node 
		// NodsalValue: values,tied node
		template<typename... Params>
		Nodal_Value(Params ...aParams)
        : dummy<aParams...>,
		  m_NumOfAttr{},     // number of attributes
		  m_TieNode_No{},    // degree of liberty that is fix
		  m_nodeIdX{},       // identifier of the node (node number and coord.)
		  m_Node3Data{}
		{}

//		Nodal_Value( unsigned aId, int aNumAttr, int aTiedNode=-1);
		// Design Note: really need it??
		// copy initialization
//		Nodal_Value( const Nodal_Value& aOther);
		
		// assignment 
//		Nodal_Value & operator= ( const Nodal_Value & aOther);
		
		// force destructor to be virtual
    
//	    virtual ~Nodal_Value() = default;

		// node parameters
		nodeIdX&    getIdX()            { return m_nodeIdX;       }
		nodeIdX     getIdX()      const { return m_nodeIdX;       } 
		int32&      node_no()           { return m_nodeIdX.first; }
		const int32 node_no()     const { return m_nodeIdX.first; }
		int32&      tie_node_no()       { return m_TieNode_No;    }
		const int32 tie_node_no() const { return m_TieNode_No;    }
		int32&      no_of_dim()         { return m_NumOfAttr;     }
		const int32 no_of_dim()   const { return m_NumOfAttr;     }
		// check if we have a tied node
		bool isTiedNode()         const { return m_TieNode_No == -1;}

		// new version Usage: auto [A,Q,H] = Values();
		auto Values() const { return m_Node3Data;}

    // Design Note (N.Josuttis book "C++ Standard Library")
    //  Remove operator[] it doesn't make sense, typical usage 
    //  is to use std::tie to retrieve value from tuple
    //  std::tie(A,Q,H) = values(); for nodal variables
    //  std::tie(x,std::ignore,std:;ignote) = value(); grid node in 1D, one attribute
    //  need to declare variable first before the use of tie
    //  C++17 you can structured binding which is very similar
    //  auto[A,Q,H] = values(); don't need to declare variable 
#if 0
		// element accessor (write access)
		float64& operator[]( int i)       
		{
//			BOOST_ASSERT(i>=0&&i<3);

			if( m_NumOfAttr==3)
			{
        // just a test, make it more readable
        auto testEnumA = std::get<A>(m_Node3Data);
        auto testEnumQ = std::get<Q>(m_Node3Data);

				switch(i)
				{
				case 0: // A
					return std::get<A>(m_Node3Data);
					break;
				case 1: // Q
					return std::get<Q>(m_Node3Data);
					break;
				case 2: // H
					return std::get<H>(m_Node3Data);
					break;
				default: // return the first element as default
					return std::get<A>(m_Node3Data);
					break;
				}
			} 
			else
			{
				return m_nodeIdX.second; // x-coordinate
			}
		}
		// element accessor (read access)
		const float64& operator[] ( int i) const       
		{
//			BOOST_ASSERT(i>=0&&i<3);

			if( m_NumOfAttr==3)
			{
				switch(i)
				{
				case 0: // A
					return std::get<A>(m_Node3Data);
					break;
				case 1: // Q
					return std::get<Q>(m_Node3Data);
					break;
				case 2: // H
					return std::get<H>(m_Node3Data);
					break;
				default: // return the first element as default
					return std::get<A>(m_Node3Data);
					break;
				}
			} 
			else
			{
				return m_nodeIdX.second;
			}
		}
#endif

		// i don't get it??
		void Values( const tuplevar aTuple)      
		{
			m_Node3Data = aTuple;
		}

        // is that make sense? i do not think so!!!
        template<typename... Types>
		void setTupleValues(Types... aType)
		{
		//	std::make_from_tuple(aType);
			m_Node3Data = std::make_from_tuple(aType);
		}

		// Display node's information 
		// (used with for_each and mem_fun_ref)
    // Design Note
    //  Use variadic template or the Dsn utility
    //  file: "Dsn_PrintUtils.hpp"
    //
		void printNodeInfo()
		{
			// formating the output operator
			std::cout.setf(std::ios::fixed);
			std::cout.precision(4);
			std::cout.width(4);

      // print tuple utility
    //  using Dsn::operator<<;
     // std::cout << "Nodal Variables value: " << m_Node3Data << "\n";
      // ...
      std::cout << "Id " << m_nodeIdX.first << '\t'
                << "X: " << m_nodeIdX.second << "\n";

// 			if( m_NumOfAttr==3)
// 			{
// 				std::cout << "Id " << m_nodeIdX.first      << '\t'
// 					<< "A: " << std::get<A>(m_Node3Data) << '\t'
// 					<< "Q: " << std::get<Q>(m_Node3Data) << '\t'
// 					<< "H: " << std::get<H>(m_Node3Data) << '\t'
// 					<< "X: " << m_nodeIdX.second << "\n"; 
// 			} 
// 			else
// 			{
// 				std::cout << "Id " << m_nodeIdX.first   << '\t'
// 					<< "X: " << m_nodeIdX.second  <<  "\n";
// 			}
		}
	
		// check if it is implemented
		friend std::ostream& operator << ( std::ostream &, Nodal_Value &);

	protected:
	//	int32 m_NodeNumber;        node number
		int32 m_NumOfAttr;    // number of attributes
		int32 m_TieNode_No;   // degree of liberty that is fix
		nodeIdX m_nodeIdX;    // identifier of the node (node number and coord.)
		tuplevar m_Node3Data; // hold state variable value (A,Q,H)

        template<typename... Params>
		struct dummy {
			std::tuple<Params...> m_nodeData;
		};

        nodeDataType<double,double,double, signed> NodeVal;
		nodeDataType<unsigned,double,signed> GNode;
	};

	// a nodal type that hold coordinate of the node
    // donn't realyy need that, all ou have to do is
	//  GNode:   
	//	 Nodal_Value( std::make_pair(aId,aXcoord),aNumAttr=1, std::make_tuple(1.2,0.,0.)aTiedNode=0)
    //
	// Anyway  this don't make sense, 
	// class GNode : public Nodal_Value 
	// {
	// public:
	// 	GNode( int32 aId, float64 aXcoord, int32 aTiedNode=-1, unsigned aNumAttr=1)
	// 	: Nodal_Value( std::make_pair(aId,aXcoord)/*,aNumAttr,aTiedNode*/)
	// 	{
	// 		// nothing else to do 
	// 	}
	// 	// for now we check the x coordinate equality
	// 	bool operator== ( const GNode& aOther) { return getIdX().second==aOther.getIdX().second;}
	// 	bool operator!= ( const GNode& aOther) { return !(operator==(aOther));}
	// };

  // this is a temporary fix (just prototyping)
  class NodConstraint : public Nodal_Value
  {
  public:
    NodConstraint(int32 aId, float64 aXcoord, tuplevar aTpl, unsigned aNumAttr = 3, int aTiedNode = -1)
    : Nodal_Value(std::make_pair(aId, aXcoord), aNumAttr, aTpl, aTiedNode)
    {
      // nothing else to do 
    }

    // other ctor will do
  };
} // End of namespace