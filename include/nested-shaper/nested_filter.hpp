#pragma once

#include "average_filter.hpp"
#include "internal/derivative.hpp"
#include "internal/pack.hpp"

namespace ns
{
    // A nested moving average filter
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    class nested_filter : protected average_filter<T, Depth, Recursive, summatorType>
    {
        static_assert(Depth >= 1);

    public:
        nested_filter();
        template <typename... Args>
        nested_filter(Args... args);

        // Re maxsize all internal filters.
        template <typename... Args>
        void resize(Args... args);

        // Set empty for all filters
        void reset();

        // Just fill entire queue as input, and mark as full.
        void fill(const T &input);

        // Check all nested moving filter is ready.
        bool isReady() const;

        // Return true if all moving average filter is full
        bool update(const T &input);

        // Calculate average, derivatives of nested average queue.
        const T *peek(const T &dt);

    private:
        T derivatives[Depth];
        __ns__internal::average_filter_pack<T, Recursive, summatorType, Capacities...> _average_filters;

        template <size_t Ordinal>
        friend class __ns__internal::forward_derivative;
        template <size_t Ordinal>
        friend class __ns__internal::backward_derivative;
        template <size_t A, size_t B>
        friend class __ns__internal::recursive_derivative;
    };

    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    nested_filter<T, Depth, Recursive, summatorType, Capacities...>::nested_filter() : average_filter<T, Depth, Recursive, summatorType>() {}
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    template <typename... Args>
    nested_filter<T, Depth, Recursive, summatorType, Capacities...>::nested_filter(Args... args) { resize(args...); }
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    template <typename... Args>
    void nested_filter<T, Depth, Recursive, summatorType, Capacities...>::resize(Args... args)
    {
        static_assert(sizeof...(args) == sizeof...(Capacities));
        __ns__internal::pack_functor<Capacities...>{}.resize(_average_filters, args...);
    }
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    void nested_filter<T, Depth, Recursive, summatorType, Capacities...>::reset()
    {
        average_filter<T, Depth, Recursive, summatorType>::reset();
        __ns__internal::pack_functor<Capacities...>{}.reset(_average_filters);
    }
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    void nested_filter<T, Depth, Recursive, summatorType, Capacities...>::fill(const T &input)
    {
        average_filter<T, Depth, Recursive, summatorType>::fill(input);
        __ns__internal::pack_functor<Capacities...>{}.fill(_average_filters, input);
    }
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    bool nested_filter<T, Depth, Recursive, summatorType, Capacities...>::isReady() const
    {
        return average_filter<T, Depth, Recursive, summatorType>::full();
    }
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    bool nested_filter<T, Depth, Recursive, summatorType, Capacities...>::update(const T &input)
    {
        __ns__internal::pack_functor<Capacities...>{}.update(_average_filters, input);
        auto &_final_filter = ns::__ns__internal::getter<T, sizeof...(Capacities) - 1>{}.get(_average_filters);
        if (_final_filter.full())
            average_filter<T, Depth, Recursive, summatorType>::push(_final_filter.get());
        return isReady();
    }
    template <typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    const T *nested_filter<T, Depth, Recursive, summatorType, Capacities...>::peek(const T &dt)
    {
        __ns__internal::recursive_derivative<Depth, Depth>{}.derivative(*this, derivatives, dt);
        return derivatives;
    }
} // namespace ns
