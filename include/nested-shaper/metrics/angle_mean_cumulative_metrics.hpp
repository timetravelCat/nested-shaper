/**
 * @file angle_mean_cumulative_metrics.hpp
 * 
 * @brief This file contains the definition of the AngleMeanCumulativeMetrics class.
 */

#pragma once

#include <stddef.h>
#include <nested-shaper/Queue.hpp>
#include <nested-shaper/tiny_utility.hpp> // for ns::array
#define _USE_MATH_DEFINES
#include <math.h>

namespace ns {
// https://en.wikipedia.org/wiki/Circular_mean
template<typename Type>
struct AngleMeanCumulativeMetrics {
    Type operator()(const Type& mean,
                    const Type& popped,
                    const Type& pushed,
                    QueueConstIterator<Type> forwardIterator,
                    QueueConstIterator<Type> backwardIterator) const {
        (void)mean;
        (void)popped;
        (void)pushed;
        (void)backwardIterator;

        Type s{Type(0)};
        Type c{Type(0)};

        for(size_t i = 0; i < forwardIterator.size; ++i, ++forwardIterator) {
            s += ::sin(*forwardIterator);
            c += ::cos(*forwardIterator);
        }

        return ::atan2(s, c);
    }
};

template<typename Type, size_t N>
struct AngleMeanCumulativeMetricsArray {
    using array_type = ns::array<Type, N>;

    array_type operator()(
      const array_type& mean,
      const array_type& popped,
      const array_type& pushed,
      QueueConstIterator<array_type> forwardIterator,
      QueueConstIterator<array_type> backwardIterator) const {
        (void)mean;
        (void)popped;
        (void)pushed;
        (void)backwardIterator;

        array_type s{};
        array_type c{};
        for(size_t i = 0; i < N; i++) {
            s[i] = Type(0);
            c[i] = Type(0);
        }

        for(size_t i = 0; i < forwardIterator.size; ++i, ++forwardIterator) {
            for(size_t j = 0; j < N; ++j) {
                s[j] += ::sin((*forwardIterator)[j]);
                c[j] += ::cos((*forwardIterator)[j]);
            }
        }

        array_type means{};
        for(size_t i = 0; i < N; ++i) {
            means[i] = ::atan2(s[i], c[i]);
        }

        return means;
    }
};

}; // namespace ns