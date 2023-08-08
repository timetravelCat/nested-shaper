#pragma once

namespace ns
{
    enum class SummatorType
    {
        NAIVE,
        KBK, // KAHAN_BABUSHKA_KLEIN,
        KBN, // KAHAN_BABUSHKA_NEUMAIER
    };

    // Interface class for summator implementations
    template <typename T>
    class summator_interface
    {
    public:
        using value_type = T;
        using reference_type = T &;
        using const_reference = const T &;

        // summator required to implement reset, operator += and getter
        virtual void reset() = 0;
        virtual void operator+=(const_reference value) = 0;
        virtual value_type get() const = 0;

        void operator-=(const_reference value) { operator+=(-value); }

    protected:
        T sum;
    };

    // Dummy class
    template <typename T, SummatorType type>
    class summator : public summator_interface<T>
    {
        // summator required to implement reset, operator += and getter
        // virtual void reset() = 0;
        // virtual void operator+=(const_reference value) = 0;
        // virtual value_type get() const = 0;
    };
} // namespace ns