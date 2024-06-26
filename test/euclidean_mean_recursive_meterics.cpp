#include <nested-shaper/metrics/euclidean_mean_recursive_metrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("EuclideanMeanRecursiveMetrics") {
    Queue<float, 3> q;
    q.push(1.0f);
    q.push(1.0f);
    q.push(1.0f);

    EuclideanMeanRecursiveMetrics<float> emcm;
    float mean = emcm.operator()(1.0f, 1.0f, 4.0f, q.forwardConstIterator(), q.backwardConstIterator()); // 1, 1, 4
    REQUIRE(mean == 2.0f);
    mean = emcm.operator()(2.0f, 1.0f, 4.0f, q.forwardConstIterator(), q.backwardConstIterator()); //1 , 4, 4
    REQUIRE(mean == 3.0f);
}