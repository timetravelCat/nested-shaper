#pragma once

#include "summator.hpp"

namespace ns
{
    template <typename T>
    class summator<T, SummatorType::NAIVE> : public summator_interface<T>
    {
    public:
        using value_type = typename summator_interface<T>::value_type;
        using const_reference = typename summator_interface<T>::const_reference;

        summator();
        void reset() override;
        void operator+=(const_reference value) override;
        value_type get() const override;
    };
    
    template <typename T>
    summator<T, SummatorType::NAIVE>::summator() { reset(); }
    template <typename T>
    void summator<T, SummatorType::NAIVE>::reset() { summator_interface<T>::sum = static_cast<T>(0); }
    template <typename T>
    void summator<T, SummatorType::NAIVE>::operator+=(const_reference value) { summator_interface<T>::sum += value; }
    template <typename T>
    typename summator<T, SummatorType::NAIVE>::value_type summator<T, SummatorType::NAIVE>::get() const { return summator_interface<T>::sum; }
} // namespace ns