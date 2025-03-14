#pragma once

// C++ include
#include <tuple>

namespace vsc19
{
    // apply on a tuple
    template <typename... Ts>
    std::ostream &operator<<(std::ostream &os, std::tuple<Ts...> const &theTuple)
    {
        std::apply(
            [&os](const Ts &...tupleArgs)
            {
                os << '[';
                std::size_t n{0};
                ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
                os << ']';
            },
            theTuple);
        return os;
    }
    // Implementation for nodal variable concept
    // used in our application by making use of variadic template.
    // Nodal values concept, a node can be represented by
    // a set 2,3,4,... ndf (number degree of freedom)
    // the number of attributes is not constant.
    // I think variadic template is best suited to model this.
    // Example of use:
    //  Nodaltpl<unsigned (id), double (x-coord)> 2-element tuple
    // which represent values at node: node no nd x-coordinate
    template <typename... Params>
    struct NodalTpl
    {
        using value_type = std::tuple_element<1,m_tpl<Params...>>;
        bool m_tiedNode = false;     // tied node (half-open bondary)
        std::tuple<Params...> m_tpl; // pack expansion
        NodalTpl() = default;        // to be used in a collection (ctor size)
        // NodalTpl(Params... Prms) : m_tiedNode{false}, m_tpl(Prms...) {}
        NodalTpl(Params... Prms, bool tied = false) : m_tiedNode{tied}, m_tpl(Prms...) {}

        auto size() const { return std::tuple_size_v<decltype(m_tpl)>; }
        auto nodeData() const
        {
            if (isGNode())
                return std::get<1>(m_tpl);
            return std::make_tuple(std::get<1>(m_tpl), std::get<2>(m_tpl), std::get<3>(m_tpl));
        }
        auto nodeIdx() const { return std::get<0>(m_tpl); }

        // default both node at each end is tied
        auto tieNodeIdx() const
        {
            if (isTiedNode())
            {
                return std::get<0>(m_tpl);
            }
            else
            {
                return -1;
            }
        }
        /**  @brief
         *
         */
        auto isGhostNode() const
        {
            assert(std::get<0>(m_tpl) < vsc19::EMCNEILNbSections::value);
            return std::get<0>(m_tpl) == vsc19::DIM::value;
        }
        /**  @brief
         *   tuple implement '<=>' spaceship operator
         */
        bool isGNode() const { return (std::tuple_size_v<decltype(m_tpl)>) == 2; }

        /**  @brief
         *
         */
        bool isTiedNode() const
        {
            assert(std::get<0>(m_tpl) < vsc19::EMCNEILNbSections::value); // default prototype
            return m_tiedNode;
        }

        /**  @brief logical operator ==,!=, <,<=,>,>= data member
         *   tuple implement '<=>' spaceship operator
         */
        auto operator<=>(const NodalTpl &aOther) const = default;
    };
} // End of namespace