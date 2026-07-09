// Exercise 05 — Reflect a point off the window edges (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <utility>

std::pair<float, float> reflect_axis(float pos, float vel, float radius, float size) {
    if (pos - radius < 0.0f) {          // crossed the low edge (top / left)
        pos = radius;
        vel = -vel;
    } else if (pos + radius > size) {   // crossed the high edge (bottom / right)
        pos = size - radius;
        vel = -vel;
    }
    return {pos, vel};
}

TEST_CASE("crossing the low edge snaps in and reverses velocity") {
    auto [pos, vel] = reflect_axis(5.0f, -100.0f, 10.0f, 800.0f);
    REQUIRE(pos == Catch::Approx(10.0f));
    REQUIRE(vel == Catch::Approx(100.0f));
}

TEST_CASE("crossing the high edge snaps in and reverses velocity") {
    auto [pos, vel] = reflect_axis(795.0f, 100.0f, 10.0f, 800.0f);
    REQUIRE(pos == Catch::Approx(790.0f));
    REQUIRE(vel == Catch::Approx(-100.0f));
}

TEST_CASE("a point safely inside is left untouched") {
    auto [pos, vel] = reflect_axis(400.0f, 100.0f, 10.0f, 800.0f);
    REQUIRE(pos == Catch::Approx(400.0f));
    REQUIRE(vel == Catch::Approx(100.0f));
}
