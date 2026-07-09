// Exercise 05 — Reflect a point off the window edges (medium) — Catch2 drill.
//
// The ball's bounce is pure logic too, so we test it without a window. Model a
// 1D-per-axis bounce: given a position, a velocity, a radius and the wall
// coordinates [0, size], if the object has crossed a wall, snap it back to the
// wall and reverse that velocity component.
//
// Remember SFML's axes: 0 is the top/left edge, `size` is the bottom/right edge.
//
// TODO: implement reflect_axis so it returns the corrected {pos, vel} pair.
//   - if pos - radius < 0      -> pos = radius,        vel = -vel
//   - if pos + radius > size   -> pos = size - radius, vel = -vel
//   - otherwise leave both unchanged
//
// Build & test:
//   cmake --build build --target 05_reflect_edges && ./build/05_reflect_edges

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <utility>  // std::pair

// Returns the (possibly corrected) {position, velocity} for one axis.
std::pair<float, float> reflect_axis(float pos, float vel, float radius, float size) {
    // TODO: replace this stub with the reflection logic described above.
    return {pos, vel};
}

TEST_CASE("crossing the low edge snaps in and reverses velocity") {
    auto [pos, vel] = reflect_axis(5.0f, -100.0f, 10.0f, 800.0f);
    REQUIRE(pos == Catch::Approx(10.0f));   // snapped to the radius
    REQUIRE(vel == Catch::Approx(100.0f));  // reversed
}

TEST_CASE("crossing the high edge snaps in and reverses velocity") {
    auto [pos, vel] = reflect_axis(795.0f, 100.0f, 10.0f, 800.0f);
    REQUIRE(pos == Catch::Approx(790.0f));   // size - radius
    REQUIRE(vel == Catch::Approx(-100.0f));
}

TEST_CASE("a point safely inside is left untouched") {
    auto [pos, vel] = reflect_axis(400.0f, 100.0f, 10.0f, 800.0f);
    REQUIRE(pos == Catch::Approx(400.0f));
    REQUIRE(vel == Catch::Approx(100.0f));
}
