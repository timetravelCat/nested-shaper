#pragma once

namespace ns
{
    namespace __ns__internal
    {
        template <size_t Ordinal>
        struct forward_derivative
        {
            template <typename average_filter>
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index) const
            {
                return (forward_derivative<Ordinal - 1>{}(_average_filter, dt, index + 1) - forward_derivative<Ordinal - 1>{}(_average_filter, dt, index)) / dt;
            }
        };

        template <>
        struct forward_derivative<0>
        {
            template <typename average_filter>
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index) const
            {
                return _average_filter.front(index);
            }
        };

        template <size_t Ordinal>
        struct backward_derivative
        {
            template <typename average_filter>
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index) const
            {
                return (backward_derivative<Ordinal - 1>{}(_average_filter, dt, index) - backward_derivative<Ordinal - 1>{}(_average_filter, dt, index + 1)) / dt;
            }
        };

        template <>
        struct backward_derivative<0>
        {
            template <typename average_filter>
            typename average_filter::value_type operator()(const average_filter &_average_filter, typename average_filter::const_reference dt, const size_t &index) const
            {
                return _average_filter.back(index);
            }
        };

        template <size_t Depth, size_t TotalDepth>
        struct recursive_derivative
        {
            template <typename average_filter, typename T>
            void derivative(const average_filter &_p, T result[TotalDepth], const T &dt) const
            {
                result[Depth - 1] = (backward_derivative<Depth - 2>{}(_p, dt, 0) -
                                     forward_derivative<Depth - 2>{}(_p, dt, 0)) /
                                    (dt * static_cast<T>(TotalDepth - Depth + 1));
                recursive_derivative<Depth - 1, TotalDepth>{}.derivative(_p, result, dt);
            }
        };

        template <size_t TotalDepth>
        struct recursive_derivative<1, TotalDepth>
        {
            template <typename average_filter, typename T>
            void derivative(const average_filter &_p, T result[TotalDepth], const T &dt) const
            {
                result[0] = _p.get();
            }
        };
    } // namespace internal
};