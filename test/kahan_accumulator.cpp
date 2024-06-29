#include <nested-shaper/metrics/kahan_accumulator.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("KahanBabushkaNeumaierAccumulator") {
    KahanBabushkaNeumaierAccumulator<double> accumulator{};

    double sum = 0.0;
    sum = accumulator.accumulate(sum, 1.0);
    REQUIRE(fabs(sum - 1.0) < 1e-10);
    sum = accumulator.accumulate(sum, 1.0);
    REQUIRE(fabs(sum - 2.0) < 1e-10);
    sum = accumulator.accumulate(sum, 1.0);
    REQUIRE(fabs(sum - 3.0) < 1e-10);
}