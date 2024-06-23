
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
template<typename Type>
struct QueueIterator;
template<typename Type>
struct QueueConstIterator;

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

    template<typename _Type>
    friend struct QueueIterator;
    template<typename _Type>
    friend struct QueueConstIterator;

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
     * Iterators
     * 
     * Warnings : 
     * forwardIterator should use ++ or + operator.
     * backwardIterator should use -- or - operator.
     */
    inline QueueIterator<Type> forwardIterator() { return QueueIterator<Type>(*this, _back); }
    inline QueueIterator<Type> backwardIterator() { return QueueIterator<Type>(*this, _front); }
    inline QueueConstIterator<Type> forwardConstIterator() const { return QueueConstIterator<Type>{*this, _back}; }
    inline QueueConstIterator<Type> backwardConstIterator() const { return QueueConstIterator<Type>{*this, _front}; }

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

/**
 * Iterator of circular buffer.
 */
template<typename Type>
struct QueueIterator {
    template<size_t Extent>
    explicit QueueIterator(Queue<Type, Extent>& queue, Type* _ptr) :
    size(queue._size), ptr(_ptr), pBegin(queue._data), pEnd(queue._data + queue.extent) {}
    QueueIterator(const QueueIterator&) = default;
    QueueIterator(const QueueIterator<Type>& other, Type* _ptr) :
    size(other.size), ptr(_ptr), pBegin(other.pBegin), pEnd(other.pEnd) {}

    inline Type& operator*() const { return *ptr; }

    inline QueueIterator& operator++() {
        if(++ptr == pEnd) {
            ptr = pBegin;
        }

        return *this;
    }
    inline QueueIterator operator++(int) {
        QueueIterator tmp(*this);
        operator++();
        return tmp;
    }
    inline QueueIterator& operator--() {
        if(--ptr < pBegin) {
            ptr = pEnd - 1U;
        }

        return *this;
    }
    inline QueueIterator operator--(int) {
        QueueIterator tmp(*this);
        operator--();
        return tmp;
    }

    inline QueueIterator operator+(const size_t& n) const {
        return QueueIterator{*this, (ptr + n >= pEnd) ? pBegin + (ptr + n - pEnd) : ptr + n};
    }
    inline void operator+=(const size_t& n) {
        ptr = (ptr + n >= pEnd) ? pBegin + (ptr + n - pEnd) : ptr + n;
    }
    inline QueueIterator operator-(const size_t& n) const {
        return QueueIterator{*this, (ptr < pBegin + n) ? pEnd - (pBegin + n - ptr) : ptr - n};
    }
    inline void operator-=(const size_t& n) {
        ptr = (ptr < pBegin + n) ? pEnd - (pBegin + n - ptr) : ptr - n;
    }

    inline bool operator==(const QueueIterator& other) const {
        return ptr == other.ptr;
    }

    const size_t size;

private:
    Type* ptr;
    Type* pBegin;
    Type* pEnd;
};

template<typename Type>
struct QueueConstIterator {
    template<size_t Extent>
    explicit QueueConstIterator(const Queue<Type, Extent>& queue, const Type* _ptr) :
    size(queue._size), ptr(_ptr), pBegin(queue._data), pEnd(queue._data + queue.extent) {}
    QueueConstIterator(const QueueConstIterator&) = default;
    QueueConstIterator(const QueueConstIterator<Type>& other, const Type* _ptr) :
    size(other.size), ptr(_ptr), pBegin(other.pBegin), pEnd(other.pEnd) {}

    inline const Type& operator*() const { return *ptr; }

    inline QueueConstIterator& operator++() {
        if(++ptr == pEnd) {
            ptr = pBegin;
        }

        return *this;
    }
    inline QueueConstIterator operator++(int) {
        QueueConstIterator tmp(*this);
        operator++();
        return tmp;
    }
    inline QueueConstIterator& operator--() {
        if(--ptr < pBegin) {
            ptr = pEnd - 1U;
        }

        return *this;
    }
    inline QueueConstIterator operator--(int) {
        QueueConstIterator tmp(*this);
        operator--();
        return tmp;
    }

    inline QueueConstIterator operator+(const size_t& n) const {
        return QueueConstIterator{*this, (ptr + n >= pEnd) ? pBegin + (ptr + n - pEnd) : ptr + n};
    }
    inline void operator+=(const size_t& n) {
        ptr = (ptr + n >= pEnd) ? pBegin + (ptr + n - pEnd) : ptr + n;
    }
    inline QueueConstIterator operator-(const size_t& n) const {
        return QueueConstIterator{*this, (ptr < pBegin + n) ? pEnd - (pBegin + n - ptr) : ptr - n};
    }
    inline void operator-=(const size_t& n) {
        ptr = (ptr < pBegin + n) ? pEnd - (pBegin + n - ptr) : ptr - n;
    }

    inline bool operator==(const QueueConstIterator& other) const {
        return ptr == other.ptr;
    }

    const size_t size;

private:
    const Type* ptr;
    const Type* const pBegin;
    const Type* const pEnd;
};

}; // namespace ns