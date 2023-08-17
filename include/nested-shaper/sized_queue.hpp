#pragma once

#include "container.hpp"

namespace ns
{
    template <typename T, size_t Capacity>
    class sized_queue : virtual public queue<T>
    {
    public:
        sized_queue();
        sized_queue(const size_type &max_size);
        sized_queue(const sized_queue &rhs);

        void operator=(const sized_queue &rhs);
        bool empty() const override;
        bool full() const override;
        size_type size() const override;
        size_type max_size() const override;
        size_type capacity() const override;
        virtual void reset() override;
        virtual void resize(const size_type &size) override;
        virtual void fill(const value_type &value) override;
        virtual void pop() override;
        virtual void push(const value_type &value) override;
        reference front(const size_type &index = 0) override;
        const_reference front(const size_type &index = 0) const override;
        reference back(const size_type &index = 0) override;
        const_reference back(const size_type &index = 0) const override;

    protected:
        size_type _back;  // recent index
        size_type _front; // old index
        size_type _size;
        size_type _max_size;
        value_type _data[Capacity];
    };

    // Constructors
    template <typename T, size_t Capacity>
    sized_queue<T, Capacity>::sized_queue() { resize(capacity()); }
    template <typename T, size_t Capacity>
    sized_queue<T, Capacity>::sized_queue(const size_type &max_size) { resize(max_size); }
    template <typename T, size_t Capacity>
    sized_queue<T, Capacity>::sized_queue(const sized_queue &rhs) { *this = rhs; }

    // Member functions
    template <typename T, size_t Capacity>
    void sized_queue<T, Capacity>::operator=(const sized_queue &rhs)
    {
        _back = rhs._back;
        _front = rhs._front;
        _size = rhs._size;
        _max_size = rhs._max_size;
        for (size_t i = 0; i < Capacity; i++)
        {
            _data[i] = rhs._data[i];
        }
    }
    template <typename T, size_t Capacity>
    bool sized_queue<T, Capacity>::empty() const { return _size == 0; }
    template <typename T, size_t Capacity>
    bool sized_queue<T, Capacity>::full() const { return _size == _max_size; }
    template <typename T, size_t Capacity>
    typename sized_queue<T, Capacity>::size_type sized_queue<T, Capacity>::size() const { return _size; }
    template <typename T, size_t Capacity>
    typename sized_queue<T, Capacity>::size_type sized_queue<T, Capacity>::max_size() const { return _max_size; }
    template <typename T, size_t Capacity>
    typename sized_queue<T, Capacity>::size_type sized_queue<T, Capacity>::capacity() const { return Capacity; }
    template <typename T, size_t Capacity>
    void sized_queue<T, Capacity>::reset()
    {
        // mark as empty
        _front = 0;
        _back = _max_size - 1;
        _size = 0;
    }
    template <typename T, size_t Capacity>
    void sized_queue<T, Capacity>::resize(const size_type &size)
    {
        _max_size = size;
        reset();
        if (size > Capacity)
            _max_size = Capacity;
        else if (size == 0)
            _max_size = 1;
    }
    template <typename T, size_t Capacity>
    void sized_queue<T, Capacity>::fill(const value_type &value)
    {
        for (size_t i = 0; i < Capacity; i++)
            _data[i] = value;

        // mark as full
        _size = _max_size;
        _back = _max_size - 1;
        _front = 0;
    }
    template <typename T, size_t Capacity>
    void sized_queue<T, Capacity>::pop()
    {
        if (empty())
            return;

        _size--;
        _front == _max_size - 1 ? _front = 0 : _front++;
    }
    template <typename T, size_t Capacity>
    void sized_queue<T, Capacity>::push(const value_type &value)
    {
        if (full())
            pop();

        _size++;
        _back == _max_size - 1 ? _back = 0 : _back++;
        _data[_back] = value;
    };
    template <typename T, size_t Capacity>
    typename sized_queue<T, Capacity>::reference sized_queue<T, Capacity>::front(const size_type &index)
    {
        if (index >= _size)
            return back();
        return _data[(_front + index) % _max_size];
    }
    template <typename T, size_t Capacity>
    typename sized_queue<T, Capacity>::const_reference sized_queue<T, Capacity>::front(const size_type &index) const
    {
        if (index >= _size)
            return back();
        return _data[(_front + index) % _max_size];
    }
    template <typename T, size_t Capacity>
    typename sized_queue<T, Capacity>::reference sized_queue<T, Capacity>::back(const size_type &index)
    {
        if (index >= _size)
            return front();
        return _back >= index ? _data[_back - index] : _data[_max_size - index + _back];
    }
    template <typename T, size_t Capacity>
    typename sized_queue<T, Capacity>::const_reference sized_queue<T, Capacity>::back(const size_type &index) const
    {
        if (index >= _size)
            return front();
        return _back >= index ? _data[_back - index] : _data[_max_size - index + _back];
    }
};