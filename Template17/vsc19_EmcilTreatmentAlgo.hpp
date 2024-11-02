#pragma once
// STL numeric includes
#include <cmath>
#include <valarray>
// C++20 include
#include <ranges>
// Boost numeric include
#include <boost/numeric/ublas/vector_expression.hpp>
// App includes
#include "../Classes/valarrField1D.h"
#include "../Classes/vsc19_ListSectionFlow.h"

namespace vsc19 
{
    namespace ublas = boost::numeric::ublas;
    using float64 = double;

	/**
	 * @brief Function object (callable) that perform calculation 
	 *   cross-section ( wetted area, hydraulic radius, width at free surface.
	*/
	struct WettedPerimeter {  

		constexpr float64 operator () ( const float64 aArea, float64 aWidth = 1.) const
		{
			//	Fonction spécifique à un canal rectangulaire de largeur B
			//	Périmètre établi en fonction d'une superficie d'écoulement A

			//	float64 Resultat;

			//	Resultat = 2.*A/B + B;

			//	return (Resultat);

			return (2.*aArea)/aWidth + aWidth;
		}
	};

	// ____________________________________________________
	//
	// struct HydRadius : public std::binary_function<float64,float64,float64>
	// result_type operator() ( const first_argument_type aArea, 
	//                          const second_argument_type aWidth) const
	//  use of std::bind2nd to set width = 1.
	
	
	/**
	 * @brief Function specific to a rectangular channel of width B
	 *		    Perimeter established according to a flow area A
	*/
	struct HydRadius {   

		constexpr float64 operator () ( const float64 aArea, float64 aWidth = 1.) const
		{
			//	Fonction spécifique à un canal rectangulaire de largeur B
			//	Périmètre établi en fonction d'une superficie d'écoulement A

			//	float64 Resultat;

			//	Resultat = A/P(A, B);

			//	return (Resultat);
			WettedPerimeter wWP;

			return (aArea/wWP( aArea));
		}
	};

    /** @brief Expression template fast floating point container
     * 
    */
    template<std::ranges::input_range Rng>
    class NumericalVec : 
    public ublas::vector_expression<Rng> 
    {
       // using Rng::begin, Rng::end;
        public:
        using value_type = std::vector<double>::value_type;
			using pointer = value_type *;
			using iterator = pointer;
			using const_pointer = const value_type *;

        public:

        NumericalVec() = default;
        bool isRange() const {return true;}

        private:
        //not sure about that
        std::vector<double> m_data;
     };

    // N^2*Q*abs(Q)/A*A*R^4/3 where R hydraulic radius
    std::valarray<double> TraitemnentSource2( const scalarField1D& Q,
    const scalarField1D& A, const ListSectionFlow& aListOfSections /*const Gamma& aBC*/)
    {
        using namespace std;

        if(!aListOfSections.isFrictionLess()) return std::valarray<double>();

        // retrieve filed values
        const auto& w_Q = Q.values();
        const auto& w_A = A.values();

        // Hydraulic radius (lambda)
        auto pow43 = std::pow(w_A.apply([] (double aWettedArea) 
        {
            WettedPerimeter w_wetP; // compute section wet perimeter (unit width) 
            return aWettedArea/w_wetP(aWettedArea);
        }),4/3);

        // frictionless (Manning coeff. = 0.) => N2=0.       
        auto N2 = std::pow(valarray<double>(A.values().size()),2);


        // Energy slope term 
        auto Sf = (N2*w_Q*w_Q.apply(std::abs))/(w_A*w_A*pow43); 
        
        // Based on Nujic assumption (1995)
        // compute derivative (need list of sections flow) 
        // std::vector<double> w_H; w_H.reserve(gblDiscr->Uh().size());
        // std::transform(aListOfSections.cbegin(),aListOfSections.cend(),
        //  std::back_inserter(w_Hvec)std::bind());
        // create scalarField1D from std vector w_H
         // scalar field over a finite difference grid
         // shall consider using the following
         //IMPORTANT w_dbData.toString(Simulation::instance()->getDBdata());
        // auto w_Hfield = new scalarField1D{ std::string("H-field"), // use E McNeil discretization data
        //  std::make_shared<Sfx::gridLattice>( std::string("d=1 [1,1000] [0:100]")),w_Hvec};
        // retrieve b.c. downstream (ghost node)
        // std::tie(ignore,ignore,Hdowntream) = gamma().getDownstream().values();
        // compute derivative by central second-order scheme (Taylor expansion)
        // FiniteDifferenceDerivative w_dxH2nd{ std::string{ "H-Field" }, false }; // not using ENO stencil
        // w_dxH2nd.setDerivativeOrder(2);  // central scheme 2nd order ()
        // auto w_dxHvarray =fd.d1x(Hfield,ghost node values)
        // compute average A
        // 
        // product avgA*d1xH
        // return Sf-S0
        return std::valarray<double>(); // debugging purpose 
    }

    /** @brief  Prototyping algo with template and C++20 range library
      * using C++14 keyword decltype(auto) to deduce return type
      * according to value categorie 
    */
    template<std::ranges::input_range Rng, auto NbSections=101>
    decltype(auto) TraitemnentSource2( const Rng& Q,  // could be a scalar field
     const Rng& A/*, double dx*/)  // or std valarray<double>
    {
       decltype(NbSections) nbSections = 101; //just testing  
       // first need to check if it is a range

       return std::valarray<double>(); // temp fix for debugging purpose
    }  
} // End of namespace4