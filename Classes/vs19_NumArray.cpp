
//////////////////////////////////////////////////////////////////////////
//
//  FILE: Sfx_NumArray.cpp
//
//    Author: Jean Belanger (Elligno Inc.)
//    Date of creation: January 25, 2007
//    Revision history:

#ifndef __numarray_cpp__
#define __numarray_cpp__

// description : implementation of class NumArray

// C++ include
#include <memory>

// Application include
#include "vs19_NumArray.hpp"

namespace vsc19 {

	// ____________________________________________________________
	//
	template< typename T >
	NumArray<T>::NumArray() 
	: A(nullptr)
	{}

	// ____________________________________________________________
	//
	template< typename T >
	NumArray<T>::NumArray(int32 n1)
	{
		A = allocate(n1);
	}

	// ____________________________________________________________
	//
	template< typename T >
	NumArray<T>::NumArray(int32 n1, int32 n2)
	{
		A = allocate(n1, n2);
	}

	// ____________________________________________________________
	//
	template< typename T >
	NumArray<T>::NumArray( const NumArray<T>& aOther)
	{
		if( aOther.dimensions == 2) {
			A = allocate( aOther.length[0], aOther.length[1]);

			for( int i = 0; i != (length[0] * length[1]); ++i)
				A[i] = aOther.A[i];
		}
		else if ( aOther.dimensions == 1) {
			A = allocate( aOther.length[0]);

			for( int i = 0; i != length[0]; ++i)
				A[i] = aOther.A[i];
		}
		else {
			std::cerr << "NumArray::NumArray(const NumArray<T>&) -- "
				<< "illegal dimension " << aOther.dimensions << std::endl;

			exit(1);
		}
	}

	// ____________________________________________________________
	//
	template< typename T >
	NumArray<T>::~NumArray()
	{
		deallocate();
	}

	// ____________________________________________________________
	//
	template< typename T >
	void NumArray<T>::redim(int32 n1)
	{
		if( length[0] == n1)
			return;

		deallocate();
		A = allocate(n1);
	}

	// ____________________________________________________________
	//
	template< typename T >
	void NumArray<T>::redim(int32 n1, int32 n2)
	{
		if( length[0] == n1 && length[1] == n2)
			return;

		deallocate();
		A = allocate(n1, n2);
	}

	// ____________________________________________________________
	//
	template< typename T >
	int NumArray<T>::size() const
	{ return length[0];}

	// ____________________________________________________________
	//
	template< typename T >
	int NumArray<T>::size(int32 dimension) const
	{
		return( dimension == 1) ? length[0] : length[1];
	}

	// ____________________________________________________________
	//
	template< typename T >
	T* NumArray<T>::allocate(int32 n1)
	{
		T* ptr = nullptr;
		length[0] = n1;
		dimensions = 1;

		if (n1 < 0) {
			std::cerr << "NumArray::allocate -- illegal length " << n1
				<< std::endl;

			exit(1);
		}
		else if (n1 == 0) {
			ptr = nullptr;
			return ptr;
		}

		try {
			ptr = new T[n1];
		} 
		catch( std::bad_alloc &) {
			std::cerr << "NumArray::allocate -- unable to allocate array of length "
				<< n1 << std::endl;
			exit(1);
		}    

		return ptr;
	}

	// ____________________________________________________________
	//
	template< typename T >
	T* NumArray<T>::allocate( int32 n1, int32 n2)
	{
		T* ptr = NULL;
		length[0] = n1;
		length[1] = n2;
		length0 = n1;       // special variable for efficient indexing
		length0p1 = n1+1;   // special variable for efficient indexing
		dimensions = 2;

		if( n1 < 0 || n2 < 0) {
			std::cerr << "NumArray::allocate -- illegal length " << n1
				<< ", " << n2 << std::endl;

			return NULL;
		}

		if (n1 == 0) {
			ptr = NULL;
			return ptr;
		}

		try {
			ptr = new T [n1*n2];
		}
		catch (std::bad_alloc&) {
			std::cerr << "NumArray::allocate -- unable to allocate array of length "
				<< length[0] << ", " << length[1] << std::endl;

			exit(1);
		}

		return ptr;
	}

	// ____________________________________________________________
	//
	template< typename T >
	void NumArray<T>::deallocate()
	{
		delete [] A;

		length[0] = 0;
		length[1] = 0;
	}

	// ____________________________________________________________
	//
	template< typename T >
	T* NumArray<T>::getPtr()
	{ return A;}

	// ____________________________________________________________
	//
	template< typename T >
	bool NumArray<T>::indexOk( int32 i) const
	{
		if( A == NULL) {
			std::cerr << "vector index check; current array is of length 0,"
				<< " call redim!" << std::endl;

			return false;
		}
		else if (i < 1 || i > length[0]) {
			std::cerr << "vector index check; index " << i 
				<< "out of bounds in array (1:" << length[0] << ")" << std::endl;

			return false;
		}
		else
			return true;  // valid index!
	}

	// ____________________________________________________________
	//
	template< typename T >
	bool NumArray<T>::indexOk(int32 i, int32 j) const
	{
		if (A == 0) {
			std::cerr << "vector index check; current array is of length 0,"
				<< " call redim!" << std::endl;

			return false;
		}
		else if (i < 1 || i > length[0]
		|| j < 1 || j > length[1]) {
			std::cerr << "vector index check; index " << i << "," << j
				<< "out of bounds in array (" << 1 << ":"
				<< 1 + length[0] << ")(" << 1 << ":"
				<< 1 + length[1] << std::endl;

			return false;
		}
		else
			return true;  // valid index!
	}

	// ____________________________________________________________
	//
	template< typename T >
	void NumArray<T>::print( std::ostream& os)
	{
		for( int32 i = 1; i != size(1); ++i) {
			for( int32 j = 1; j != size(2); ++j)
				os << (*this)(i, j) << " ";

			os << "\n";
		}
	}

	// ____________________________________________________________
	//
	template<typename T> 
	std::ostream & operator << ( std::ostream & os, NumArray<T> & arr)
	{
		arr.print(os);
		return os;
	}
}  // End of namespace
#endif  // Include guard
