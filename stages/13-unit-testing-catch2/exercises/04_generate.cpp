// Exercise 04 — parameterized tests with GENERATE
//
// GENERATE feeds a test the SAME assertions across many inputs: Catch2 re-runs
// the case once per generated value. It's Catch2's answer to pytest's
// @pytest.mark.parametrize.
//
// TODO:
//   1. First test: use GENERATE to run the property check for each of these
//      inputs: -100, -1, 0, 4, 10, 11, 999. The clamp result must ALWAYS land
//      in [0, 10]. CAPTURE the input so a failure prints which value broke it.
//        auto value = GENERATE(-100.0f, -1.0f, 0.0f, 4.0f, 10.0f, 11.0f, 999.0f);
//        CAPTURE(value);
//        REQUIRE(clamp(value, 0.0f, 10.0f) >= 0.0f);
//        REQUIRE(clamp(value, 0.0f, 10.0f) <= 10.0f);
//   2. Second test: GENERATE over ClampCase values to check EXACT outputs, e.g.
//        auto c = GENERATE(ClampCase{-10.0f, 0.0f}, ClampCase{5.0f, 5.0f}, ...);
//        CAPTURE(c.value);
//        REQUIRE(clamp(c.value, 0.0f, 10.0f) == Catch::Approx(c.expected));

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

float clamp(float value, float lo, float hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

TEST_CASE("clamp output is always inside the range (property)", "[ex04]") {
    // TODO: GENERATE the inputs, CAPTURE, and REQUIRE the result is within [0,10].
}

struct ClampCase {
    float value;
    float expected;
};

TEST_CASE("clamp maps specific inputs to expected outputs", "[ex04]") {
    // TODO: GENERATE a set of ClampCase values, CAPTURE c.value, and REQUIRE the
    // clamp result equals c.expected (use Catch::Approx for the float compare).
}
