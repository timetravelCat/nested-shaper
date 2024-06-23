#include <nested-shaper/ShaperMetrics.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

template<typename Type>
struct EmptyMeanMetrics {
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

template<typename Type, typename TimeType>
struct EmptyDerivativeMetrics {
    Type* operator()(QueueConstIterator<Type> forwardIterator,
                     QueueConstIterator<Type> backwardIterator,
                     const TimeType& dt) const {
        (void)forwardIterator;
        (void)backwardIterator;
        (void)dt;
        return nullptr;
    }
};

TEST_CASE("ShaperMetrics.hpp") {
    SECTION("Constructor") {
        ShaperMetrics<int, EmptyDerivativeMetrics<int, float>, 4, EmptyMeanMetrics<int>, 7, 3, 11> sm1{71};
        REQUIRE(sm1.capacity() == 4);
        REQUIRE(sm1.convolute(71, 0.1f) == nullptr);

        ShaperMetrics<int, EmptyDerivativeMetrics<int, float>, 5, EmptyMeanMetrics<int>, 7, 3, 11> sm2{71, 5U, 2U, 8U};
        REQUIRE(sm2.capacity() == 5);
        REQUIRE(sm2.convolute(71, 0.1f) == nullptr);
    }

    SECTION("Initialize") {
        ShaperMetrics<int, EmptyDerivativeMetrics<int, float>, 4, EmptyMeanMetrics<int>, 7, 3, 11> sm1{71};
        sm1.initialize(72);
        REQUIRE(sm1.convolute(72, 0.1f) == nullptr);

        ShaperMetrics<int, EmptyDerivativeMetrics<int, float>, 5, EmptyMeanMetrics<int>, 7, 3, 11> sm2{71, 5U, 2U, 8U};
        sm2.initialize(73, 5U, 2U, 8U);
        REQUIRE(sm2.capacity() == 5);
        REQUIRE(sm2.convolute(73, 0.1f) == nullptr);
    }
}