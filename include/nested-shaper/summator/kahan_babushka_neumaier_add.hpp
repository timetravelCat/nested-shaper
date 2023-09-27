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
        using value_type = typename summator_interface<T>::value_type;
        using const_reference = typename summator_interface<T>::const_reference;

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
        summator_interface<T>::sum = c = static_cast<T>(0);
    }
    template <typename T>
    void summator<T, SummatorType::KBN>::operator+=(const_reference value)
    {
        const T t = summator_interface<T>::sum + value;
        if ((summator_interface<T>::sum > static_cast<T>(0) ? summator_interface<T>::sum : -summator_interface<T>::sum) >= (value > static_cast<T>(0) ? value : -value))
            c += (summator_interface<T>::sum - t) + value;
        else
            c += (value - t) + summator_interface<T>::sum;

        summator_interface<T>::sum = t;
    }
    template <typename T>
    typename summator<T, SummatorType::KBN>::value_type summator<T, SummatorType::KBN>::get() const
    {
        return summator_interface<T>::sum + c;
    }
} // namespace ns