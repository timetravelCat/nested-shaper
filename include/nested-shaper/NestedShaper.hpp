/**
 * @file NestedShaper.hpp
 * 
 * @brief This file contains the definition of the NestedShaper class.
 * 
 * @author timetravelCat <timetraveler930@gmail.com>
 */

#pragma once

#include "version.hpp"
#include "metrics/euclidean_derivative_metrics.hpp"
#include "metrics/euclidean_mean_cumulative_metrics.hpp"
#include "metrics/euclidean_mean_recursive_metrics.hpp"
#include "metrics/angle_derivative_metrics.hpp"
#include "metrics/angle_mean_cumulative_metrics.hpp"
#include "metrics/angle_mean_recursive_metrics.hpp"
#include "ShaperMetrics.hpp"

namespace ns {
template<typename Type, size_t DerivativeOrder, size_t... Extents>
using NestedShaperEuclideanCumulative = ShaperMetrics<Type, EuclideanDerivativeMetrics<Type, DerivativeOrder>, DerivativeOrder, EuclideanMeanCumulativeMetrics<Type>, Extents...>;
template<typename Type, size_t DerivativeOrder, size_t... Extents>
using NestedShaperEuclideanRecursive = ShaperMetrics<Type, EuclideanDerivativeMetrics<Type, DerivativeOrder>, DerivativeOrder, EuclideanMeanRecursiveMetrics<Type>, Extents...>;
template<typename Type, size_t DerivativeOrder, size_t... Extents>
using NestedShaperEuclidean = NestedShaperEuclideanRecursive<Type, DerivativeOrder, Extents...>;

template<typename Type, size_t Dimension, size_t DerivativeOrder, size_t... Extents>
using NestedShaperEuclideanCumulativeArray = ShaperMetrics<array<Type, Dimension>, EuclideanDerivativeMetricsArray<Type, Dimension, DerivativeOrder>, DerivativeOrder, EuclideanMeanCumulativeMetricsArray<Type, Dimension>, Extents...>;
template<typename Type, size_t Dimension, size_t DerivativeOrder, size_t... Extents>
using NestedShaperEuclideanRecursiveArray = ShaperMetrics<array<Type, Dimension>, EuclideanDerivativeMetricsArray<Type, Dimension, DerivativeOrder>, DerivativeOrder, EuclideanMeanRecursiveMetricsArray<Type, Dimension>, Extents...>;
template<typename Type, size_t Dimension, size_t DerivativeOrder, size_t... Extents>
using NestedShaperEuclideanArray = NestedShaperEuclideanRecursiveArray<array<Type, Dimension>, Dimension, DerivativeOrder, Extents...>;

template<typename Type, size_t DerivativeOrder, size_t... Extents>
using NestedShaperAngleCumulative = ShaperMetrics<Type, AngleDerivativeMetrics<Type, DerivativeOrder>, DerivativeOrder, AngleMeanCumulativeMetrics<Type>, Extents...>;
template<typename Type, size_t DerivativeOrder, size_t... Extents>
using NestedShaperAngleRecursive = ShaperMetrics<Type, AngleDerivativeMetrics<Type, DerivativeOrder>, DerivativeOrder, AngleMeanRecursiveMetrics<Type>, Extents...>;
template<typename Type, size_t DerivativeOrder, size_t... Extents>
using NestedShaperAngle = NestedShaperAngleRecursive<Type, DerivativeOrder, Extents...>;

template<typename Type, size_t Dimension, size_t DerivativeOrder, size_t... Extents>
using NestedShaperAngleCumulativeArray = ShaperMetrics<array<Type, Dimension>, AngleDerivativeMetricsArray<Type, Dimension, DerivativeOrder>, DerivativeOrder, AngleMeanCumulativeMetricsArray<Type, Dimension>, Extents...>;
template<typename Type, size_t Dimension, size_t DerivativeOrder, size_t... Extents>
using NestedShaperAngleRecursiveArray = ShaperMetrics<array<Type, Dimension>, AngleDerivativeMetricsArray<Type, Dimension, DerivativeOrder>, DerivativeOrder, AngleMeanRecursiveMetricsArray<Type, Dimension>, Extents...>;
template<typename Type, size_t Dimension, size_t DerivativeOrder, size_t... Extents>
using NestedShaperAngleArray = NestedShaperAngleRecursiveArray<array<Type, Dimension>, Dimension, DerivativeOrder, Extents...>;
}; // namespace ns