/**
 * @file kahan_accumulator.hpp
 * 
 * @brief This file contains the definition of the KahanAccumulator class.
 */

#pragma once

#include "math.h"

namespace ns {
template<typename Type>
class KahanBabushkaNeumaierAccumulator {
public:
    Type accumulate(const Type& prev, const Type& value) {
        const Type t = prev + value;
        c += ::fabs(prev) >= ::fabs(value) ? (prev - t) + value : (value - t) + prev;
        return t + c;
    }

private:
    Type c{Type(0)}; // A running compensation for lost low-order bits.
};
} // namespace ns
