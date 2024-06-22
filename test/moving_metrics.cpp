#include <nested-shaper/MovingMetrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

template<typename Type>
struct EmptyMetrics {
    Type operator()(const Type& popped, const Type& pushed, const Type& mean, const size_t& size) const {
        (void)popped;
        (void)pushed;
        (void)size;
        return mean;
    }
    template<typename Iterator>
    Type operator()(Iterator begin, Iterator end, const Type& mean, const size_t& size) const {
        (void)begin;
        (void)end;
        (void)size;
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