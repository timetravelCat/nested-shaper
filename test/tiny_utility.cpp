#include <nested-shaper/tiny_utility.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace ns;

TEST_CASE("tiny_utility") {
    SECTION("pair") {
        pair<int, float> p1{3, 7.0f};
        REQUIRE(p1.first == 3);
        REQUIRE(p1.second == 7.0f);

        p1 = make_pair(5, 9.0f);
        REQUIRE(p1.first == 5);
        REQUIRE(p1.second == 9.0f);
    }

    SECTION("wrap") {
        float angle = 27.0f;
        angle = wrap(angle, 10.0f, 20.0f);
        REQUIRE(fabs(angle - 17.0f) < 1e-4f);
    }
}