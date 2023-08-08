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
        sum = cs = ccs = c = cc = static_cast<T>(0);
    }
    template <typename T>
    void summator<T, SummatorType::KBK>::operator+=(const_reference value)
    {
        T t = sum + value;
        if ((sum > static_cast<T>(0) ? sum : -sum) >= (value > static_cast<T>(0) ? value : -value))
            c += (sum - t) + value;
        else
            c += (value - t) + sum;

        sum = t;
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
        return sum + cs + ccs;
    }
} // namespace ns