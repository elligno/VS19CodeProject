
#pragma once

// C++ include
#include <string>
// SfxBase19 include
#include "include/Singleton.hpp"

namespace vsc19 
{
    // Just a prototype of a class that manage the simulation 
	// parameters. These parameters could be read from an XML file 
	// for example or any other format.
	class Simulation : public Sfx::Singleton<Simulation> 
	{
	public:
    	std::string getAlgorithmName() const { return m_algoName;    }
		void setAlgorithmName( const std::string aName) { m_algoName=aName;}
     		
       // declare as friend of the class to access protected default ctor
	   friend class Singleton<Simulation>;
	protected:
       Simulation()=default; // access denied 
	private:
       std::string m_algoName;
	   std::list<std::string> m_listOfAlgorithm;
    };
} // End of namespace
