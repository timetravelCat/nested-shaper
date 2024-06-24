/**
 * @file tiny_utility.hpp
 * 
 * A template based minimal utility classes.
 */

#pragma once

#include <stddef.h>

namespace ns {
/**
 * @class pair
 */
template<typename T1, typename T2>
struct pair {
    /**
     * Members
     */
    T1 first;
    T2 second;

    /**
     * Constructors
     */
    pair() :
    first(), second(){};
    pair(const T1& _first, const T2& _second) :
    first(_first), second(_second){};
    pair(const pair<T1, T2>& other) :
    first(other.first), second(other.second){};

    /**
     * Assignment operator
     */
    pair<T1, T2>& operator=(const pair<T1, T2>& other) {
        if(this == &other) {
            return *this;
        }

        first = other.first;
        second = other.second;
        return *this;
    };
};
template<typename T1, typename T2>
pair<T1, T2> make_pair(const T1& first, const T2& second) {
    return pair<T1, T2>(first, second);
};

/**
 * @class array
 */
template<typename T, size_t N>
struct array {
    /**
     * Members
     */
    T data[N]{0};

    /**
     * Subscript operator
     */
    T& operator[](size_t i) {
        return data[i];
    };
    const T& operator[](size_t i) const {
        return data[i];
    };

    /**
     * size
     */
    static constexpr size_t size() {
        return N;
    };
};
}; // namespace ns