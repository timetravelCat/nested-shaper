/**
 * @file central_finite_difference_coefficients.hpp
 * 
 * @brief This file contains the definition of the CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS function.
 */
#pragma once

#include <stddef.h>
#include <nested-shaper/tiny_utility.hpp> // for ns::array

namespace ns {
/**
 * N : Number of elements in the queue
 * https://en.wikipedia.org/wiki/Finite_difference_coefficient
 * https://web.media.mit.edu/~crtaylor/calculator.html
 */
template<typename T, size_t N>
constexpr array<array<T, N>, N - 1> CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS() {
    static_assert((N == 3) || (N == 5) || (N == 7) || (N == 9), "Not implemented");
    return array<array<T, N>, N - 1>{};
}

template<typename T>
constexpr array<array<T, 3>, 2> CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS() {
    return array<array<T, 3>, 2>{
      array<T, 3>{T(-0.5), T(0), T(0.5)},
      array<T, 3>{T(1.0), T(-2.0), T(1.0)}};
}

template<typename T>
constexpr array<array<T, 5>, 4> CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS() {
    return array<array<T, 5>, 4>{
      array<T, 5>{T(1.0 / 12.0), T(-2.0 / 3.0), T(0), T(2.0 / 3.0), T(-1.0 / 12.0)},
      array<T, 5>{T(-1.0 / 12.0), T(4.0 / 3.0), T(-5.0 / 2.0), T(4.0 / 3.0), T(-1.0 / 12.0)},
      array<T, 5>{T(-1.0 / 2.0), T(1.0), T(0), T(-1.0), T(1.0 / 2.0)},
      array<T, 5>{T(1.0), T(-4.0), T(6.0), T(-4.0), T(1.0)}};
}

template<typename T>
constexpr array<array<T, 7>, 6> CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS() {
    return array<array<T, 7>, 6>{
      array<T, 7>{-T(1.0 / 60.0), T(3.0 / 20.0), T(-3.0 / 4.0), T(0.0), T(3.0 / 4.0), T(-3.0 / 20.0), T(1.0 / 60.0)},
      array<T, 7>{T(1.0 / 90.0), T(-3.0 / 20.0), T(3.0 / 2.0), T(-49.0 / 18.0), T(3.0 / 2.0), T(-3.0 / 20.0), T(1.0 / 90.0)},
      array<T, 7>{T(1.0 / 8.0), T(-1.0), T(13.0 / 8.0), T(0.0), T(-13.0 / 8.0), T(1.0), T(-1.0 / 8.0)},
      array<T, 7>{T(-1.0 / 6.0), T(2.0), T(-13.0 / 2.0), T(28.0 / 3.0), T(-13.0 / 2.0), T(2.0), T(-1.0 / 6.0)},
      array<T, 7>{T(-1.0 / 2.0), T(2.0), T(-5.0 / 2.0), T(0.0), T(5.0 / 2.0), T(-2.0), T(1.0 / 2.0)},
      array<T, 7>{T(1.0), T(-6.0), T(15.0), T(-20.0), T(15.0), T(-6.0), T(1.0)}};
}

template<typename T>
constexpr array<array<T, 9>, 8> CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS() {
    return array<array<T, 9>, 8>{
      array<T, 9>{T(3.0 / 840.0), T(-32.0 / 840.0), T(168.0 / 840.0), T(-672.0 / 840.0), T(0.0 / 840.0), T(672.0 / 840.0), T(-168.0 / 840.0), T(32.0 / 840.0), T(-3.0 / 840.0)},
      array<T, 9>{T(-9.0 / 5040.0), T(128.0 / 5040.0), T(-1008.0 / 5040.0), T(8064.0 / 5040.0), T(-14350.0 / 5040.0), T(8064.0 / 5040.0), T(-1008.0 / 5040.0), T(128.0 / 5040.0), T(-9.0 / 5040.0)},
      array<T, 9>{T(-7.0 / 240.0), T(72.0 / 240.0), T(-338.0 / 240.0), T(488.0 / 240.0), T(0.0 / 240.0), T(-488.0 / 240.0), T(338.0 / 240.0), T(-72.0 / 240.0), T(7.0 / 240.0)},
      array<T, 9>{T(7.0 / 240.0), T(-96.0 / 240.0), T(676.0 / 240.0), T(-1952.0 / 240.0), T(2730.0 / 240.0), T(-1952.0 / 240.0), T(676.0 / 240.0), T(-96.0 / 240.0), T(7.0 / 240.0)},
      array<T, 9>{T(1.0 / 6.0), T(-9.0 / 6.0), T(26.0 / 6.0), T(-29.0 / 6.0), T(0.0 / 6.0), T(29.0 / 6.0), T(-26.0 / 6.0), T(9.0 / 6.0), T(-1.0 / 6.0)},
      array<T, 9>{T(-1.0 / 4.0), T(12.0 / 4.0), T(-52.0 / 4.0), T(116.0 / 4.0), T(-150.0 / 4.0), T(116.0 / 4.0), T(-52.0 / 4.0), T(12.0 / 4.0), T(-1.0 / 4.0)},
      array<T, 9>{T(-1.0 / 2.0), T(6.0 / 2.0), T(-14.0 / 2.0), T(14.0 / 2.0), T(0.0 / 2.0), T(-14.0 / 2.0), T(14.0 / 2.0), T(-6.0 / 2.0), T(1.0 / 2.0)},
      array<T, 9>{T(1.0), T(-8.0), T(28.0), T(-56.0), T(70.0), T(-56.0), T(28.0), T(-8.0), T(1.0)},
    };
};
}; // namespace ns