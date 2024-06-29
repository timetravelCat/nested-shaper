#include <nested-shaper/metrics/angle_derivative_metrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("AngleDerivativeMetrics") {
    Queue<float, 1> q_1;
    q_1.push(1.0f);
    AngleDerivativeMetrics<float, 1> edm_1;
    array<float, 1> derivatives_1 = edm_1.operator()(q_1.forwardConstIterator(), q_1.backwardConstIterator(), 66.0f);
    REQUIRE_THAT(derivatives_1[0], Catch::Matchers::WithinRel(1.0f, 1e-4f));

    Queue<float, 3> q_3;
    q_3.push(0.1f);
    q_3.push(0.2f + 2.0f * M_PIf);
    q_3.push(0.4f);
    AngleDerivativeMetrics<float, 3> edm_3;
    array<float, 3> derivatives_3 = edm_3.operator()(q_3.forwardConstIterator(), q_3.backwardConstIterator(), 0.1f);
    REQUIRE_THAT(derivatives_3[0], Catch::Matchers::WithinRel(0.20f, 1e-4f));
    REQUIRE_THAT(derivatives_3[1], Catch::Matchers::WithinRel(1.5f, 1e-4f));

    using array_type = array<float, 1>;
    Queue<array_type, 1> q_1_1;
    q_1_1.push(array_type{1.0f});
    AngleDerivativeMetricsArray<float, 1, 1> edm_1_1;
    array<array_type, 1> derivatives_1_1 = edm_1_1.operator()(q_1_1.forwardConstIterator(), q_1_1.backwardConstIterator(), 0.1f);
    REQUIRE_THAT(derivatives_1_1[0][0], Catch::Matchers::WithinRel(1.0f, 1e-4f));

    Queue<array<float, 3>, 3> q_3_3;
    q_3_3.push(array<float, 3>{0.0f, 0.1f, 0.2f});
    q_3_3.push(array<float, 3>{0.1f, 0.2f, 0.4f});
    q_3_3.push(array<float, 3>{0.2f, 0.4f, 0.8f});
    AngleDerivativeMetricsArray<float, 3, 3> edm_3_3;
    array<array<float, 3>, 3> derivatives_3_3 = edm_3_3.operator()(q_3_3.forwardConstIterator(), q_3_3.backwardConstIterator(), 0.1f);

    REQUIRE_THAT(derivatives_3_3[0][0], Catch::Matchers::WithinRel(0.1f, 1e-4f));
    REQUIRE_THAT(derivatives_3_3[0][1], Catch::Matchers::WithinRel(1.0f, 1e-4f));

    REQUIRE_THAT(derivatives_3_3[1][0], Catch::Matchers::WithinRel(0.2f, 1e-4f));
    REQUIRE_THAT(derivatives_3_3[1][1], Catch::Matchers::WithinRel(1.5f, 1e-4f));

    REQUIRE_THAT(derivatives_3_3[2][0], Catch::Matchers::WithinRel(0.4f, 1e-4f));
    REQUIRE_THAT(derivatives_3_3[2][1], Catch::Matchers::WithinRel(3.0f, 1e-4f));
}