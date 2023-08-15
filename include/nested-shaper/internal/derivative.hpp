#pragma once

namespace ns
{
    namespace __ns__internal
    {
        template <typename average_filter, size_t Ordinal>
        struct forward_derivative
        {
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index)
            {
                return (_forward_derivative(_average_filter, dt, index + 1) - _forward_derivative(_average_filter, dt, index)) / dt;
            }
            forward_derivative<average_filter, Ordinal - 1> _forward_derivative;
        };

        template <typename average_filter>
        struct forward_derivative<average_filter, 0>
        {
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index)
            {
                return _average_filter.front(index);
            }
        };

        template <typename average_filter, size_t Ordinal>
        struct backward_derivative
        {
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index)
            {
                return (_backward_derivative(_average_filter, dt, index) - _backward_derivative(_average_filter, dt, index + 1)) / dt;
            }
            backward_derivative<average_filter, Ordinal - 1> _backward_derivative;
        };

        template <typename average_filter>
        struct backward_derivative<average_filter, 0>
        {
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index)
            {
                return _average_filter.back(index);
            }
        };
    } // namespace internal
};