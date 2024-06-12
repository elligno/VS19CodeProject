
// C++20 include
#include <ranges>

// App includes
//#include "gridLattice1D.h"

//#include "E:\JeanProjects\TestVS2019\Classes\gridLattice1D.h"
//#include "Classes/gridLattice1D.h"
//#include "Classes/valarrField1D.h"
#include "Classes/vs19_GridLattice.h"
#include "Classes/vs19_valArrField.h"

// includes 
namespace vsc19 
{
  // Testing new implementation of numerical array 
	void testvalArrField()
	{
    //d=1 [0,1]x[0,10] NO!!NO!! number of grid node can't be from 0 to N
  // must be set to a valid range [1,N]
 //   gridLattice1D w_grid1D{ std::string {"d=1 [0,1]x[1,10]"} };
   // auto dx = w_grid1D.Delta();

    const auto w_grid = std::make_shared<GridLattice>(10, 0., 1.);
    valArrField w_scalarField1D{ w_grid, std::string{"scalarField"} };
    auto& w_gridPointVal = w_scalarField1D.values();
    w_gridPointVal[0] = 1.;
    w_gridPointVal[1] = 1.3;
    w_gridPointVal[2] = 14.3;
    w_gridPointVal[3] = 1.2;
    w_gridPointVal[4] = 0.4;
    w_gridPointVal[5] = 7.3;
    w_gridPointVal[6] = 1.8;
    w_gridPointVal[7] = 6.2;
    w_gridPointVal[8] = 8.6;
    w_gridPointVal[9] = 0.4;

    std::valarray<double> w_testValCtor(5); // set initial values to zero
    auto checkSiz = w_testValCtor.size();
    auto firstVal = w_testValCtor[0];
    auto first = w_testValCtor[1];
    auto st = w_testValCtor[2];
    std::valarray<double> w_testInitValCtor(1., 5); // set initial values to zero

    std::valarray<double> w_mvCpy = std::move(w_testValCtor);
    auto siz = w_testValCtor.size(); // shall be zero
    //w_testValCtor[0] = 2.5; // still assignable??
    printValarray("Field Value", w_gridPointVal, 1);
    w_scalarField1D += 2;
    printValarray("Field Value", w_gridPointVal);

    // ...
    w_scalarField1D += w_scalarField1D;
    valArrField w_tstField = valArrField{ w_grid, std::string{"scalarField"} } + w_scalarField1D;
    printValarray( "Field Value", w_tstField.values());
    valArrField w_mvField = std::move(w_tstField) + 2.3;
    printValarray( "Field Value", w_tstField.values(), 1);
	}

  void testRngField()
  {
//    gridLattice1D w_grid1D{ std::string {"d=1 [0,1]x[1,10]"} };
//    auto dx = w_grid1D.Delta();

    const auto w_grid = std::make_shared<GridLattice>(10, 0., 1.);
    valArrField w_scalarField1D{  w_grid, std::string{"scalarField"} };
    auto& w_gridPointVal = w_scalarField1D.values();
    w_gridPointVal[0] = 1.;
    w_gridPointVal[1] = 1.3;
    w_gridPointVal[2] = 14.3;
    w_gridPointVal[3] = 1.2;
    w_gridPointVal[4] = 0.4;
    w_gridPointVal[5] = 7.3;
    w_gridPointVal[6] = 1.8;
    w_gridPointVal[7] = 6.2;
    w_gridPointVal[8] = 8.6;
    w_gridPointVal[9] = 0.4;

    // aliases for clarity
    using range_diff_type = std::ranges::range_difference_t<valArrField>;   //distance
    using value_type = std::ranges::range_value_t<valArrField>;      //type
    // boost similar range
    const auto fieldSiz = std::ranges::distance( w_scalarField1D);
  }

}// End of namespace