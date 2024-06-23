#include <nested-shaper/ShaperMetrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

template<typename Type>
struct EmptyMeanMetrics {
    template<typename Iterator>
    Type operator()(const Type& mean, const Type& popped, const Type& pushed, Iterator begin, Iterator end, const size_t& size) const {
        (void)mean;
        (void)popped;
        (void)pushed;
        (void)begin;
        (void)end;
        (void)size;
        return mean;
    }
};

template<typename Type>
struct EmptyDerivateMetrics {
    template<typename Iterator>
    Type* operator()(Iterator begin, Iterator end, const size_t& size) const {
        (void)begin;
        (void)end;
        (void)size;
        return nullptr;
    }
};

TEST_CASE("ShaperMetrics.hpp") {
    SECTION("Constructor") {
        ShaperMetrics<int, EmptyDerivateMetrics<int>, 4, EmptyMeanMetrics<int>, 7, 3, 11> sm1{71};
        REQUIRE(sm1.capacity() == 4);
        REQUIRE(sm1.convolute(71) == nullptr);

        ShaperMetrics<int, EmptyDerivateMetrics<int>, 5, EmptyMeanMetrics<int>, 7, 3, 11> sm2{71, 5U, 2U, 8U};
        REQUIRE(sm2.capacity() == 5);
        REQUIRE(sm2.convolute(71) == nullptr);
    }

    SECTION("Initialize") {
        ShaperMetrics<int, EmptyDerivateMetrics<int>, 4, EmptyMeanMetrics<int>, 7, 3, 11> sm1{71};
        sm1.initialize(72);
        REQUIRE(sm1.convolute(72) == nullptr);

        ShaperMetrics<int, EmptyDerivateMetrics<int>, 5, EmptyMeanMetrics<int>, 7, 3, 11> sm2{71, 5U, 2U, 8U};
        sm2.initialize(73, 5U, 2U, 8U);
        REQUIRE(sm2.capacity() == 5);
        REQUIRE(sm2.convolute(73) == nullptr);
    }
}