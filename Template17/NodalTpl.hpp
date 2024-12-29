#pragma once

#include <tuple>

namespace vs11 
{
    // Experimenting an implementation for nodal variable concept 
    // used in our application by making use of variadic template.
    // Nodal values concept, a node can be represented by 
    // a set 2,3,4,... ndf (number degree of freedom)
    // the number of attributes is not constant.
    // I think variadic template is best suited to model this. 
    // Example of use:
    //  Nodaltpl<unsigned (id), double (x-coord)> 2-element tuple
    // which represent values at node: node no nd x-coordinate 
    template<typename... Ts>
    class NodalTpl 
    {
    public:
        // need ctor?? generated on will do!!!
        //???
        NodalTpl(bool isTied=false) : m_data<Ts...>{}, m_isTied{isTied} {}

        // can i do that? want to use it with structured binding
        auto getData() const { return m_data; }

        // does it make sense? tuple support this operator
        auto operator<=>( const Nodaltpl &aOther) = default;
        size_t size() const { return std::tuple_size_v<decltype(m_data)>; }
        int nodeNo() { return 1; } // default value
        bool isTiedNode() { return false; } // default value
    private:
        // Hold data at node of the global grid
        // For example, can be  variables such state
        // variables etc...
        std::tuple<Ts...> m_data;
        bool m_isTied;
    };
 }// End of namespace