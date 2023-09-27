#pragma once

#include "summator.hpp"

namespace ns
{
    // https://en.wikipedia.org/wiki/Kahan_summation_algorithm
    // function KahanBabushkaKleinSum(input)
    template <typename T>
    class summator<T, SummatorType::KBK> : public summator_interface<T>
    {
    public:
        using value_type = typename summator_interface<T>::value_type;
        using const_reference = typename summator_interface<T>::const_reference;

        summator();
        void reset() override;
        void operator+=(const_reference value) override;
        value_type get() const override;

    private:
        T cs;
        T ccs;
        T c;
        T cc;
    };
    
    template <typename T>
    summator<T, SummatorType::KBK>::summator() { reset(); }
    template <typename T>
    void summator<T, SummatorType::KBK>::reset()
    {
        summator_interface<T>::sum = cs = ccs = c = cc = static_cast<T>(0);
    }
    template <typename T>
    void summator<T, SummatorType::KBK>::operator+=(const_reference value)
    {
        T t = summator_interface<T>::sum + value;
        if ((summator_interface<T>::sum > static_cast<T>(0) ? summator_interface<T>::sum : -summator_interface<T>::sum) >= (value > static_cast<T>(0) ? value : -value))
            c += (summator_interface<T>::sum - t) + value;
        else
            c += (value - t) + summator_interface<T>::sum;

        summator_interface<T>::sum = t;
        t = cs + c;

        if ((cs > static_cast<T>(0) ? cs : -cs) >= (c > static_cast<T>(0) ? c : -c))
            cc = (cs - t) + c;
        else
            cc = (c - t) + cs;

        cs = t;
        ccs = ccs + cc;
    }
    template <typename T>
    typename summator<T, SummatorType::KBK>::value_type summator<T, SummatorType::KBK>::get() const
    {
        return summator_interface<T>::sum + cs + ccs;
    }
} // namespace ns