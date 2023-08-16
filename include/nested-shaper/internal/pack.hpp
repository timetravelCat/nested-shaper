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

        template <size_t Capacity, size_t... Capacities>
        struct pack_functor
        {
            template <typename pack>
            void reset(pack &_p)
            {
                _p.v.reset();
                pack_functor<Capacities...>{}.reset(_p.p);
            }

            template <typename pack, typename T>
            void fill(pack &_p, const T &input)
            {
                _p.v.fill(input);
                pack_functor<Capacities...>{}.fill(_p.p, input);
            }

            template <typename pack, typename Arg, typename... Args>
            void resize(pack &_p, Arg arg, Args... args)
            {
                _p.v.resize(arg);
                pack_functor<Capacities...>{}.resize(_p.p, args...);
            }

            template <typename pack, typename T>
            void update(pack &_p, const T &input)
            {
                _p.v.push(input);
                if (_p.v.full())
                    pack_functor<Capacities...>{}.update(_p.p, _p.v.get());
            }
        };

        template <size_t Capacity>
        struct pack_functor<Capacity>
        {
            template <typename pack>
            void reset(pack &_p)
            {
                _p.v.reset();
            }

            template <typename pack, typename T>
            void fill(pack &_p, const T &input)
            {
                _p.v.fill(input);
            }

            template <typename pack, typename Arg>
            void resize(pack &_p, Arg arg)
            {
                _p.v.resize(arg);
            }

            template <typename pack, typename T>
            void update(pack &_p, const T &input)
            {
                _p.v.push(input);
            }
        };
    } // namespace __ns__internal
} // namespace ns
