
#if !defined(__numarrayh__)
#define __numarrayh__

// C++ include
#include <iostream>
// STL includes
#include <vector>
#include <valarray>

namespace vsc19 {

	using int32 = int;
	using float64 = double;
	//template<typename T, typename CONT = std::valarray<T> >  // default container

	/**
	 * @brief The class NumArray is a templated 1D or 2D array, specialized 
	 *   for containing numerical data.
	 * @tparam T 
	*/
	template<typename T>
	class NumArray {

	public:

		//
		// STL like container (Added by Jean Belanger Jan.26, 2007) 
		//

		// some typedef's
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator = pointer;
		using const_iterator = const pointer;

		iterator begin()             { return A;         }
		iterator end()               { return A + size();}
		const_iterator begin() const { return A;         }
		const_iterator end() const   { return A + size();}

		//using iter_type = boost::range_iterator<T*>::type;

	public:

		// default ctor (what does it means?)
		NumArray();

		// constructor for 1D array
		explicit NumArray( int32 n1);           

		// constructor for 2D array
		NumArray( int32 n1, int32 n2);   

		// should we have a ctor of this type?
		NumArray( int32 n1, int32 n2, float64* aArray);

		// copy constructor
		NumArray( const NumArray<T>& aArray);  

		// desctructor
		~NumArray();                        

		void redim( int32 n1);                // redim 1D array
		void redim( int32 n1, int32 n2);      // redim 2D array

		// return the size of the arrays dimensions
		int32 size() const;                   // length of 1D array
		int32 size( int32 dimension) const;

		bool indexOk( int32 i) const;         // check if index is ok to use
		bool indexOk( int32 i, int j) const;  // check if indices are ok to use

		// operator() for 1D array
		const T& operator()( int32 i) const;
		      T& operator()( int32 i);

		// operator() for 2D array
		const T& operator()( int32 i, int32 j) const;
		      T& operator()( int32 i, int32 j);

		// assignment operator (not implemented yet)
		NumArray& operator = ( const NumArray & aOther);  

		// returns pointers to the data
		const T* getPtr() const;
		      T* getPtr();

        /** Some utilities for backward compatibility*/
	    std::vector<double> toStdVector() const     { return std::vector<double>(A,A+size());}
		std::valarray<double> toStdValarray() const {return std::valarray<double>(A,size());}

         // why don't we overload operator << ?
		//std::iostream operator << ( std::iostream & aIOstream, const NumArray & aArray);
		void print( std::ostream & os);

	private:

		T* A;                                   // the data
		int32 dimensions;                       // number of dimensions (max is 2)
		int32 length[2];                        // lengths of each dimension

		// needed for optimizations:
		int32 length0;
		int32 length0p1;

		// these three functions are used by constructors, destructors and redim:
		T* allocate( int32 length1);
		T* allocate( int32 length1, int32 length2);
		void deallocate();

		// transform fake indices to real:
		int transform_index( int32 i, int32 j) const;
	};

	// =========================================
	//           Inline definitions
	// =========================================

	// _______________________________________________
	//
	template<typename T>
	inline int NumArray<T>::transform_index( int32 i, int32 j) const
	{ return j + i*length0 - length0p1;}

	// _______________________________________________
	//
	template< typename T >
	inline T& NumArray<T>:: operator()( int32 i)
	{
#ifdef SAFETY_CHECKS
		indexOk(i);
#endif

		return  A[i - 1];
	}

	// _______________________________________________
	//
	template< typename T >
	inline const T& NumArray<T>:: operator()( int32 i) const
	{
#ifdef SAFETY_CHECKS
		indexOk(i);
#endif

		return  A[i - 1];
	}

	// _______________________________________________
	//
	template< typename T >
	inline T& NumArray<T>:: operator()( int32 i, int32 j)
	{
#ifdef SAFETY_CHECKS
		indexOk(i, j);
#endif

#ifdef NO_NESTED_INLINES
		return A[j + i*length0 - length0p1];
#else
		return A[transform_index(i, j)];
#endif
	}

	// _______________________________________________
	//
	template< typename T >
	inline const T& NumArray<T>:: operator()( int32 i, int32 j) const
	{
#ifdef SAFETY_CHECKS
		indexOk(i, j);
#endif

#ifdef NO_NESTED_INLINES
		return A[j + i*length0 - length0p1];
#else
		return  A[transform_index(i, j)];
#endif
	}
}  // End of namespace

// Since this is a template class, we must make the body of the
// functions available in this header file:
#include "vs19_NumArray.cpp"

#endif  // Include guard
