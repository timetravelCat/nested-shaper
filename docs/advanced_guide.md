# Advanced Guide of **nested-shaper**

nested-shaper supports average metrics on euclidean space and lie algebra space. 

However, if you want to apply other metrics on your custom algebra space, advanced guide explains how to do that.

Also you can apply your array / vector types as input / output.

## Support custom data type / average metrics / algebra space

All default class aliases came from ShaperMetrics.

```cpp
template<typename Type, size_t Dimension, size_t DerivativeOrder, size_t... Extents>
using NestedShaperEuclideanRecursiveArray = ShaperMetrics<array<Type, Dimension>, EuclideanDerivativeMetricsArray<Type, Dimension, DerivativeOrder>, DerivativeOrder, EuclideanMeanRecursiveMetricsArray<Type, Dimension>, Extents...>;
```
**NestedShaperEuclideanRecursiveArray** class is template with 

**array<Type, Dimension\>** as input data type,

**EuclideanDerivativeMetricsArray<Type, Dimension, DerivativeOrder\>** as metrics for calculating derivatives, 

**DerivativeOrder** as queue size of last position data for calculating derivatives,

**EuclideanMeanRecursiveMetricsArray<Type, Dimension\>**  as metrics for calculating averages of each filter.

**Extents...** as queue extents in order to store size of moving average filter.

### Step 1. 
Decide your data type. Let we assume input data type is 3D, **std::array<double, 3\>**.

### Step 2.
Implement a functor, about How to calculate derivatives from queue.

Euclidean 1D implementation [nested-shaper/metrics/euclidean_derivative_metrics.hpp]

```cpp
template<size_t DerivativeOrder>
struct MyDerivativeMetrics {
    MyReturnType operator() (QueueConstIterator<std::array<double, 3\>> forwardIterator, QueueConstIterator<std::array<double, 3\>> backwardIterator, const Type& dt) {
        // MyReturnType will be result type of convolute method.
        // By iterators, you can access last queue data of nested moving average filters.
        // You must use forwardIterator with operator++. or forwardIterator + N
        // You must use backwardIterator with operator--. or forwardIterator - N
    }
};
```
### Step 3.
Implement a functor, about How to calculate average of queue.

Euclidean 1D implementation [nested-shaper/metrics/euclidean_mean_recursive_metrics.hpp]

```cpp
struct MyAverageMetrics {
    std::array<double, 3\> operator()(const std::array<double, 3\>& mean,
                const std::array<double, 3\>& popped,
                const std::array<double, 3\>& pushed,
                QueueConstIterator<std::array<double, 3\>> forwardIterator,
                QueueConstIterator<std::array<double, 3\>> backwardIterator){
    // You can average data, by recursively using previous mean, pushed data and popped data.
    // Or you can average data by cumulative using iterator.
    }
};
```

### Step 4. 
Adapt to ShaperMetrics.

```cpp
template<size_t DerivativeOrder, size_t... Extents>
using MyNestedShaper = ShaperMetrics<std::array<double, 3\>, MyDerivativeMetrics<DerivativeOrder>, DerivativeOrder, MyAverageMetrics, Extents...>;
```