#pragma once

#include "nested_filter.hpp"

namespace ns
{
    // In case of jerk limited trajectory, input p, v, a, j is provided from algorithm.
    // Assume 2-step nested single position channel. (this result continuous jerk, snap results)
    // If you use single channel nested_avg_filter, the input p is the only thing that required to filter.
    // p1  p2  p3  p4  [p5] <= ouput of nested_avg_filter
    //   v1  v2  v3  v4
    //     a1  a2  a3
    //       j1  j2
    //         s1
    // We need 5 Depth of nested_avg_filter in order to get position to snap.
    // 5 depth has 2 step time delay compare to recent output of nested_avg_filter

    // Nested_avg_group class uses multi channels.
    // Compare to single channel, it uses all inputs, p, v, a, j
    // p1 [p2] <= output of position channel of nested_avg_filter
    // v1 [v2] <= output of velocity channel of nested_avg_filter
    // a1 [a2] <= output of acc channel of nested_avg_filter
    // j1 [j2] <= output of jerk channel of nested_avg_filter
    //   s1
    // We need 1 depth of nested_avg_filter in order to get position to snap.
    // 1 depth has 0.5 step time delay compare to recent output of nested_avg_filter
    // Nested_avg_group has advantage when partial component usage (such as using v,a,j only, p is not valid)
    // However, requires more buffer ( number of channles * queue size of single channel ),
    // And requires more computing performance due to all channel conduct moving average calculation.
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
        // [0, Channel - 1]
        for (size_t i = 0; i < Channel; i++)
            derivatives[i] = nested_filters[i].get();

        //  [Channel, Channel + Depth - 1]
        const T *last_nested_filter_peek = nested_filters[Channel - 1].peek(dt);
        for (size_t i = 1; i < Depth; i++)
            derivatives[Channel + i - 1] = last_nested_filter_peek[i];

        return derivatives;
    }
} // namespace ns
