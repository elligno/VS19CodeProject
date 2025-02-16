#pragma once

// C++ includes
#include <iostream>
#include <tuple>

namespace vsc19
{
	// Concept
	// -------
	//   Nodal_Value type is a model of node variable, in a simulation
	//   there a set of variables associated with mesh node, and Nodal_Value
	//   represent those node variables.
	
	// nodal type that hold  variable value (see vs19_NodeVarTmpl.hpp)
	template<typename... TplPrms>
	class Nodal_Value  
	{		
  public:
    enum eVarType {A=0, Q=1, H=2};
	public:
		// typedefs
		using int32 = int;
		using float64 = double;
		//using nodeIdX = std::pair<int32,float64>;                  represent node Id and coordinate
		//using tuplevar = std::tuple<float64/*A*/, float64 /*Q*/, float64 /*H*/>;  represent (A,Q,H)
		template<typename... Params>
		using nodeDataType = std::tuple<Params...>;
	public:		
		// default ctor (i am not sure about this one!!)
		Nodal_Value()=default; // set attribute values?
		//Nodal_Value( nodeIdX aNodeXId, int aNumOfAttr=1, int aTieNode=-1);
	//	Nodal_Value( nodeIdX aNodeXId, int aNumAttr, tuplevar aTuple, int aTiedNode=-1);

        // This will contains all information about a node (id,x,values,tied node)
		// <unsigned, double, double,double,double, signed>
		// GNode: id,x, tied node 
		// NodsalValue: values,tied node
		//template<typename... Params>
		Nodal_Value(TplPrms ...aTplPrms)
        : m_tplData{aTplPrms...},
		  m_TieNode_No{}    // degree of liberty that is fix
		{}

		int32&      tie_node_no()       { return m_TieNode_No;    }
		const int32 tie_node_no() const { return m_TieNode_No;    }
		// check if we have a tied node
		bool isTiedNode()         const { return m_TieNode_No == -1;}

		// Display node's information 
		// (used with for_each and mem_fun_ref)
    // Design Note
    //  Use variadic template or the Dsn utility
    //  file: "Dsn_PrintUtils.hpp"
    //
		// void printNodeInfo()
		// {
		// 	// formating the output operator
		// 	std::cout.setf(std::ios::fixed);
		// 	std::cout.precision(4);
		// 	std::cout.width(4);

      // print tuple utility
    //  using Dsn::operator<<;
     // std::cout << "Nodal Variables value: " << m_Node3Data << "\n";
      // ...
    //   std::cout << "Id " << m_nodeIdX.first << '\t'
    //             << "X: " << m_nodeIdX.second << "\n";

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
//		}
	
		// check if it is implemented
//		friend std::ostream& operator << ( std::ostream &, Nodal_Value &);

	protected:
	//	int32 m_NodeNumber;        node number
	//	int32 m_NumOfAttr;    // number of attributes
		int32 m_TieNode_No;   // degree of liberty that is fix
	//	nodeIdX m_nodeIdX;    // identifier of the node (node number and coord.)
	//	tuplevar m_Node3Data; // hold state variable value (A,Q,H)

        template<typename... Params>
		struct dummy {
			std::tuple<Params...> m_nodeData;
		};
        
		dummy<TplPrms ...> m_tplData;
        
		nodeDataType<double,double,double, signed> NodeVal;
		nodeDataType<unsigned,double,signed> GNode;
	};
} // End of namespace