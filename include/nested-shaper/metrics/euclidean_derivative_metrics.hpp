/**
 * @file euclidean_derivative_metrics.hpp
 * 
 * @brief This file contains the definition of the EuclideanDerivativeMetrics class.
 */

#pragma once

#include <stddef.h>
#include <nested-shaper/tiny_utility.hpp> // for ns::array
#include <nested-shaper/Queue.hpp>
#include "central_finite_difference_coefficients.hpp"

namespace ns {

template<typename Type, size_t N>
struct EuclideanDerivativeMetrics {
    array<Type, N> operator()(QueueConstIterator<Type> forwardIterator, QueueConstIterator<Type> backwardIterator, const Type& dt) const {
        assert(N == forwardIterator.size);
        assert(N == backwardIterator.size);
        (void)backwardIterator;

        array<Type, N> derivatives{};
        for(size_t i = 0; i < N; i++) {
            derivatives[i] = Type(0);
        }

        derivatives[0] = *(forwardIterator + (N / 2U)); // Central point

        Type dtn{dt};
        static constexpr array<array<Type, N>, N - 1> coefficients = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<Type, N>::value;
        for(size_t i = 0; i < N - 1; i++) {
            const size_t k = i + 1;
            for(size_t j = 0; j < N; j++) {
                derivatives[k] += coefficients[i][j] * (*(forwardIterator + j));
            }
            derivatives[k] /= dtn;
            dtn *= dt;
        }

        return derivatives;
    }
};

template<typename Type>
struct EuclideanDerivativeMetrics<Type, 1> {
    array<Type, 1> operator()(QueueConstIterator<Type> forwardIterator, QueueConstIterator<Type> backwardIterator, const Type& dt) const {
        assert(1 == forwardIterator.size);
        assert(1 == backwardIterator.size);
        (void)backwardIterator;
        (void)dt;
        return array<Type, 1>{*forwardIterator};
    }
};

// M : dimension
// N : derivative order
// derivatives[i][j] : i-th dimension of j-th derivative
template<typename Type, size_t M, size_t N>
struct EuclideanDerivativeMetricsArray {
    using array_type = array<Type, M>;
    using derivative_type = array<array<Type, N>, M>;

    derivative_type operator()(QueueConstIterator<array_type> forwardIterator, QueueConstIterator<array_type> backwardIterator, const Type& dt) const {
        assert(N == forwardIterator.size);
        assert(N == backwardIterator.size);
        (void)backwardIterator;

        derivative_type derivatives{};
        for(size_t i = 0; i < M; i++) {
            for(size_t j = 0; j < N; j++) {
                derivatives[i][j] = Type(0);
            }
        }

        for(size_t i = 0; i < M; i++) {
            derivatives[i][0] = (*(forwardIterator + (N / 2U)))[i]; // Central point
        }

        static constexpr array<array<Type, N>, N - 1> coefficients = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<Type, N>::value;
        for(size_t m = 0; m < M; m++) {
            Type dtn{dt};
            for(size_t i = 0; i < N - 1; i++) {
                const size_t k = i + 1;
                for(size_t j = 0; j < N; j++) {
                    derivatives[m][k] += coefficients[i][j] * (*(forwardIterator + j))[m];
                }
                derivatives[m][k] /= dtn;
                dtn *= dt;
            }
        }

        return derivatives;
    }
};

template<typename Type, size_t M>
struct EuclideanDerivativeMetricsArray<Type, M, 1> {
    using array_type = array<Type, M>;
    using derivative_type = array<array<Type, 1>, M>;

    derivative_type operator()(QueueConstIterator<array_type> forwardIterator, QueueConstIterator<array_type> backwardIterator, const Type& dt) const {
        assert(1 == forwardIterator.size);
        assert(1 == backwardIterator.size);
        (void)backwardIterator;
        (void)dt;

        derivative_type derivatives;
        for(size_t i = 0; i < M; i++) {
            derivatives[i][0] = (*(forwardIterator))[i];
        }

        return derivatives;
    }
};
}; // namespace ns