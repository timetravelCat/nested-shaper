/**
 * @file angle_derivative_metrics.hpp
 * 
 * @brief This file contains the definition of the AngleDerivativeMetrics class.
 */

#pragma once

#include <stddef.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <nested-shaper/tiny_utility.hpp> // for ns::array
#include <nested-shaper/Queue.hpp>
#include "central_finite_difference_coefficients.hpp"

namespace ns {
template<typename Type, size_t N>
struct AngleDerivativeMetrics {
    array<Type, N> operator()(QueueConstIterator<Type> forwardIterator, QueueConstIterator<Type> backwardIterator, const Type& dt) const {
        assert(N == forwardIterator.size);
        assert(N == backwardIterator.size);
        (void)backwardIterator;

        array<Type, N> angles_wrapped{};
        angles_wrapped[0] = *(forwardIterator++); // Assign first angle

        for(size_t i = 1U; i < N; i++) {
            angles_wrapped[i] = wrap(*(forwardIterator++), angles_wrapped[i] - Type(M_PI), angles_wrapped[i] + Type(M_PI));
        }

        array<Type, N> derivatives{};
        for(size_t i = 0; i < N; i++) {
            derivatives[i] = Type(0);
        }

        derivatives[0] = angles_wrapped[N / 2U]; // Central point

        Type dtn{dt};
        static constexpr array<array<Type, N>, N - 1> coefficients = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<Type, N>::value;
        for(size_t i = 0; i < N - 1; i++) {
            const size_t k = i + 1;
            for(size_t j = 0; j < N; j++) {
                derivatives[k] += coefficients[i][j] * angles_wrapped[j];
            }
            derivatives[k] /= dtn;
            dtn *= dt;
        }

        return derivatives;
    }
};

template<typename Type>
struct AngleDerivativeMetrics<Type, 1> {
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
struct AngleDerivativeMetricsArray {
    using array_type = array<Type, M>;
    using derivative_type = array<array<Type, N>, M>;

    derivative_type operator()(QueueConstIterator<array_type> forwardIterator, QueueConstIterator<array_type> backwardIterator, const Type& dt) const {
        assert(N == forwardIterator.size);
        assert(N == backwardIterator.size);
        (void)backwardIterator;

        Type angles_wrapped[N][M];
        for(size_t i = 0; i < M; i++) {
            angles_wrapped[0][i] = (*forwardIterator)[i]; // Assign first angle
        }
        ++forwardIterator;

        for(size_t i = 1U; i < N; i++) {
            for(size_t j = 0; j < M; j++) {
                angles_wrapped[i][j] = wrap((*forwardIterator)[j], angles_wrapped[i - 1U][j] - Type(M_PI), angles_wrapped[i - 1U][j] + Type(M_PI));
            }
            ++forwardIterator;
        }

        derivative_type derivatives{};
        for(size_t i = 0; i < M; i++) {
            for(size_t j = 0; j < N; j++) {
                derivatives[i][j] = Type(0);
            }
        }

        for(size_t i = 0; i < M; i++) {
            derivatives[i][0] = angles_wrapped[N / 2U][i]; // Central point
        }

        static constexpr array<array<Type, N>, N - 1> coefficients = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<Type, N>::value;
        for(size_t m = 0; m < M; m++) {
            Type dtn{dt};
            for(size_t i = 0; i < N - 1; i++) {
                const size_t k = i + 1;
                for(size_t j = 0; j < N; j++) {
                    derivatives[m][k] += coefficients[i][j] * angles_wrapped[j][m];
                }
                derivatives[m][k] /= dtn;
                dtn *= dt;
            }
        }

        return derivatives;
    }
};

template<typename Type, size_t M>
struct AngleDerivativeMetricsArray<Type, M, 1> {
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
} // namespace ns