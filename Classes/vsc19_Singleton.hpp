

#pragma once

// C++ includes
#include <iostream>
#include <cstddef>

namespace vsc19
{ 
	/** Global instance of the application
	*/ 
	template<class T>
	class Singleton 
	{               
	  public:
		  // some methods 
		  static T* instance();
		  void release();
	  protected:

		  // We want to prevent of using them from client side
		  Singleton();
      Singleton( const Singleton&) = delete;
      Singleton& operator= ( const Singleton&) = delete;

		  // Shouldn't the destructor be virtual?
      virtual ~Singleton() = default;
	  private:
		  // The sole instance of the class
		  static T* m_singleton;
	};

	// initialize static variable 
	template<class T> T* Singleton<T>::m_singleton = nullptr;

	template<class T>
	Singleton<T>::Singleton()
	{}

	template<class T>
	T* Singleton<T>::instance()
	{
		if( m_singleton == nullptr) 
		{
			m_singleton = new T;
		}

		return m_singleton;
	}

	template<class T>
	void Singleton<T>::release()
	{
		if( m_singleton == nullptr)
			return;

		delete m_singleton;
		m_singleton = nullptr;
	}
}  // End of namespace