#pragma once

#include "sized_queue.hpp"
#include "summator/summators.hpp"

namespace ns
{
    // Recursive implementation (Fast, but vulnerable to long time operation)
    template <typename T, size_t Capacity, bool Recursive = true, SummatorType summatorType = SummatorType::KBN>
    class average_filter : public sized_queue<T, Capacity>, public filter<T>
    {
    public:
        using value_type = typename queue<T>::value_type;
        using reference = typename queue<T>::reference;
        using const_reference = typename queue<T>::const_reference;
        using size_type = typename queue<T>::size_type;

        average_filter();
        average_filter(const size_type &max_size);
        average_filter(const average_filter &rhs);

        T get() const override;
        T retrieve(const T &value) override;

        void operator=(const average_filter &rhs);
        void reset() override;
        void resize(const size_type &size) override;
        void fill(const value_type &value) override;
        void pop() override;
        void push(const value_type &value) override;

    private:
        summator<T, summatorType> _summator;
    };

    // Smooth implementation (Very slow, but strong to long time operation)
    template <typename T, size_t Capacity, SummatorType type>
    class average_filter<T, Capacity, false, type> : public sized_queue<T, Capacity>, public filter<T>
    {
    public:
        using value_type = typename queue<T>::value_type;
        using reference = typename queue<T>::reference;
        using const_reference = typename queue<T>::const_reference;
        using size_type = typename queue<T>::size_type;

        average_filter();
        average_filter(const size_type &max_size);
        average_filter(const average_filter &rhs);

        T get() const override;
        T retrieve(const T &value) override;
    };

    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    average_filter<T, Capacity, Recursive, summatorType>::average_filter() : sized_queue<T, Capacity>() {}
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    average_filter<T, Capacity, Recursive, summatorType>::average_filter(const size_type &max_size) : sized_queue<T, Capacity>(max_size) {}
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    average_filter<T, Capacity, Recursive, summatorType>::average_filter(const average_filter &rhs) : sized_queue<T, Capacity>(rhs) { _summator = rhs._summator; }

    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    T average_filter<T, Capacity, Recursive, summatorType>::get() const
    {
        return _summator.get() / static_cast<T>(sized_queue<T, Capacity>::size());
    }
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    T average_filter<T, Capacity, Recursive, summatorType>::retrieve(const T &value)
    {
        push(value);
        return get();
    }
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    void average_filter<T, Capacity, Recursive, summatorType>::operator=(const average_filter &rhs)
    {
        sized_queue<T, Capacity>::operator=(rhs);
        _summator = rhs._summator;
    }
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    void average_filter<T, Capacity, Recursive, summatorType>::reset()
    {
        sized_queue<T, Capacity>::reset();
        _summator.reset();
    }
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    void average_filter<T, Capacity, Recursive, summatorType>::resize(const size_type &size)
    {
        sized_queue<T, Capacity>::resize(size);
        _summator.reset();
    }
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    void average_filter<T, Capacity, Recursive, summatorType>::fill(const value_type &value)
    {
        sized_queue<T, Capacity>::fill(value);
        _summator.set(static_cast<T>(sized_queue<T, Capacity>::size()) * value);
    }
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    void average_filter<T, Capacity, Recursive, summatorType>::pop()
    {
        _summator -= sized_queue<T, Capacity>::front();
        sized_queue<T, Capacity>::pop();
    }
    template <typename T, size_t Capacity, bool Recursive, SummatorType summatorType>
    void average_filter<T, Capacity, Recursive, summatorType>::push(const value_type &value)
    {
        sized_queue<T, Capacity>::push(value);
        _summator += value;
    }

    template <typename T, size_t Capacity, SummatorType summatorType>
    average_filter<T, Capacity, false, summatorType>::average_filter() : sized_queue<T, Capacity>() {}
    template <typename T, size_t Capacity, SummatorType summatorType>
    average_filter<T, Capacity, false, summatorType>::average_filter(const size_type &max_size) : sized_queue<T, Capacity>(max_size) {}
    template <typename T, size_t Capacity, SummatorType summatorType>
    average_filter<T, Capacity, false, summatorType>::average_filter(const average_filter &rhs) : sized_queue<T, Capacity>(rhs) {}
    template <typename T, size_t Capacity, SummatorType summatorType>
    T average_filter<T, Capacity, false, summatorType>::get() const
    {
        summator<T, summatorType> _summator;
        if (sized_queue<T, Capacity>::full())
        {
            for (size_t i = 0; i < sized_queue<T, Capacity>::size(); i++)
                _summator += sized_queue<T, Capacity>::_data[i];
        }
        else
        {
            for (size_t i = 0; i < sized_queue<T, Capacity>::size(); i++)
                _summator += sized_queue<T, Capacity>::back(i);
        }
        return _summator.get() / static_cast<T>(sized_queue<T, Capacity>::size());
    }
    template <typename T, size_t Capacity, SummatorType summatorType>
    T average_filter<T, Capacity, false, summatorType>::retrieve(const T &value)
    {
        sized_queue<T, Capacity>::push(value);
        return get();
    }
} // namespace ns