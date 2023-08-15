#pragma once

#include "../average_filter.hpp"

namespace ns
{
    namespace __ns__internal
    {
        template <typename T, bool Recursive, SummatorType summatorType, size_t Capacity, size_t... Capacities>
        struct average_filter_pack
        {
            average_filter<T, Capacity, Recursive, summatorType> v;
            average_filter_pack<T, Recursive, summatorType, Capacities...> p;
            filter<T> &operator[](const size_t &index) { return (index == 0) ? v : p[index - 1]; }
        };

        template <typename T, bool Recursive, SummatorType summatorType, size_t Capacity>
        struct average_filter_pack<T, Recursive, summatorType, Capacity>
        {
            average_filter<T, Capacity, Recursive, summatorType> v;
            filter<T> &operator[](const size_t &index) { return v; }
        };

        template <typename T, size_t index>
        struct getter
        {
            template <typename pack>
            filter<T> &get(pack &_p) { return getter<T, index - 1>{}.get(_p.p); }
        };

        template <typename T>
        struct getter<T, 0>
        {
            template <typename pack>
            filter<T> &get(pack &_p) { return _p.v; }
        };
    } // namespace __ns__internal
} // namespace ns
