/**
 * @file angle_mean_recursive_metrics.hpp
 * 
 * @brief This file contains the definition of the AngleMeanRecursiveMetrics class.
 */

#pragma once

#include <stddef.h>
#include "kahan_accumulator.hpp"
#include <nested-shaper/Queue.hpp>
#include <nested-shaper/tiny_utility.hpp> // for ns::array

namespace ns {
template<typename Type>
struct AngleMeanRecursiveMetrics {
    Type operator()(const Type& mean,
                    const Type& popped,
                    const Type& pushed,
                    QueueConstIterator<Type> forwardIterator,
                    QueueConstIterator<Type> backwardIterator) {
        (void)forwardIterator;
        (void)backwardIterator;

        const Type size = Type(forwardIterator.size);
        return accumulator.accumulate(mean, wrap(pushed - popped, mean - Type(M_PI), mean + Type(M_PI)) / size);
    }

private:
    KahanBabushkaNeumaierAccumulator<Type> accumulator{};
};

template<typename Type, size_t N>
struct AngleMeanRecursiveMetricsArray {
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
        const Type size = Type(forwardIterator.size);

        for(size_t i = 0; i < N; ++i) {
            sums[i] = accumulators[i].accumulate(mean[i], wrap(pushed[i] - popped[i], mean[i] - Type(M_PI), mean[i] + Type(M_PI)) / size);
        }

        return sums;
    }

private:
    KahanBabushkaNeumaierAccumulator<Type> accumulators[N]{};
};
} // namespace ns