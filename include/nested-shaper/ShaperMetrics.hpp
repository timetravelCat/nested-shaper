/**
 * @file ShaperMetrics.hpp
 * 
 * @brief This file contains the definition of the ShaperMetrics class.
 * 
 * @author timetravelCat <timetraveler930@gmail.com>
 */

#pragma once

#include "MovingMetrics.hpp"

namespace ns {
/**
 * @class ShaperMetrics
 * 
 * A template based moving metrics class.
 * ShaperMetrics is always initialized with full data, same as the given value.
 * 
 * MeanMetrics is a functor that calculates the new mean value.
 * MeanMetrics functor should have the following signature:
 * 
 * template<typename Type>
 *   struct MeanMetrics {
 *   Type operator()(const Type& mean, const Type& popped, const Type& pushed, QueueConstIterator<Type> forwardIterator, QueueConstIterator<Type> backwardIterator) const { return mean; }
 * };
 * 
 * DerivativeMetrics is a functor that calculates the new derivative value.
 * DerivativeMetrics functor should have the following signature:
 * 
 * template<typename Type, typename TimeType>
 * struct DerivativeMetrics {
 *  ResultType operator()(QueueConstIterator<Type> forwardIterator, QueueConstIterator<Type> backwardIterator, const TimeType& dt) const { return [as ResultType]; }
 * };
 */
template<typename Type, typename DerivativeMetrics, size_t Extent, typename MeanMetrics, size_t... Extents>
class ShaperMetrics : protected Queue<Type, Extent>, protected MovingMetricsNested<Type, MeanMetrics, Extents...> {
public:
    using value_type = Type;
    using reference = Type&;
    using const_reference = const Type&;
    using size_type = size_t;

    /**
     * Constructors
     */
    explicit ShaperMetrics(const Type& value);
    template<typename... Args>
    explicit ShaperMetrics(const Type& value, const Args&... capacities);

    /**
     * Status of the queue
     */
    using Queue<Type, Extent>::capacity;

    /**
     * (Re)Initializers
     */
    void initialize(const Type& value);
    template<typename... Args>
    void initialize(const Type& value, const Args&... capacities);

    /**
     * Convolute
     */
    template<typename TimeType>
    auto convolute(const Type& input, const TimeType& dt);

protected:
    DerivativeMetrics derivative_metrics{}; // DerivativeMetrics functor
    using Queue<Type, Extent>::fill;
};

template<typename Type, typename DerivativeMetrics, size_t Extent, typename MeanMetrics, size_t... Extents>
ShaperMetrics<Type, DerivativeMetrics, Extent, MeanMetrics, Extents...>::ShaperMetrics(const Type& value) :
Queue<Type, Extent>(), MovingMetricsNested<Type, MeanMetrics, Extents...>(value) {
    fill(value);
}

template<typename Type, typename DerivativeMetrics, size_t Extent, typename MeanMetrics, size_t... Extents>
template<typename... Args>
ShaperMetrics<Type, DerivativeMetrics, Extent, MeanMetrics, Extents...>::ShaperMetrics(const Type& value, const Args&... capacities) :
Queue<Type, Extent>(), MovingMetricsNested<Type, MeanMetrics, Extents...>(value, capacities...) {
    fill(value);
}

template<typename Type, typename DerivativeMetrics, size_t Extent, typename MeanMetrics, size_t... Extents>
void ShaperMetrics<Type, DerivativeMetrics, Extent, MeanMetrics, Extents...>::initialize(const Type& value) {
    derivative_metrics = DerivativeMetrics{};
    fill(value);
    MovingMetricsNested<Type, MeanMetrics, Extents...>::initialize(value);
}

template<typename Type, typename DerivativeMetrics, size_t Extent, typename MeanMetrics, size_t... Extents>
template<typename... Args>
void ShaperMetrics<Type, DerivativeMetrics, Extent, MeanMetrics, Extents...>::initialize(const Type& value, const Args&... capacities) {
    derivative_metrics = DerivativeMetrics{};
    fill(value);
    MovingMetricsNested<Type, MeanMetrics, Extents...>::initialize(value, capacities...);
}

template<typename Type, typename DerivativeMetrics, size_t Extent, typename MeanMetrics, size_t... Extents>
template<typename TimeType>
auto ShaperMetrics<Type, DerivativeMetrics, Extent, MeanMetrics, Extents...>::convolute(const Type& input, const TimeType& dt) {
    Queue<Type, Extent>::push(MovingMetricsNested<Type, MeanMetrics, Extents...>::convolute(input));
    return derivative_metrics.template operator()(Queue<Type, Extent>::forwardConstIterator(),
                                                  Queue<Type, Extent>::backwardConstIterator(),
                                                  dt);
}
}; // namespace ns