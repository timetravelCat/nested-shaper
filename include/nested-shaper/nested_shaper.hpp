#pragma once

#include "nested_filter.hpp"

namespace ns
{
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    class nested_shaper
    {
        static_assert(Channel >= 1);

    public:
        nested_shaper();
        template <typename... Args>
        nested_shaper(Args... args);

        // Re maxsize all internal filters.
        template <typename... Args>
        void resize(Args... args);

        // Set empty for all filters
        void reset();

        // Just fill entire queue as input, and mark as full.
        void fill(const T input[Channel]);

        // Check all nested moving filter is ready.
        bool isReady() const;

        // Return true if all moving average filter is full
        bool update(const T input[Channel]);

        // Calculate average, derivatives of nested average queue.
        const T *peek(const T &dt);

    private:
        T derivatives[Channel + Depth - 1];
        nested_filter<T, Depth, Recursive, summatorType, Capacities...> nested_filters[Channel];
    };

    template <size_t Channel, size_t Depth, size_t... Capacities>
    using nested_shaperf = nested_shaper<Channel, float, Depth, true, SummatorType::KBN, Capacities...>;
    template <size_t Channel, size_t Depth, size_t... Capacities>
    using nested_shaperd = nested_shaper<Channel, double, Depth, true, SummatorType::KBN, Capacities...>;

    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::nested_shaper() {}
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    template <typename... Args>
    nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::nested_shaper(Args... args) { resize(args...); }
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    template <typename... Args>
    void nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::resize(Args... args)
    {
        for (size_t i = 0; i < Channel; i++)
            nested_filters[i].resize(args...);
    }
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    void nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::reset()
    {
        for (size_t i = 0; i < Channel; i++)
            nested_filters[i].reset();
    }
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    void nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::fill(const T input[Channel])
    {
        for (size_t i = 0; i < Channel; i++)
            nested_filters[i].fill(input[i]);
    }
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    bool nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::isReady() const
    {
        for (size_t i = 0; i < Channel; i++)
        {
            if (!nested_filters[i].isReady())
                return false;
        }

        return true;
    }
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    bool nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::update(const T input[Channel])
    {
        for (size_t i = 0; i < Channel; i++)
            nested_filters[i].update(input[i]);

        return isReady();
    }
    template <size_t Channel, typename T, size_t Depth, bool Recursive, SummatorType summatorType, size_t... Capacities>
    const T *nested_shaper<Channel, T, Depth, Recursive, summatorType, Capacities...>::peek(const T &dt)
    {
        // [0, Channel - 2]
        for (size_t i = 0; i < Channel - 1; i++)
            derivatives[i] = nested_filters[i].get();

        //  [Channel - 1, Channel + Depth - 1]
        const T *last_nested_filter_peek = nested_filters[Channel - 1].peek(dt);
        for (size_t i = 0; i < Depth; i++)
            derivatives[Channel + i - 1] = last_nested_filter_peek[i];

        return derivatives;
    }
} // namespace ns
