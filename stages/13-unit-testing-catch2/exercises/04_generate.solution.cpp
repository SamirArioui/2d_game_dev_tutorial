// Exercise 04 — solution
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
    // The case body re-runs once per generated value. This is a PROPERTY test:
    // whatever the input, the output must be inside the range.
    auto value = GENERATE(-100.0f, -1.0f, 0.0f, 4.0f, 10.0f, 11.0f, 999.0f);
    CAPTURE(value);  // if a REQUIRE fails, Catch2 prints which `value` it was
    REQUIRE(clamp(value, 0.0f, 10.0f) >= 0.0f);
    REQUIRE(clamp(value, 0.0f, 10.0f) <= 10.0f);
}

struct ClampCase {
    float value;
    float expected;
};

TEST_CASE("clamp maps specific inputs to expected outputs", "[ex04]") {
    // GENERATE also works over custom structs: each is one parameterized case.
    auto c = GENERATE(ClampCase{-10.0f, 0.0f}, ClampCase{0.0f, 0.0f},
                      ClampCase{5.0f, 5.0f}, ClampCase{10.0f, 10.0f},
                      ClampCase{25.0f, 10.0f});
    CAPTURE(c.value);
    REQUIRE(clamp(c.value, 0.0f, 10.0f) == Catch::Approx(c.expected));
}
