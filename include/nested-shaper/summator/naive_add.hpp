#pragma once

#include "summator.hpp"

namespace ns
{
    template <typename T>
    class summator<T, SummatorType::NAIVE> : public summator_interface<T>
    {
    public:
        summator();
        void reset() override;
        void operator+=(const_reference value) override;
        value_type get() const override;
    };
    
    template <typename T>
    summator<T, SummatorType::NAIVE>::summator() { reset(); }
    template <typename T>
    void summator<T, SummatorType::NAIVE>::reset() { sum = static_cast<T>(0); }
    template <typename T>
    void summator<T, SummatorType::NAIVE>::operator+=(const_reference value) { sum += value; }
    template <typename T>
    typename summator<T, SummatorType::NAIVE>::value_type summator<T, SummatorType::NAIVE>::get() const { return sum; }
} // namespace ns