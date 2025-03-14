// C++ includes
#include <cassert>
#include <list>
//#include <compare> spaceship operator <=>
// Packages includes
//#include "../Classes/valarrField1D.h"
#include "../Classes/vsc19_RealNumArray.h"
#include "../Classes/vsc19_SectionFlow.h"
#include "../Algo/vs19_numScheme.hpp"
#include "../Algo/vs19_HLLAlgorithm.hpp"
#include "../Algo/vsc19_EmcilTreatmentAlgo.hpp"
// 
//#include "../vs19_PrintUtils.hpp"
#include "../vs19_UniversalConst.hpp"
#include "../Template17/vs19_Utilities.hpp"

#if 0
 //   Say for example that you have a custom container type in your codebase called SmallVector that, 
 //  like std::vector, can be initialized by passing it two iterators denoting a range. We can write 
 //  this with concepts from <iterator> and <concepts> as follows:
template <std::semiregular T>
struct SmallVector {
  template <std::input_iterator I>
    requires std::same_as<T, std::iter_value_t<I>>
  SmallVector( I i, std::Sentinel<I> auto s ) {
    // ...push back all elements in [i,s)
  }
};

// Likewise, this type can get a constructor that takes a range directly 
// using concepts defined in the new <ranges> header:
// ... as before
  template <std::InputRange R>
    requires std::Same<T, std::range_value_t<R>>
  explicit SmallVector( R && r )
  : SmallVector(std::ranges::begin(r),
                  std::ranges::end(r)) {
  }
};

      // "for_each" creates a new view by applying a
// transformation to each element in an input
// range, and flattening the resulting range of
// ranges.
// (This uses one syntax for constrained lambdas
// in C++20.)
inline constexpr auto for_each = [] <Range R, Iterator I = std::iterator_t<R>,
     std::indirectly_unary_invocable<I> Fun>(R&& r, Fun fun)
        requires std::range<std::indirect_result_t<Fun, I>> 
        {
      return std::forward<R>(r)
        | std::views::transform(std::move(fun))
        | std::views::join;
  };
  #endif


namespace vsc19 
{
    // ...
    void testTreatmentSrc2() 
    {
       namespace rng = std::ranges;
       
       // using namespace vsc19;
       std::list<SectionFlow> myListOfSectionFlow;
       myListOfSectionFlow.push_back(SectionFlow{1, 0., 1.});
       myListOfSectionFlow.push_back(SectionFlow{2, 0.1, 1.});
       myListOfSectionFlow.push_back(SectionFlow{3, 0.2, 1.});
       myListOfSectionFlow.push_back(SectionFlow{4, 0.3, 1.});
       myListOfSectionFlow.push_back(SectionFlow{5, 0.4, 1.});
 
       // use std invoke C++17 (check values)
       rng::for_each(myListOfSectionFlow, &SectionFlow::printSectionInfo);
     //  rng::sort( myListOfSectionFlow, rng::less{}, &SectionFlow::m_id);
 
       // create shared ptr grid
       auto grid1Dptr = std::make_shared<vsc19::gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
 
       // one-dimensional scalar field 
       scalarField1D w_scal1D{ grid1Dptr, std::string{"scalar field 1D"}};
       auto divs = w_scal1D.grid().getDivisions();
       auto w_valu = w_scal1D.values(); // zero initialization

       // sanity check
       static_assert( std::is_same_v<decltype(w_scal1D),scalarField1D> == true ); // value type
       assert(std::is_lvalue_reference_v<decltype((w_scal1D))>); // value category
       assert(rng::range<decltype(w_scal1D)>); // check range
       
       // since our field is a range, we can use the range library utility
       rng::fill( w_scal1D, 2.3); // initialize with values
       auto cbegVal = rng::begin(w_scal1D);
       auto cendVal = rng::end(w_scal1D);
       auto dst = cendVal-cbegVal;
       auto fieldDist = rng::distance(w_scal1D);
       assert(dst==fieldDist);

      // //std::fill(rng::begin(w_scal1D.values()), rng::end(w_scal1D.values()), 2.3);
      auto chk1 = w_scal1D.values();

       std::vector<double> w_contRng(w_scal1D.grid().getNoPoints()); // zero initialized
       //w_contRng.reserve(w_scal1D.grid().getNoPoints());
       rng::fill(w_contRng, 1.);
       // for our test just make a field from initial value (contiguous range concept)
       scalarField1D w_contRngScal1D{grid1Dptr, std::string{"scalar field 1D"}, w_contRng};
       auto cch = w_contRngScal1D.values();
       auto chk2 = w_contRngScal1D.asStdVector();

       // initialize with valarray
       std::valarray<double> w_vablarr(DIM::value);  // zero initialization
       scalarField1D w_ptRngScal1D{grid1Dptr, std::string{"scalar field 1D"}, w_vablarr};
       auto aaa = w_ptRngScal1D.asStdVector();

       // created a borrowed view (DIM -> metaprogramming constant)
       //auto ptrArr  = new double[DIM::value]{};
       RealNumArray<double> w_numArray(DIM::value, new double[DIM::value]{}); // in-house fast-floating point array
       if( rng::sized_range<decltype(w_numArray)> &&   // STL-like API 
           rng::contiguous_range<RealNumArray<double>>) 
       {
         scalarField1D w_numRngScal1D{grid1Dptr, std::string{"scalar field 1D"}, w_numArray};
         assert(rng::common_range<scalarField1D>);
         //delete [] ptrArr;
         auto numInit = w_numRngScal1D.values();       
        }
        else {
          std::cerr << "NumArray not a sized range\n";
        }
        // dynamic span (provide an interface to read/write elements stored in contiguous memory)
        // (span view is created (passing an lvalue as arg) to initialize the scalar field (cheap to copy/move)
        auto w_dynExtent = std::views::counted(rng::begin(w_numArray), rng::size(w_numArray));
        if( !rng::empty(w_dynExtent))
        {
          scalarField1D w_spnRngScal1D{grid1Dptr, std::string{"scalar field 1D"}, w_dynExtent};
          auto spnInit = w_spnRngScal1D.values();       
        }
      //  auto initSpn = rng::for_each( w_spn, [](double aVal) {return aVal = 1.2;}); 
 
       // C++17/20 features
       // subrange{vec} deduces the iterator and sentinel (use deduction guide)
       // template parameters from the range 'w_contRng', and since subrange is 
       // tuple-like, we can unpack the iterator/sentinel pair using structured bindings
       auto [begIter,endIter] = rng::subrange{w_contRng}; 
       
       // C++20 feature contiguous 
       assert(std::ranges::contiguous_range<scalarField1D>);
       auto [begIter1D,endIter1D] = rng::subrange{w_contRngScal1D};

       // define a subrange of our scalar field 
       auto [rngFieldBeg, rngFieldEnd] =  // iterator pair
          rng::subrange{rng::next(rng::begin(w_contRngScal1D)),  // begin range 
                        rng::prev(rng::end(w_contRngScal1D)),    // end range
                        rng::size(w_contRngScal1D)-2};            // number of elements
        
       auto sugRngDist = rngFieldEnd - rngFieldBeg;

      // auto begIter = rng::begin(w_contRng);
      // auto endIter = rng::end(w_contRng);
 
       ListSectionFlow w_testList;
       w_testList.add(SectionFlow{1, 0., 1.});
 
       SectionFlow w_sectionFlow1{1, 0., 1.};
       SectionFlow w_sectionFlow2{1, 0., 1.};
       // check equality operator
       //auto cmpSect = w_sectionFlow1 <=> w_sectionFlow2;
       // not sure about this one
       // auto strOrder = w_sectionFlow1<=>w_sectionFlow2;
       if (auto strOrder = w_sectionFlow1 <=> w_sectionFlow2; strOrder == 0) // is that make sense?? it sure is!!
       {
          std::cout << "These 2 section flow are equal and stronglyordered\n";
       }
    } 

  // some tests with range and other concept of modern C++
  void testHllSchemeImpl() 
  {
    namespace rng = std::ranges;

    // create shared ptr grid
    auto grid1Dptr = std::make_shared<gridLattice1D>(std::string{"d=1 [0,1] [0:99]"});
    // one-dimensional scalar field
    scalarField1D w_scal1D{grid1Dptr, std::string{"scalar field 1D"}};
    auto sizValues = w_scal1D.values().size();
    // auto &w_valu = w_scal1D.values();
    //   auto begVal = std::begin(w_scal1D.values());
    //   auto endVal = std::end(w_scal1D.values());
    // shall be a range
    if constexpr (rng::range<decltype(w_scal1D)>) // concept C++20
    {
      std::cout << "ScalarField is a range, use range algorithm\n";

      // initialize our field with step function
      rng::fill(w_scal1D, 1.);
      rng::fill_n(rng::next(rng::begin(w_scal1D),
                            w_scal1D.values().size() / 2),
                  w_scal1D.values().size() / 2, 0.5);

      // print shock location values (debugging purpose)
      auto w_shockView = w_scal1D.asStdVector() | std::views::drop(48) | std::views::take(4);
      std::cout << "Values at shock location are : " << w_shockView[0] << "  " << w_shockView[1]
                << "  " << w_shockView[2] << "  " << w_shockView[3] << "\n";

      // print utility (fold expression C++17)
  //    print17<'-'>(w_shockView[0],w_shockView[1],w_shockView[2],w_shockView[3]);
      printAddspace10(w_shockView[0],w_shockView[1],w_shockView[2],w_shockView[3]);

      // in-house fast-floating point array
      RealNumArray<double> w_numArray(DIM::value, new double[DIM::value]{}); 
      if( rng::sized_range<decltype(w_numArray)> &&   // STL-like API 
          rng::contiguous_range<RealNumArray<double>>) 
      {
        using val_type = std::ranges::range_value_t<scalarField1D>;
        scalarField1D w_numRngScal1D{grid1Dptr, std::string{"Discharge Q"}, w_numArray};
        //vsc19::MinMod19<val_type> w_minmodLimiter{}; Limiter slope function
        auto w_cellFacesFlux = // MUSCL reconstrution at second-order  
          Sfx::HLLFluxAlgorithmTest( w_scal1D.values(), w_numRngScal1D.values(), 
          MinMod19<val_type>{}); //Limiter slope function
        
        // let's check some values
      }

      // to modify the scalar field I need to use the 'all' adaptor/factory view
      // return a ref_view (reference semantic) when pass a lvalue. Below I used
      // 'counted' not a reference semantic it doesn't modify scalar Field
      auto testVwsField = std::views::all(w_scal1D) | // convert a range (lvalue) as a ref_view
                          std::views::transform([](double aVal)
                                                { return 2. * aVal; });

      auto w_midView = std::ranges::next(testVwsField.begin(), 50);
      for( auto pos = w_midView; pos != testVwsField.end(); ++pos)
      {
        std::cout << "Views values are :" << *pos << '\n';
      }

      //   auto viewTest = std::views::counted( std::ranges::next(std::ranges::begin(w_scal1D.values()),
      //   w_scal1D.values().size()/2),50) |
      //  std::views::transform([] (double_t aValue) {return aValue-0.5;});

      std::cout << "scalar field of size : " << w_scal1D.values().size() << '\n';
      std::cout << "scalar field vector of size : " << w_scal1D.asStdVector().size() << '\n';
    }
    else
    {
      std::cout << "ScalarField is not a range, use range algorithm\n";
      std::fill(std::begin(w_scal1D.values()), std::end(w_scal1D.values()), 2.3);
    }
    
    std::vector<double> w_fieldWithBc(w_scal1D.asStdVector()); // rvalue move ctor
    w_fieldWithBc.push_back(0.3);                              // physical b.c. (open boundary at right far end)
    // calling a version of the algorithm (here we pass an rvalue as the first arg)
    // we also use the metaprogramming constant C++17 feature (it call another algo)
    hllSchemeFlux(std::move(w_fieldWithBc),                  // is that make sense?? it sure is!!! xvalue
                  w_scal1D.grid().Delta(), cGravity<double>, // template variable
                  EMCNEILNbSections::value);                 // metaprogramming constant

   //   auto ret = vsc19::call( vsc19::hllSchemeFlux,
    //   std::move(w_fieldWithBc), // is that make sense?? it sure is!!! xvalue
    // w_scal1D.grid().Delta(), cGravity<double>, // template variable
    // EMCNEILNbSections::value);
 
    // just testing callable concept
  //  if( std::invocable<vsc19::hllSchemeFlux,std::vector<double>,float64,float64,int>)
  //  {
  //     std::invoke( std::forward<decltype(hllSchemeFlux)>(hllSchemeFlux), std::move(w_fieldWithBc),
  //     w_scal1D.grid().Delta(), cGravity<double>,EMCNEILNbSections::value);
  // //  }

    std::cout << "Finished testing our new C++20 algorithm\n";
  }
}