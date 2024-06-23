
/**
 * @file Queue.hpp
 * 
 * @brief This file contains the definition of the Queue class.
 * 
 * @author timetravelCat <timetraveler930@gmail.com>
 */

#pragma once

#include "version.hpp"
#include <stddef.h>
#include <assert.h>

namespace ns {
/**
 * @class Queue
 * 
 * A template based queue class.
 * 
 * @tparam Type Type of the elements.
 * @tparam Extent Maximum extent of the queue.
 */
template<typename Type, size_t Extent>
class Queue {
public:
    using value_type = Type;
    using reference = Type&;
    using const_reference = const Type&;
    using size_type = size_t;
    static constexpr size_type extent = Extent;

    /**
     * Constructors
     */
    Queue();
    explicit Queue(const size_type& capacity);

    /**
     * Status of the queue
     */
    inline bool isEmpty() const { return _size == 0; }
    inline bool isFull() const { return _size == _capacity; }
    inline size_type capacity() const { return _capacity; }
    inline size_type size() const { return _size; }

    /**
     * Accessors
     * 
     * front : retrieve from the most recent index
     * back : retrieve from the most old index
     */
    reference front(const size_type& index = 0);
    const_reference front(const size_type& index = 0) const;
    reference back(const size_type& index = 0);
    const_reference back(const size_type& index = 0) const;

    /**
     * reset : Set queue as empty
     * resize : Set capacity of queue
     */
    void reset();
    void resize(const size_type& size);

    /**
     * Pop and Push
     */
    void pop();
    void push(const value_type& value);

    /**
     * Fill the queue with given value.
     */
    void fill(const value_type& value);

    /**
     * Iterator class, allows to iterate over the queue.
     */
    struct ConstIterator {
        explicit ConstIterator(const Queue& queue, const value_type* _ptr) :
        size(queue._size),
        ptr(_ptr),
        pBegin(queue._data),
        pEnd(queue._data + queue._capacity) {}

        inline const_reference operator*() const { return *ptr; }
        inline ConstIterator& operator++() {
            if(++ptr == pEnd)
                ptr = pBegin;

            ++counter;
            return *this;
        }
        inline ConstIterator operator++(int) {
            ConstIterator tmp(*this);
            operator++();
            return tmp;
        }
        inline bool operator==(const ConstIterator& other) const {
            return ptr == other.ptr;
        }
        inline bool operator!=(const ConstIterator& other) const {
            if(size == 0) {
                return false;
            }

            if(counter == 0) {
                return true;
            }

            return ptr != other.ptr;
        }

    private:
        size_type counter{0};
        const size_type size;
        const value_type* ptr;
        const value_type* const pBegin;
        const value_type* const pEnd;
    };

    struct Iterator {
        explicit Iterator(Queue& queue, value_type* _ptr) :
        size(queue._size),
        ptr(_ptr),
        pBegin(queue._data),
        pEnd(queue._data + queue._capacity) {}

        inline reference operator*() const { return *ptr; }
        inline Iterator& operator++() {
            if(++ptr == pEnd)
                ptr = pBegin;

            ++counter;
            return *this;
        }
        inline Iterator operator++(int) {
            Iterator tmp(*this);
            operator++();
            return tmp;
        }
        inline bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }
        inline bool operator!=(const Iterator& other) const {
            if(size == 0) {
                return false;
            }

            if(counter == 0) {
                return true;
            }

            return ptr != other.ptr;
        }

    private:
        size_type counter{0};
        const size_type size;
        value_type* ptr;
        value_type* pBegin;
        value_type* pEnd;
    };

    /**
     * Begin and End
     */
    inline ConstIterator cbegin() const { return ConstIterator{*this, _back}; }
    inline ConstIterator cend() const { return ConstIterator{*this, ((_front + 1) == (_data + _capacity)) ? _data : _front + 1}; }
    inline Iterator begin() { return Iterator(*this, _back); }
    inline Iterator end() { return Iterator(*this, ((_front + 1) == (_data + _capacity)) ? _data : _front + 1); }

protected:
    value_type _data[Extent]{};
    size_type _capacity;                       // maximum size
    size_type _size{0};                        // current size
    value_type* _back{_data};                  // old index
    value_type* _front{_data + _capacity - 1}; // recent index
};

template<typename Type, size_t Extent>
Queue<Type, Extent>::Queue() :
_capacity(Extent) {}

template<typename Type, size_t Extent>
Queue<Type, Extent>::Queue(const size_type& capacity) :
_capacity(capacity) {
    assert(capacity <= Extent);
}

template<typename Type, size_t Extent>
typename Queue<Type, Extent>::reference Queue<Type, Extent>::front(const size_type& index) {
    if(index >= _size) {
        assert(false);
        return *_back;
    }

    return (_front - index >= _data) ? *(_front - index) : *(_front + _capacity - index);
}

template<typename Type, size_t Extent>
typename Queue<Type, Extent>::const_reference Queue<Type, Extent>::front(const size_type& index) const {
    if(index >= _size) {
        assert(false);
        return *_back;
    }

    return (_front - index >= _data) ? *(_front - index) : *(_front + _capacity - index);
}

template<typename Type, size_t Extent>
typename Queue<Type, Extent>::reference Queue<Type, Extent>::back(const size_type& index) {
    if(index >= _size) {
        assert(false);
        return *_front;
    }

    return (_back + index >= _data + _capacity) ? *(_back + index - _capacity) : *(_back + index);
}

template<typename Type, size_t Extent>
typename Queue<Type, Extent>::const_reference Queue<Type, Extent>::back(const size_type& index) const {
    if(index >= _size) {
        assert(false);
        return *_front;
    }

    return (_back + index >= _data + _capacity) ? *(_back + index - _capacity) : *(_back + index);
}

template<typename Type, size_t Extent>
void Queue<Type, Extent>::reset() {
    _size = 0;
    _back = _data;
    _front = _data + _capacity - 1;
}

template<typename Type, size_t Extent>
void Queue<Type, Extent>::resize(const size_type& size) {
    if(size > Extent) {
        assert(false);
        resize(Extent);
        return;
    }

    if(size == _capacity) {
        return;
    }

    _capacity = size;
    reset();
}

template<typename Type, size_t Extent>
void Queue<Type, Extent>::pop() {
    if(isEmpty()) {
        assert(false);
        return;
    }

    --_size;
    _back == _data + _capacity - 1 ? _back = _data : _back++;
}

template<typename Type, size_t Extent>
void Queue<Type, Extent>::push(const value_type& value) {
    if(isFull()) {
        pop();
    }

    ++_size;
    _front == _data + _capacity - 1 ? _front = _data : _front++;
    *_front = value;
}

template<typename Type, size_t Extent>
void Queue<Type, Extent>::fill(const value_type& value) {
    _size = _capacity;
    _back = _data;
    _front = _data + _capacity - 1;

    for(size_type i = 0; i < _capacity; ++i) {
        _data[i] = value;
    }
}
}; // namespace ns