#include <nested-shaper/metrics/euclidean_mean_cumulative_metrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("EuclideanMeanCumulativeMetrics") {
    Queue<float, 3> q;
    q.push(10.0f);
    q.push(20.0f);
    q.push(30.0f);

    const float dummy{0.0f};
    EuclideanMeanCumulativeMetrics<float> emcm;
    for(size_t i = 0; i < 10; i++) {
        q.push(float(i + 4U) * 10.0f);
        const float mean = emcm.operator()(dummy, dummy, dummy, q.forwardConstIterator(), q.backwardConstIterator());
        REQUIRE_THAT(mean, Catch::Matchers::WithinRel(float(10 * (i + 3)), 1e-4f));
    }

    Queue<array<float, 3>, 3> qa;
    qa.push(array<float, 3>{10.0f, 20.0f, 30.0f});
    qa.push(array<float, 3>{20.0f, 30.0f, 40.0f});
    qa.push(array<float, 3>{30.0f, 40.0f, 50.0f});

    const array<float, 3> dummyArray{0.0f, 0.0f, 0.0f};
    EuclideanMeanCumulativeMetricsArray<float, 3> emcma;
    for(size_t i = 0; i < 10; i++) {
        qa.push(array<float, 3>{float(i + 4U) * 10.0f, float(i + 5U) * 10.0f, float(i + 6U) * 10.0f});
        const array<float, 3> mean = emcma.operator()(dummyArray, dummyArray, dummyArray, qa.forwardConstIterator(), qa.backwardConstIterator());
        REQUIRE_THAT(mean[0], Catch::Matchers::WithinRel(float(10 * (i + 3)), 1e-4f));
        REQUIRE_THAT(mean[1], Catch::Matchers::WithinRel(float(10 * (i + 4)), 1e-4f));
        REQUIRE_THAT(mean[2], Catch::Matchers::WithinRel(float(10 * (i + 5)), 1e-4f));
    }
}