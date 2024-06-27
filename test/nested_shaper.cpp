#include <nested-shaper/NestedShaper.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <nested-shaper/metrics/central_finite_difference_coefficients.hpp>

#include <stdio.h>

using namespace ns;

TEST_CASE("NestedShaper") {
    SECTION("NestedShaperEuclideanCumulative") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        NestedShaperEuclideanCumulative<float, 5, 5, 2, 2> nested_shaper(10.0f); // all values are initialized with 10.0f
        array<float, 5> derivatives = nested_shaper.convolute(10.0f, 0.1f);      // 10.0f is pushed into the queue, dt = 0.1f

        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0] - 10.0f) < 1.0e-3f);
        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[1] - 0.0f) < 1.0e-3f);
        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[2] - 0.0f) < 1.0e-3f);
        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[3] - 0.0f) < 1.0e-3f);
        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(20.0f, 0.1f); // 20.0f is pushed into the queue, dt = 0.1f
        // 5 // 10.0f    10.0f    10.0f    10.0f    20.0f
        // 2 //              10.0f    12.0f
        // 2 //              10.0f    11.0f
        //      10.0f    10.0f    10.0f   10.0f  10.5f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0] - 10.0f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;
        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative{0.0f};

            derivative += coefficient[i][0] * 10.0f;
            derivative += coefficient[i][1] * 10.0f;
            derivative += coefficient[i][2] * 10.0f;
            derivative += coefficient[i][3] * 10.0f;
            derivative += coefficient[i][4] * 10.5f;
            derivative /= dt;
            dt *= 0.1f;

            REQUIRE(fabs(derivatives[i + 1] - derivative) < 1.0e-3f);
        }
    }

    SECTION("NestedShaperEuclideanRecursive") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        NestedShaperEuclideanRecursive<float, 5, 5, 2, 2> nested_shaper(10.0f); // all values are initialized with 10.0f
        array<float, 5> derivatives = nested_shaper.convolute(10.0f, 0.1f);     // 10.0f is pushed into the queue, dt = 0.1f

        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0] - 10.0f) < 1.0e-3f);
        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[1] - 0.0f) < 1.0e-3f);
        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[2] - 0.0f) < 1.0e-3f);
        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[3] - 0.0f) < 1.0e-3f);
        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(20.0f, 0.1f); // 20.0f is pushed into the queue, dt = 0.1f
        // 5 // 10.0f    10.0f    10.0f    10.0f    20.0f
        // 2 //              10.0f    12.0f
        // 2 //              10.0f    11.0f
        //      10.0f    10.0f    10.0f   10.0f  10.5f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0] - 10.0f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;
        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative{0.0f};

            derivative += coefficient[i][0] * 10.0f;
            derivative += coefficient[i][1] * 10.0f;
            derivative += coefficient[i][2] * 10.0f;
            derivative += coefficient[i][3] * 10.0f;
            derivative += coefficient[i][4] * 10.5f;
            derivative /= dt;
            dt *= 0.1f;

            REQUIRE(fabs(derivatives[i + 1] - derivative) < 1.0e-3f);
        }
    }

    SECTION("NestedShaperEuclideanCumulativeArray") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        using array_type = array<float, 3>;
        using derivative_type = array<array<float, 5>, 3>;
        NestedShaperEuclideanCumulativeArray<float, 3, 5, 5, 2, 2> nested_shaper(array_type{10.0f, 20.0f, 30.0f}); // all values are initialized with {10.0f, 20.0f, 30.0f}

        derivative_type derivatives = nested_shaper.convolute(array_type{10.0f, 20.0f, 30.0f}, 0.1f); // {10.0f, 20.0f, 30.0f} is pushed into the queue, dt = 0.1f
        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0][0] - 10.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][0] - 20.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][0] - 30.0f) < 1.0e-3f);

        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[0][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][1] - 0.0f) < 1.0e-3f);

        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[0][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][2] - 0.0f) < 1.0e-3f);

        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[0][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][3] - 0.0f) < 1.0e-3f);

        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[0][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(array_type{20.0f, 30.0f, 40.0f}, 0.1f); // {20.0f, 30.0f, 40.0f} is pushed into the queue, dt = 0.1f
        //                     first axis
        // 5 // 10.0f    10.0f    10.0f    10.0f    20.0f
        // 2 //              10.0f    12.0f
        // 2 //              10.0f    11.0f
        //      10.0f    10.0f    10.0f   10.0f  10.5f (Expected Last Queue Data)

        //                   second axis
        // 5 // 20.0f    20.0f    20.0f    20.0f    30.0f
        // 2 //              20.0f    22.0f
        // 2 //              20.0f    21.0f
        //      20.0f    20.0f    20.0f   20.0f  20.5f (Expected Last Queue Data)

        //                   third axis
        // 5 // 30.0f    30.0f    30.0f    30.0f    40.0f
        // 2 //              30.0f    32.0f
        // 2 //              30.0f    31.0f
        //      30.0f    30.0f    30.0f   30.0f  30.5f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0][0] - 10.0f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[1][0] - 20.0f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[2][0] - 30.0f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;

        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative_first{0.0f};
            float derivative_second{0.0f};
            float derivative_third{0.0f};

            derivative_first += coefficient[i][0] * 10.0f;
            derivative_first += coefficient[i][1] * 10.0f;
            derivative_first += coefficient[i][2] * 10.0f;
            derivative_first += coefficient[i][3] * 10.0f;
            derivative_first += coefficient[i][4] * 10.5f;
            derivative_first /= dt;

            derivative_second += coefficient[i][0] * 20.0f;
            derivative_second += coefficient[i][1] * 20.0f;
            derivative_second += coefficient[i][2] * 20.0f;
            derivative_second += coefficient[i][3] * 20.0f;
            derivative_second += coefficient[i][4] * 20.5f;
            derivative_second /= dt;

            derivative_third += coefficient[i][0] * 30.0f;
            derivative_third += coefficient[i][1] * 30.0f;
            derivative_third += coefficient[i][2] * 30.0f;
            derivative_third += coefficient[i][3] * 30.0f;
            derivative_third += coefficient[i][4] * 30.5f;
            derivative_third /= dt;

            dt *= 0.1f;

            REQUIRE(fabs(derivatives[0][i + 1] - derivative_first) < 1.0e-3f);
            REQUIRE(fabs(derivatives[1][i + 1] - derivative_second) < 1.0e-3f);
            REQUIRE(fabs(derivatives[2][i + 1] - derivative_third) < 1.0e-3f);
        }
    }

    SECTION("NestedShaperEuclideanRecursiveArray") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        using array_type = array<float, 3>;
        using derivative_type = array<array<float, 5>, 3>;
        NestedShaperEuclideanRecursiveArray<float, 3, 5, 5, 2, 2> nested_shaper(array_type{10.0f, 20.0f, 30.0f}); // all values are initialized with {10.0f, 20.0f, 30.0f}

        derivative_type derivatives = nested_shaper.convolute(array_type{10.0f, 20.0f, 30.0f}, 0.1f); // {10.0f, 20.0f, 30.0f} is pushed into the queue, dt = 0.1f
        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0][0] - 10.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][0] - 20.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][0] - 30.0f) < 1.0e-3f);

        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[0][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][1] - 0.0f) < 1.0e-3f);

        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[0][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][2] - 0.0f) < 1.0e-3f);

        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[0][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][3] - 0.0f) < 1.0e-3f);

        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[0][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(array_type{20.0f, 30.0f, 40.0f}, 0.1f); // {20.0f, 30.0f, 40.0f} is pushed into the queue, dt = 0.1f
        //                     first axis
        // 5 // 10.0f    10.0f    10.0f    10.0f    20.0f
        // 2 //              10.0f    12.0f
        // 2 //              10.0f    11.0f
        //      10.0f    10.0f    10.0f   10.0f  10.5f (Expected Last Queue Data)

        //                   second axis
        // 5 // 20.0f    20.0f    20.0f    20.0f    30.0f
        // 2 //              20.0f    22.0f
        // 2 //              20.0f    21.0f
        //      20.0f    20.0f    20.0f   20.0f  20.5f (Expected Last Queue Data)

        //                   third axis
        // 5 // 30.0f    30.0f    30.0f    30.0f    40.0f
        // 2 //              30.0f    32.0f
        // 2 //              30.0f    31.0f
        //      30.0f    30.0f    30.0f   30.0f  30.5f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0][0] - 10.0f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[1][0] - 20.0f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[2][0] - 30.0f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;

        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative_first{0.0f};
            float derivative_second{0.0f};
            float derivative_third{0.0f};

            derivative_first += coefficient[i][0] * 10.0f;
            derivative_first += coefficient[i][1] * 10.0f;
            derivative_first += coefficient[i][2] * 10.0f;
            derivative_first += coefficient[i][3] * 10.0f;
            derivative_first += coefficient[i][4] * 10.5f;
            derivative_first /= dt;

            derivative_second += coefficient[i][0] * 20.0f;
            derivative_second += coefficient[i][1] * 20.0f;
            derivative_second += coefficient[i][2] * 20.0f;
            derivative_second += coefficient[i][3] * 20.0f;
            derivative_second += coefficient[i][4] * 20.5f;
            derivative_second /= dt;

            derivative_third += coefficient[i][0] * 30.0f;
            derivative_third += coefficient[i][1] * 30.0f;
            derivative_third += coefficient[i][2] * 30.0f;
            derivative_third += coefficient[i][3] * 30.0f;
            derivative_third += coefficient[i][4] * 30.5f;
            derivative_third /= dt;

            dt *= 0.1f;

            REQUIRE(fabs(derivatives[0][i + 1] - derivative_first) < 1.0e-3f);
            REQUIRE(fabs(derivatives[1][i + 1] - derivative_second) < 1.0e-3f);
            REQUIRE(fabs(derivatives[2][i + 1] - derivative_third) < 1.0e-3f);
        }
    }

    SECTION("NestedShaperAngleCumulative") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        NestedShaperAngleCumulative<float, 5, 5, 2, 2> nested_shaper(0.1f); // all values are initialized with 0.1f
        array<float, 5> derivatives = nested_shaper.convolute(0.1f, 0.1f);  // 0.1f is pushed into the queue, dt = 0.1f

        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0] - 0.1f) < 1.0e-3f);
        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[1] - 0.0f) < 1.0e-3f);
        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[2] - 0.0f) < 1.0e-3f);
        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[3] - 0.0f) < 1.0e-3f);
        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(0.2f + M_PIf * 2.0f, 0.1f); // 0.2f + M_PIf * 2.0f is pushed into the queue, dt = 0.1f
        // 5 // 0.1f    0.1f    0.1f    0.1f    0.2f
        // 2 //              0.1f    0.12f
        // 2 //              0.1f    0.11f
        //      0.1f    0.1f    0.1f   0.1f  0.105f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0] - 0.1f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;
        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative{0.0f};

            derivative += coefficient[i][0] * 0.1f;
            derivative += coefficient[i][1] * 0.1f;
            derivative += coefficient[i][2] * 0.1f;
            derivative += coefficient[i][3] * 0.1f;
            derivative += coefficient[i][4] * 0.105f;
            derivative /= dt;
            dt *= 0.1f;

            REQUIRE_THAT(derivatives[i + 1], Catch::Matchers::WithinAbs(derivative, 5.0e-2f));
        }
    }

    SECTION("NestedShaperAngleRecursive") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        NestedShaperAngleRecursive<float, 5, 5, 2, 2> nested_shaper(0.1f); // all values are initialized with 0.1f
        array<float, 5> derivatives = nested_shaper.convolute(0.1f, 0.1f); // 0.1f is pushed into the queue, dt = 0.1f

        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0] - 0.1f) < 1.0e-3f);
        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[1] - 0.0f) < 1.0e-3f);
        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[2] - 0.0f) < 1.0e-3f);
        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[3] - 0.0f) < 1.0e-3f);
        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(0.2f + M_PIf * 2.0f, 0.1f); // 0.2f + M_PIf * 2.0f is pushed into the queue, dt = 0.1f
        // 5 // 0.1f    0.1f    0.1f    0.1f    0.2f
        // 2 //              0.1f    0.12f
        // 2 //              0.1f    0.11f
        //      0.1f    0.1f    0.1f   0.1f  0.105f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0] - 0.1f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;
        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative{0.0f};

            derivative += coefficient[i][0] * 0.1f;
            derivative += coefficient[i][1] * 0.1f;
            derivative += coefficient[i][2] * 0.1f;
            derivative += coefficient[i][3] * 0.1f;
            derivative += coefficient[i][4] * 0.105f;
            derivative /= dt;
            dt *= 0.1f;

            REQUIRE_THAT(derivatives[i + 1], Catch::Matchers::WithinAbs(derivative, 5.0e-2f));
        }
    }

    SECTION("NestedShaperAngleCumulativeArray") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        using array_type = array<float, 3>;
        using derivative_type = array<array<float, 5>, 3>;
        NestedShaperAngleCumulativeArray<float, 3, 5, 5, 2, 2> nested_shaper(array_type{0.1f, 0.2f, 0.3f}); // all values are initialized with {10.0f, 20.0f, 30.0f}

        derivative_type derivatives = nested_shaper.convolute(array_type{0.1f, 0.2f, 0.3f}, 0.1f); // {10.0f, 20.0f, 30.0f} is pushed into the queue, dt = 0.1f
        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0][0] - 0.1f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][0] - 0.2f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][0] - 0.3f) < 1.0e-3f);

        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[0][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][1] - 0.0f) < 1.0e-3f);

        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[0][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][2] - 0.0f) < 1.0e-3f);

        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[0][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][3] - 0.0f) < 1.0e-3f);

        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[0][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(array_type{0.2f + M_PIf * 2.0f, 0.3f + M_PIf * 2.0f, 0.4f + M_PIf * 2.0f}, 0.1f); // {0.2f, 0.3f, 0.4f} is pushed into the queue, dt = 0.1f
        //                     first axis
        // 5 // 0.10f    0.10f    0.10f    0.10f    0.20f
        // 2 //              0.10f    0.12f
        // 2 //              0.10f    0.11f
        //      0.10f    0.10f    0.10f   0.10f  0.105f (Expected Last Queue Data)

        //                     second axis
        // 5 // 0.20f    0.20f    0.20f    0.20f    0.30f
        // 2 //              0.20f    0.22f
        // 2 //              0.20f    0.21f
        //      0.20f    0.20f    0.20f   0.20f  0.205f (Expected Last Queue Data)

        //                     third axis
        // 5 // 0.30f    0.30f    0.30f    0.30f    0.40f
        // 2 //              0.30f    0.32f
        // 2 //              0.30f    0.31f
        //      0.30f    0.30f    0.30f   0.30f  0.305f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0][0] - 0.1f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[1][0] - 0.2f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[2][0] - 0.3f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;

        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative_first{0.0f};
            float derivative_second{0.0f};
            float derivative_third{0.0f};

            derivative_first += coefficient[i][0] * 0.1f;
            derivative_first += coefficient[i][1] * 0.1f;
            derivative_first += coefficient[i][2] * 0.1f;
            derivative_first += coefficient[i][3] * 0.1f;
            derivative_first += coefficient[i][4] * 0.105f;
            derivative_first /= dt;

            derivative_second += coefficient[i][0] * 0.200f;
            derivative_second += coefficient[i][1] * 0.200f;
            derivative_second += coefficient[i][2] * 0.200f;
            derivative_second += coefficient[i][3] * 0.200f;
            derivative_second += coefficient[i][4] * 0.205f;
            derivative_second /= dt;

            derivative_third += coefficient[i][0] * 0.300f;
            derivative_third += coefficient[i][1] * 0.300f;
            derivative_third += coefficient[i][2] * 0.300f;
            derivative_third += coefficient[i][3] * 0.300f;
            derivative_third += coefficient[i][4] * 0.305f;
            derivative_third /= dt;

            dt *= 0.1f;

            REQUIRE_THAT(derivatives[0][i + 1], Catch::Matchers::WithinAbs(derivative_first, 5.0e-2f));
            REQUIRE_THAT(derivatives[1][i + 1], Catch::Matchers::WithinAbs(derivative_second, 5.0e-2f));
            REQUIRE_THAT(derivatives[2][i + 1], Catch::Matchers::WithinAbs(derivative_third, 5.0e-2f));
        }
    }

    SECTION("NestedShaperAngleRecursiveArray") {
        // 5 : DerivativeOrder
        // 5, 2, 2 : Extents
        using array_type = array<float, 3>;
        using derivative_type = array<array<float, 5>, 3>;
        NestedShaperAngleRecursiveArray<float, 3, 5, 5, 2, 2> nested_shaper(array_type{0.1f, 0.2f, 0.3f}); // all values are initialized with {10.0f, 20.0f, 30.0f}

        derivative_type derivatives = nested_shaper.convolute(array_type{0.1f, 0.2f, 0.3f}, 0.1f); // {10.0f, 20.0f, 30.0f} is pushed into the queue, dt = 0.1f
        // first value of derivatives is central point
        REQUIRE(fabs(derivatives[0][0] - 0.1f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][0] - 0.2f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][0] - 0.3f) < 1.0e-3f);

        // second value of derivatives is first derivative
        REQUIRE(fabs(derivatives[0][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][1] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][1] - 0.0f) < 1.0e-3f);

        // third value of derivatives is second derivative
        REQUIRE(fabs(derivatives[0][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][2] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][2] - 0.0f) < 1.0e-3f);

        // fourth value of derivatives is third derivative
        REQUIRE(fabs(derivatives[0][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][3] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][3] - 0.0f) < 1.0e-3f);

        // fifth value of derivatives is fourth derivative
        REQUIRE(fabs(derivatives[0][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[1][4] - 0.0f) < 1.0e-3f);
        REQUIRE(fabs(derivatives[2][4] - 0.0f) < 1.0e-3f);

        derivatives = nested_shaper.convolute(array_type{0.2f + M_PIf * 2.0f, 0.3f + M_PIf * 2.0f, 0.4f + M_PIf * 2.0f}, 0.1f); // {0.2f, 0.3f, 0.4f} is pushed into the queue, dt = 0.1f
        //                     first axis
        // 5 // 0.10f    0.10f    0.10f    0.10f    0.20f
        // 2 //              0.10f    0.12f
        // 2 //              0.10f    0.11f
        //      0.10f    0.10f    0.10f   0.10f  0.105f (Expected Last Queue Data)

        //                     second axis
        // 5 // 0.20f    0.20f    0.20f    0.20f    0.30f
        // 2 //              0.20f    0.22f
        // 2 //              0.20f    0.21f
        //      0.20f    0.20f    0.20f   0.20f  0.205f (Expected Last Queue Data)

        //                     third axis
        // 5 // 0.30f    0.30f    0.30f    0.30f    0.40f
        // 2 //              0.30f    0.32f
        // 2 //              0.30f    0.31f
        //      0.30f    0.30f    0.30f   0.30f  0.305f (Expected Last Queue Data)

        REQUIRE(fabs(derivatives[0][0] - 0.1f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[1][0] - 0.2f) < 1.0e-3f); // Central Point
        REQUIRE(fabs(derivatives[2][0] - 0.3f) < 1.0e-3f); // Central Point

        constexpr auto coefficient = CENTRAL_FINITE_DIFFERENCE_COEFFICIENTS<float, 5>::value;

        float dt{0.1f};
        for(size_t i = 0U; i < 4U; i++) {
            float derivative_first{0.0f};
            float derivative_second{0.0f};
            float derivative_third{0.0f};

            derivative_first += coefficient[i][0] * 0.1f;
            derivative_first += coefficient[i][1] * 0.1f;
            derivative_first += coefficient[i][2] * 0.1f;
            derivative_first += coefficient[i][3] * 0.1f;
            derivative_first += coefficient[i][4] * 0.105f;
            derivative_first /= dt;

            derivative_second += coefficient[i][0] * 0.200f;
            derivative_second += coefficient[i][1] * 0.200f;
            derivative_second += coefficient[i][2] * 0.200f;
            derivative_second += coefficient[i][3] * 0.200f;
            derivative_second += coefficient[i][4] * 0.205f;
            derivative_second /= dt;

            derivative_third += coefficient[i][0] * 0.300f;
            derivative_third += coefficient[i][1] * 0.300f;
            derivative_third += coefficient[i][2] * 0.300f;
            derivative_third += coefficient[i][3] * 0.300f;
            derivative_third += coefficient[i][4] * 0.305f;
            derivative_third /= dt;

            dt *= 0.1f;

            REQUIRE_THAT(derivatives[0][i + 1], Catch::Matchers::WithinAbs(derivative_first, 5.0e-2f));
            REQUIRE_THAT(derivatives[1][i + 1], Catch::Matchers::WithinAbs(derivative_second, 5.0e-2f));
            REQUIRE_THAT(derivatives[2][i + 1], Catch::Matchers::WithinAbs(derivative_third, 5.0e-2f));
        }
    }
}