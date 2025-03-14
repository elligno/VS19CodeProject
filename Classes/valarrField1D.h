
#pragma once

// C++ includes
#include <memory> // smart pointer
#include <string>
#include <valarray>
#include <algorithm>
#include <functional>
// C++20  includes
#include <ranges>
#include <concepts>    
// App include
#include "gridLattice1D.h"

namespace vsc19
{
	/**
	 * @brief scalarField based on std numerical container for fast floating point operation.
	 *    Type that represent a function over a grid in a finite difference approximation.
	 *    std valarray support modern C++ move semantic as well math operation supported 
	 *    by such a function. 
	 */
    class scalarField1D final
    {
		public:
		    using valarraydbl = std::valarray<double>;

			/**
			 * @brief STL-container like interface (range)
			 */
			using value_type = std::valarray<double>::value_type;
			using pointer = value_type *;
			using iterator = pointer;
			using const_pointer = const value_type *;
			using const_iterator = const_pointer;
			using reference = value_type &;
			using const_reference = const value_type &;
			using range_diff_type = ptrdiff_t;

			// Replace all the lines above (next version or release)
			// SFX_DECLARE_CONTAINER( float64=aType)
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;

			/**
			 * @brief Iterator access
			 * @return iterator
			 */
			const_iterator cbegin() const;
			iterator begin();
			const_iterator cend() const;
			iterator end();
			const_reverse_iterator rbegin() const;
			reverse_iterator rbegin();
			const_reverse_iterator rend() const;
			reverse_iterator rend();

		public:
			/**
			 * @brief Construct a field lattice form a grid
			 *
			 * @param grid finite difference grid
			 * @param fieldname name description
			 * @return
			 */
			scalarField1D( const std::shared_ptr<gridLattice1D>& grid,
						std::string fieldname);
            /**
			 * @brief Construct a field lattice form a grid and initial values
			 *
			 * @param grid finite difference grid
			 * @param fieldname name description
			 * @param aInitValues field initial values 
			 * @return
			 */
			template<std::ranges::contiguous_range ContRng> // contiguous_range concept C++20
			scalarField1D( const std::shared_ptr<gridLattice1D>& aGrid, 
			               std::string aName, ContRng aInitValues)
		    : m_gridLattice{aGrid},
			  m_fieldName{std::move(aName)}
			{
				m_gridPointValues.reset(new std::valarray<double>(std::ranges::data(aInitValues), (*m_gridLattice).getDivisions()));
				// initialize the field with initial values
			//	std::copy( std::ranges::begin(aInitValues), std::ranges::end(aInitValues), std::ranges::begin(*m_gridPointValues));
				//std::ranges::copy(aInitValues, std::ranges::begin(*m_gridPointValues));
			}
			/**
		     * @brief Enable access to grid-point values
		     * 
		     * @return std::valarray<double>& 
		    */
		          valarraydbl& values()       { return *m_gridPointValues; }
		   // std::valarray<double> values() &&           { return std::move(*m_gridPointValues); }
			const valarraydbl& values() const { return *m_gridPointValues; }
			/**
			 * @brief Enable access to the grid
			 *
			 * @return gridLattice1D&
			 */
			      gridLattice1D &grid()       { return *m_gridLattice; }
			const gridLattice1D &grid() const { return *m_gridLattice; }
			/**
			 * @brief Field name description
			 *
			 * @return std::string
			 */
			std::string name() const { return m_fieldName; }
			/**
			 * @brief field maximum
			 *
			 * @return field max value
			 */
			float64 max() const { return m_gridPointValues->max(); }
			/**
			 * @brief field minimum
			 *
			 * @return field min value
			 */
			float64 min() const { return m_gridPointValues->min(); }
			/**
			 * @brief field sum
			 *
			 * @return sum of all value
			 */
			float64 sum() const { return m_gridPointValues->sum(); }
			/**
			 * @brief function to apply to the values(not implemented yet)
			 *   Template function that requires a RegularInvocable.
			 *
			 * @return Returns a new array of the same size with values
			 *   which are acquired by applying function 'func'.
			 */
            template <std::regular_invocable<float64> Func>
			valarraydbl apply(Func&& aCallable) const 
			{ 
				valarraydbl w_applyResult(m_gridPointValues->size());
				for( size_t i = 0; i < w_applyResult.size(); i++)
				{
				   w_applyResult[i] = aCallable((*m_gridPointValues)[i]);
				}
				return w_applyResult;
		    }
			/**
			 * @brief Set the values of the field
			 *
			 * @param aNewArray
			 */
			void values( valarraydbl &aNewArray);
			/**
			 * @brief Unary negate operator
			 *
			 * @return scalarField1D
			 */
			scalarField1D operator-() const;
			/**
			 * @brief Unary plus operator
			 *
			 * @return scalarField1D
			 */
			scalarField1D operator+() const;
			/**
			 * @brief Math operation on field (add 2 field)
			 *
			 * @param aOther
			 * @return vector field
			 */
			scalarField1D &operator+=(const scalarField1D &aOther);
			/**
			 * @brief
			 *
			 * @param aOther
			 * @return scalarField1D&
			 */
			scalarField1D &operator-=(const scalarField1D &aOther);
			/**
			 * @brief
			 *
			 * @param aOther scalar field to multiply
			 * @return scalarField1D&
			 */
			scalarField1D &operator*=(const scalarField1D &aOther);
			/**
			 * @brief
			 *
			 * @param aOther scalar field to be multiplied to
			 * @return scalarField1D
			 */
			scalarField1D &operator*=(double aDblVal);

			//	scalarField1D operator/= (const scalarField1D& aOther);

			/**
			 * @brief Conversion operator (support backward comppatibility with legacy code)
			 *
			 * @return scalarField1D vector representation of the scalar field
			 *          by-value since C++17 copy elison is mandatory
			 */
			explicit operator std::vector<double>() const noexcept
			{
				// return std::vector<double>(std::begin(values()),std::end(values()));
				return std::vector<double>(&(*m_gridPointValues)[0], &(*m_gridPointValues)[0] + (*m_gridPointValues).size());
		    }

            // template<typename Rng> // range
			// scalarField1D::scalarField1D( const std::shared_ptr<gridLattice1D>& aGrid, 
			//                std::string aName, Rng aInitValues)
		    // : m_gridLattice{aGrid},
			// m_fieldName{std::move(aName)}
			// {
			// 	m_gridPointValues.reset(new std::valarray<double>(initVal.data(), (*m_gridLattice).getDivisions()));
			// 	// initialize the field with initial values
			// 	std::copy(std::ranges::begin(aInitValues), std::ranges::end(aInitValues), std::begin(*m_gridPointValues));
			// }
         /**
         * @brief Services to expose field data as std vector (to be use by legacy code)
         * 
         * @return std vector  (by-value since C++17 copy elison is mandatory)
         */
		std::vector<double> asStdVector() const noexcept
		{
			return std::vector<double>(&(*m_gridPointValues)[0],&(*m_gridPointValues)[0]+(*m_gridPointValues).size());
	    }

		/**
		 * @brief Check emptyness
		 *
		 * @return true/false
		 */
		[[nodiscard]] bool isEmpty() noexcept { return (*m_gridPointValues).size() == 0; }

		/**
		 * @brief Field values extent
		 *
		 * @return size of the field
		 */
		size_t size() const { return (*m_gridPointValues).size(); }

	private:
        std::shared_ptr<gridLattice1D> m_gridLattice;     /**< shared grid*/
        std::shared_ptr<valarraydbl>   m_gridPointValues; /**< shared array (point values)*/
		std::string                    m_fieldName;       /**< field name*/
    };

    /** @brief Math operations on scalar field
	 * 
	*/
	scalarField1D operator+ (const scalarField1D& aF1, const scalarField1D& aF2);
	scalarField1D operator- (const scalarField1D& aF1, const scalarField1D& aF2);
	scalarField1D operator* (const scalarField1D& aF1, const scalarField1D& aF2);
 	scalarField1D operator+ (const scalarField1D& aF1, double aDbl);
	scalarField1D operator+ (double aDbl, const scalarField1D& aF1);
	scalarField1D operator- (const scalarField1D& aF1, double aDbl);
	scalarField1D operator- (double aDbl, const scalarField1D& aF1);
	scalarField1D operator* (const scalarField1D& aF1, double aDbl);
	scalarField1D operator* (double aDbl, const scalarField1D& aF1);
} // End of namespace

#if 0  // o = printValarray
	/**
     * @brief Printing element of container or a scalar 
     * @param rem ...
     * @param aVector type value to print
     * @param newline return end of line 
    */
   void printValarray( std::string_view rem, // cheap to copy
    const auto& aType2Print, bool newline = false)
  {
    // check if its a scalar by using type trait 
    if constexpr( std::is_scalar_v<std::decay_t<decltype(aType2Print)>>)
      std::cout << rem << " : " << aType2Print;
    else // otherwise its a container
    {
      for( std::cout << rem << " : { "; const auto vecElem : aType2Print)
        std::cout << vecElem << ' ';
      std::cout << '}';
    }
    std::cout << (newline ? "\n" : ";  ");
  }

  int main()
  {
    std::valarray<int> x, y;

    o("x", x = { 1, 2, 3, 4 }), o("y", y = { 4, 3, 2, 1 }), o("x += y", x += y, 1);
    o("x", x = { 4, 3, 2, 1 }), o("y", y = { 3, 2, 1, 0 }), o("x -= y", x -= y, 1);
    o("x", x = { 1, 2, 3, 4 }), o("y", y = { 5, 4, 3, 2 }), o("x *= y", x *= y, 1);
    o("x", x = { 1, 3, 4, 7 }), o("y", y = { 1, 1, 3, 5 }), o("x &= y", x &= y, 1);
    o("x", x = { 0, 1, 2, 4 }), o("y", y = { 4, 3, 2, 1 }), o("x <<=y", x <<= y, 1);

    std::cout << '\n';

    o("x", x = { 1, 2, 3, 4 }), o("x += 5", x += 5, 1);
    o("x", x = { 1, 2, 3, 4 }), o("x *= 2", x *= 2, 1);
    o("x", x = { 8, 6, 4, 2 }), o("x /= 2", x /= 2, 1);
    o("x", x = { 8, 4, 2, 1 }), o("x >>=1", x >>= 1, 1);
#endif
