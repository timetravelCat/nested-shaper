#pragma once

#include "summator.hpp"

namespace ns
{
    // https://en.wikipedia.org/wiki/Kahan_summation_algorithm
    // function KahanBabushkaNeumaierSum(input)
    template <typename T>
    class summator<T, SummatorType::KBN> : public summator_interface<T>
    {
    public:
        summator();
        void reset() override;
        void operator+=(const_reference value) override;
        value_type get() const override;

    private:
        T c;
    };
    
    template <typename T>
    summator<T, SummatorType::KBN>::summator() { reset(); }
    template <typename T>
    void summator<T, SummatorType::KBN>::reset()
    {
        sum = c = static_cast<T>(0);
    }
    template <typename T>
    void summator<T, SummatorType::KBN>::operator+=(const_reference value)
    {
        const T t = sum + value;
        if ((sum > static_cast<T>(0) ? sum : -sum) >= (value > static_cast<T>(0) ? value : -value))
            c += (sum - t) + value;
        else
            c += (value - t) + sum;

        sum = t;
    }
    template <typename T>
    typename summator<T, SummatorType::KBN>::value_type summator<T, SummatorType::KBN>::get() const
    {
        return sum + c;
    }
} // namespace ns