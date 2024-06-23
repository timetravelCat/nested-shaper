#include <nested-shaper/MovingMetrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

template<typename Type>
struct EmptyMetrics {
    Type operator()(const Type& mean,
                    const Type& popped,
                    const Type& pushed,
                    QueueConstIterator<Type> forwardIterator,
                    QueueConstIterator<Type> backwardIterator) const {
        (void)mean;
        (void)popped;
        (void)pushed;
        (void)forwardIterator;
        (void)backwardIterator;
        return mean;
    }
};

TEST_CASE("MovingMetrics") {
    SECTION("Constructor") {
        MovingMetrics<int, 4, EmptyMetrics<int>> mm1{11};
        REQUIRE(mm1.capacity() == 4);
        REQUIRE(mm1.convolute(11) == 11);

        MovingMetrics<int, 8, EmptyMetrics<int>> mm2(2, 5);
        REQUIRE(mm2.capacity() == 5);
        REQUIRE(mm2.convolute(2) == 2);
    }

    SECTION("Initialize") {
        MovingMetrics<int, 4, EmptyMetrics<int>> mm1{11};
        mm1.initialize(22);
        REQUIRE(mm1.convolute(22) == 22);

        MovingMetrics<int, 8, EmptyMetrics<int>> mm2(2, 5);
        mm2.initialize(33, 5);
        REQUIRE(mm2.capacity() == 5);
        REQUIRE(mm2.convolute(33) == 33);
    }
}

TEST_CASE("MovingMetricsNested") {
    SECTION("Constructor") {
        MovingMetricsNested<int, EmptyMetrics<int>, 4, 8> mmn1{11};
        REQUIRE(mmn1.moving_metrics.capacity() == 4);
        REQUIRE(mmn1.moving_metrics_nested.moving_metrics.capacity() == 8);
        REQUIRE(mmn1.convolute(11) == 11);

        MovingMetricsNested<int, EmptyMetrics<int>, 4, 8> mmn2{8, 2U, 5U};
        REQUIRE(mmn2.moving_metrics.capacity() == 2);
        REQUIRE(mmn2.moving_metrics_nested.moving_metrics.capacity() == 5);
        REQUIRE(mmn2.convolute(8) == 8);
    }

    SECTION("Initialize") {
        MovingMetricsNested<int, EmptyMetrics<int>, 4, 8> mmn1{11};
        mmn1.initialize(22);
        REQUIRE(mmn1.convolute(22) == 22);

        MovingMetricsNested<int, EmptyMetrics<int>, 4, 8> mmn2{8, 2U, 5U};
        mmn2.initialize(33, 2U, 7U);
        REQUIRE(mmn2.moving_metrics.capacity() == 2);
        REQUIRE(mmn2.moving_metrics_nested.moving_metrics.capacity() == 7);
        REQUIRE(mmn2.convolute(33) == 33);
    }
}