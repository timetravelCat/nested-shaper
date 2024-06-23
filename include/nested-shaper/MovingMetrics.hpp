/**
 * @file MovingMetrics.hpp
 * 
 * @brief This file contains the definition of the MovingMetrics class.
 * 
 * @author timetravelCat <timetraveler930@gmail.com>
 */

#pragma once

#include "Queue.hpp"

namespace ns {
/**
 * @class MovingMetrics
 * 
 * A template based moving metrics class.
 * MovingMetrics is always initialized with full data, same as the given value.
 * 
 * Metrics is a functor that calculates the new mean value.
 * Metrics functor should have the following signature:
 * 
 * template<typename Type>
 *   struct EmptyMetrics {
 *   template<typename Iterator>
 *   Type operator()(const Type& mean, const Type& popped, const Type& pushed, Iterator begin, Iterator end, const size_t& size) const { return mean; }
 * };
 */
template<typename Type, size_t Extent, typename Metrics>
class MovingMetrics : protected Queue<Type, Extent> {
public:
    using value_type = Type;
    using reference = Type&;
    using const_reference = const Type&;
    using size_type = size_t;

    /**
     * Constructors
     */
    explicit MovingMetrics(const Type& value) :
    Queue<Type, Extent>() { initialize(value); };
    explicit MovingMetrics(const Type& value, const size_type& capacity_) :
    Queue<Type, Extent>(capacity_) { initialize(value, capacity_); };

    /**
     * Status of the queue
     */
    using Queue<Type, Extent>::capacity;

    /**
     * (Re)Initializers
     * 
     * initialize : fill the queue with given value.
     */
    void initialize(const Type& value);
    void initialize(const Type& value, const size_type& capacity_);

    /**
     * Convolute
     * 
     * given value is convoluted with the metrics.
     * returns the convoluted value.
     */
    Type convolute(const Type& value, const Metrics& metrics = Metrics{});

protected:
    Type mean{}; // Mean(average) value

    using Queue<Type, Extent>::_data;
    using Queue<Type, Extent>::_capacity;
    using Queue<Type, Extent>::_size;
    using Queue<Type, Extent>::_back;
    using Queue<Type, Extent>::_front;

    /**
     * Push the value to the queue and returns popped data.
     * Does not check about queue size.
     */
    Type push(const Type& value);
};

template<typename Type, size_t Extent, typename Metrics>
void MovingMetrics<Type, Extent, Metrics>::initialize(const Type& value) {
    mean = value;
    _size = _capacity;
    _back = _data;
    _front = _data + _capacity - 1;

    for(size_t i = 0; i < _capacity; i++) {
        _data[i] = mean;
    }
};

template<typename Type, size_t Extent, typename Metrics>
void MovingMetrics<Type, Extent, Metrics>::initialize(const Type& value, const size_type& capacity_) {
    Queue<Type, Extent>::resize(capacity_);
    initialize(value);
};

template<typename Type, size_t Extent, typename Metrics>
Type MovingMetrics<Type, Extent, Metrics>::convolute(const Type& value, const Metrics& metrics) {
    mean = metrics.template
           operator()(mean,
                      push(value),
                      value,
                      Queue<Type, Extent>::cbegin(),
                      Queue<Type, Extent>::cend(),
                      _size);
    return mean;
};

template<typename Type, size_t Extent, typename Metrics>
Type MovingMetrics<Type, Extent, Metrics>::push(const Type& value) {
    const Type result = *_back;
    _back == _data + _capacity - 1 ? _back = _data : _back++;
    _front == _data + _capacity - 1 ? _front = _data : _front++;
    *_front = value;
    return result;
};

// Helper class for MovingMetrics
template<typename Type, typename Metrics, size_t Extent, size_t... Extents>
struct MovingMetricsNested {
    explicit MovingMetricsNested(const Type& value) :
    moving_metrics(value), moving_metrics_nested(value) {}
    template<typename Arg, typename... Args>
    explicit MovingMetricsNested(const Type& value, const Arg& capacity, const Args&... capacities) :
    moving_metrics(value, capacity), moving_metrics_nested(value, capacities...) {
        static_assert(sizeof...(capacities) == sizeof...(Extents), "Number of capacities must be equal to number of extents.");
    }

    inline void initialize(const Type& value) {
        moving_metrics.initialize(value);
        moving_metrics_nested.initialize(value);
    }

    template<typename... Args>
    inline void initialize(const Type& value, const size_t& capacity, const Args&... capacities) {
        static_assert(sizeof...(capacities) == sizeof...(Extents), "Number of capacities must be equal to number of extents.");
        moving_metrics.initialize(value, capacity);
        moving_metrics_nested.initialize(value, capacities...);
    }

    inline Type convolute(const Type& value, const Metrics& metrics = Metrics{}) {
        return moving_metrics_nested.convolute(moving_metrics.convolute(value, metrics));
    }

    MovingMetrics<Type, Extent, Metrics> moving_metrics;
    MovingMetricsNested<Type, Metrics, Extents...> moving_metrics_nested;
};

template<typename Type, typename Metrics, size_t Extent>
struct MovingMetricsNested<Type, Metrics, Extent> {
    explicit MovingMetricsNested(const Type& value) :
    moving_metrics(value) {}
    template<typename Arg>
    explicit MovingMetricsNested(const Type& value, const Arg& capacity) :
    moving_metrics(value, capacity) {}

    inline void initialize(const Type& value) {
        moving_metrics.initialize(value);
    };

    inline void initialize(const Type& value, const size_t& capacity) {
        moving_metrics.initialize(value, capacity);
    };

    inline Type convolute(const Type& value, const Metrics& metrics = Metrics{}) {
        return moving_metrics.convolute(value, metrics);
    };

    MovingMetrics<Type, Extent, Metrics> moving_metrics;
};
}; // namespace ns