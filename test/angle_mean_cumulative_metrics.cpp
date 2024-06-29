#include <nested-shaper/metrics/angle_mean_cumulative_metrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("AngleMeanCumulativeMetrics") {
    Queue<float, 3> q;
    q.push(-M_PIf / 6.0f + M_PIf * 2.0f);
    q.push(0.0f);
    q.push(M_PIf / 6.0f);

    AngleMeanCumulativeMetrics<float> amcm;
    float mean = amcm.operator()(0.0f, 0.0f, 0.0f, q.forwardConstIterator(), q.backwardConstIterator());
    REQUIRE(fabs(mean) < 1e-4f);

    q.push(M_PIf / 6.0f);
    q.push(M_PIf / 6.0f);
    q.push(M_PIf / 6.0f);
    mean = amcm.operator()(0.0f, 0.0f, 0.0f, q.forwardConstIterator(), q.backwardConstIterator());
    REQUIRE(fabs(mean - M_PIf / 6.0f) < 1e-4f);

    Queue<array<float, 3>, 3> qa;
    qa.push(array<float, 3>{-M_PIf / 6.0f + M_PIf * 2.0f, 0.0f, M_PIf / 6.0f});
    qa.push(array<float, 3>{0.0f, M_PIf / 6.0f, M_PIf / 6.0f});
    qa.push(array<float, 3>{M_PIf / 6.0f, M_PIf / 6.0f, M_PIf / 6.0f});

    AngleMeanCumulativeMetricsArray<float, 3> amcma;
    array<float, 3> dummyArray{0.0f, 0.0f, 0.0f};
    array<float, 3> meanArray = amcma.operator()(dummyArray, dummyArray, dummyArray, qa.forwardConstIterator(), qa.backwardConstIterator());
    REQUIRE(fabs(meanArray[0]) < 1e-4f);
    REQUIRE(fabs(meanArray[1] - M_PIf / 9.0f) < 1e-2f);
    REQUIRE(fabs(meanArray[2] - M_PIf / 6.0f) < 1e-4f);
}