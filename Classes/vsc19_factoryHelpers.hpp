
#pragma once
// C++20 includes
#include <iostream>
#include <compare>  // comparing with <=>
// App include
#include "vsc19_Simulation.hpp"
#include "vsc19_PhysicsAlgo.h"

namespace {
    void MakeLogEntry() 
    {
        std::cout << "About to delete this pointed-to type\n";;
    }

    // create custom deleter (Scott Meyers book C++ effective)
    auto customDeleter = [] (auto ... ) 
    {
        MakeLogEntry();
    };
}

namespace vsc19 {
    /**
     * @brief Factory method based on perfect forwarding.
     * @tparam Args ctor argument type
     * @return compiler deduced type
     */
    template <typename... Args>
    auto PhysicsAlgoFactory(Args&& ...args) // let's compiler deduce type
    {
        std::unique_ptr<PhysicsAlgo> m_uniqptr{nullptr};

        // make code cleaner and easir to follow
        auto w_currentAlgoName = Simulation::instance()->getAlgorithmName();

        // create type from base one
        if (w_currentAlgoName <=> std::string{"NujicAlgo"} == std::strong_ordering::equal)
        {
            // perfect forwarding
            m_uniqptr.reset(new NujicAlgo(std::forward<Args>(args)...));
        }
        if (w_currentAlgoName <=> std::string{"EMcNeilAlgo"} == std::strong_ordering::equal)
        {
            // perfect forwarding
            m_uniqptr.reset(new EMCNeilAlgo(std::forward<Args>(args)...));
        }
        return m_uniqptr;
    }

    /** @brief Factory based on perfect-forwarding.
      *  Arguments can bind to any types (based on value categorie) 
      *  to the factory parameters. Forward reference preserves the 
      *  lvalue/rvalue-ness of the argument that is pass to the factory.
      * @tparam Args ctor argument type
      * @tparam T Type created
     *  @return Type smart pointer (shared resource)
     */  
     template <typename T, typename... Args>
     std::shared_ptr<T> factoryCreator( Args&& ...aArgs)
     {
         return std::shared_ptr<T>{ new T{ std::forward<Args>(aArgs)...}};
     }
} // End of namespace