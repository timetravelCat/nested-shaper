#pragma once

namespace ns
{
    // Parent class of sized_queue
    template <typename T>
    class queue
    {
    public:
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using size_type = size_t;

        // Test whether container is empty
        virtual bool empty() const = 0;
        // Test whether container is full
        virtual bool full() const = 0;
        // Return size
        virtual size_type size() const = 0;
        // Return maximum size <= capacity
        virtual size_type max_size() const = 0;
        // Return capacity, preallocated buffer by derived template argument
        virtual size_type capacity() const = 0;
        // Set as empty
        virtual void reset() = 0;
        // Set maximum size of queue
        virtual void resize(const size_type &size) = 0;
        // Fill all data as given value
        virtual void fill(const value_type &value) = 0;
        // Removes first(old) element
        virtual void pop() = 0;
        // Insert element at the end(recent)
        virtual void push(const value_type &value) = 0;
        // Access first(old) element
        virtual reference front(const size_type &index = 0) = 0;
        virtual const_reference front(const size_type &index = 0) const = 0;
        // Access last(recent) element
        virtual reference back(const size_type &index = 0) = 0;
        virtual const_reference back(const size_type &index = 0) const = 0;
    };

    template <typename T>
    class filter : virtual public queue<T>
    {
    public:
        // Calculate filter results on current queue.
        virtual value_type get() const = 0;
        // Pubsh and calculate filter.
        virtual value_type retrieve(const_reference value) = 0;
    };
} // namespace ns
