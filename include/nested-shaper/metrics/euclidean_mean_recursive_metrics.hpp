/**
 * @file euclidean_mean_recursive_metrics.hpp
 * 
 * @brief This file contains the definition of the EuclideanMeanRecursiveMetrics class.
 */

#pragma once

#include <stddef.h>
#include "kahan_accumulator.hpp"
#include <nested-shaper/Queue.hpp>
#include <nested-shaper/tiny_utility.hpp> // for ns::array

namespace ns {
template<typename Type>
struct EuclideanMeanRecursiveMetrics {
    Type operator()(const Type& mean,
                    const Type& popped,
                    const Type& pushed,
                    QueueConstIterator<Type> forwardIterator,
                    QueueConstIterator<Type> backwardIterator) {
        (void)forwardIterator;
        (void)backwardIterator;

        return accumulator.accumulate(mean, (pushed - popped) / Type(forwardIterator.size));
    }

private:
    KahanBabushkaNeumaierAccumulator<Type> accumulator{};
};

template<typename Type, size_t N>
struct EuclideanMeanRecursiveMetricsArray {
    using array_type = ns::array<Type, N>;

    array_type operator()(
      const array_type& mean,
      const array_type& popped,
      const array_type& pushed,
      QueueConstIterator<array_type> forwardIterator,
      QueueConstIterator<array_type> backwardIterator) {
        (void)forwardIterator;
        (void)backwardIterator;

        array_type sums{};
        for(size_t i = 0; i < N; ++i) {
            sums[i] = accumulator[i].accumulate(mean[i], (pushed[i] - popped[i]) / Type(forwardIterator.size));
        }

        return sums;
    }

private:
    KahanBabushkaNeumaierAccumulator<Type> accumulator[N]{};
};
} // namespace ns