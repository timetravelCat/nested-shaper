#include <nested-shaper/metrics/angle_mean_recursive_metrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("AngleMeanRecursiveMetrics") {
    Queue<float, 3> q;
    q.push(0.1f);
    q.push(0.1f);
    q.push(0.1f);

    AngleMeanRecursiveMetrics<float> amrm{};

    float mean = amrm(0.1f, 0.1f, 0.1f, q.forwardConstIterator(), q.backwardConstIterator());
    REQUIRE(mean == 0.1f);
    mean = amrm(mean, 0.1f, 0.7f + 2.0f * M_PIf, q.forwardConstIterator(), q.backwardConstIterator());
    REQUIRE(fabs(mean - 0.3f) < 1e-4f);
}