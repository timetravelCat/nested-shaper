/**
 * @file euclidean_mean_cumulative_metrics.hpp
 * 
 * @brief This file contains the definition of the EuclideanDerivativeMetrics class.
 */

#pragma once

#include <stddef.h>
#include <nested-shaper/Queue.hpp>
#include <nested-shaper/tiny_utility.hpp> // for ns::array

namespace ns {
template<typename Type>
struct EuclideanMeanCumulativeMetrics {
    Type operator()(const Type& mean,
                    const Type& popped,
                    const Type& pushed,
                    QueueConstIterator<Type> forwardIterator,
                    QueueConstIterator<Type> backwardIterator) const {
        (void)mean;
        (void)popped;
        (void)pushed;
        (void)backwardIterator;

        Type sum{Type(0)};
        for(size_t i = 0; i < forwardIterator.size; ++i, ++forwardIterator) {
            sum += *forwardIterator;
        }

        return sum / static_cast<Type>(forwardIterator.size);
    }
};

template<typename Type, size_t N>
struct EuclideanMeanCumulativeMetricsArray {
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

        array_type sums;
        for(size_t i = 0; i < forwardIterator.size; ++i, ++forwardIterator) {
            for(size_t j = 0; j < N; ++j) {
                sums[j] += (*forwardIterator)[j];
            }
        }

        for(size_t j = 0; j < N; ++j) {
            sums[j] /= static_cast<Type>(forwardIterator.size);
        }

        return sums;
    }
};
}; // namespace ns