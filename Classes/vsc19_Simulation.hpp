
#pragma once

// C++ includes
#include <string>
#include <optional>
#include <unordered_map>
// numeric include
#include <valarray>
// App include
#include "include/Singleton.hpp"

namespace vsc19 
{
    // Just a prototype of a class that manage the simulation 
	// parameters. These parameters could be read from an XML file 
	// for example or any other format.

	/// @brief Just a prototype of a class that manage the simulation
	/// parameters. These parameters could be read from an XML file
	/// for example or any other format.
	class Simulation : public Sfx::Singleton<Simulation> 
	{
		public:
		/// @brief aliases for code clarity
		using valarrdbl = std::valarray<double>;
		/// @brief Pointer-to-function alias (numerical flux algorithm)
		using CalculFF = void(*)(std::valarray<double>& aFF1,std::valarray<double>& aFF2, 
			 const std::valarray<double>& aU1, const std::valarray<double>& aU2) noexcept;
		/// @brief alias for mapping between function and name
		using fluxalgomap = std::unordered_map<std::string_view, CalculFF>;
	public:
	    /// @brief Make deleted to avoid client call
	    /// @param  
	    Simulation(const Simulation&)=delete;
		Simulation& operator= (const Simulation&)=delete;

		// Should I delete move ctor as well?

    	std::string getAlgorithmName() const { return m_algoName;}
		void setAlgorithmName( const std::string aName) { m_algoName=aName;}
     		
		/// @brief 
		/// @param aAlgoName 
		/// @return 
		CalculFF getFluxAlgorithm( std::string_view aAlgoName) const;
		/// @brief 
		/// @param aCaclFF 
		void setFluxAlgorithm(CalculFF aCaclFF) {m_fluxAlgo=aCaclFF;}

		/// @brief 
		/// @param aCalFlux 
		bool insert(CalculFF aCalFlux);
		
       // declare as friend of the class to access protected default ctor
	   friend class Singleton<Simulation>;
	protected:
       Simulation()=default; // access denied 
	private:
       std::string m_algoName;     /**< */ 
	   CalculFF m_fluxAlgo;        /**< */ 
	   fluxalgomap m_listPtr2Func; /**< */
	   //std::list<std::string> m_listOfAlgorithm;
    };
} // End of namespace