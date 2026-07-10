// ============================================================================
// Tests for the vector free functions added this stage (vec2.hpp).
//
// These functions have exact, known answers, so testing them is just stating
// the maths. No window, no SFML — that's the payoff of keeping vec2.hpp pure.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "vec2.hpp"

using gmath::Vec2f;

TEST_CASE("dot product", "[vec2][dot]") {
    REQUIRE(gmath::dot(Vec2f{3.0f, 4.0f}, Vec2f{2.0f, 1.0f}) == Catch::Approx(10.0f));
    // Perpendicular vectors have a zero dot product.
    REQUIRE(gmath::dot(Vec2f{1.0f, 0.0f}, Vec2f{0.0f, 1.0f}) == Catch::Approx(0.0f));
    // Opposed vectors are negative.
    REQUIRE(gmath::dot(Vec2f{1.0f, 0.0f}, Vec2f{-1.0f, 0.0f}) == Catch::Approx(-1.0f));
}

TEST_CASE("length and length_squared", "[vec2][length]") {
    // The 3-4-5 triangle: sqrt(3² + 4²) == 5.
    REQUIRE(gmath::length(Vec2f{3.0f, 4.0f}) == Catch::Approx(5.0f));
    REQUIRE(gmath::length_squared(Vec2f{3.0f, 4.0f}) == Catch::Approx(25.0f));
    REQUIRE(gmath::length(Vec2f{0.0f, 0.0f}) == Catch::Approx(0.0f));
}

TEST_CASE("normalize yields a unit vector in the same direction", "[vec2][normalize]") {
    Vec2f n = gmath::normalize(Vec2f{0.0f, 5.0f});
    REQUIRE(gmath::length(n) == Catch::Approx(1.0f));
    REQUIRE(n.x == Catch::Approx(0.0f));
    REQUIRE(n.y == Catch::Approx(1.0f));

    SECTION("a diagonal normalizes to 1/sqrt2 components") {
        Vec2f d = gmath::normalize(Vec2f{10.0f, 10.0f});
        REQUIRE(gmath::length(d) == Catch::Approx(1.0f));
        REQUIRE(d.x == Catch::Approx(d.y));
    }

    SECTION("the zero vector normalizes to zero (no NaN)") {
        Vec2f z = gmath::normalize(Vec2f{0.0f, 0.0f});
        REQUIRE(z == Vec2f{0.0f, 0.0f});
    }
}

TEST_CASE("distance is symmetric and matches Pythagoras", "[vec2][distance]") {
    REQUIRE(gmath::distance(Vec2f{0.0f, 0.0f}, Vec2f{3.0f, 4.0f}) == Catch::Approx(5.0f));
    REQUIRE(gmath::distance(Vec2f{1.0f, 1.0f}, Vec2f{1.0f, 1.0f}) == Catch::Approx(0.0f));
    REQUIRE(gmath::distance(Vec2f{2.0f, 5.0f}, Vec2f{7.0f, 1.0f}) ==
            Catch::Approx(gmath::distance(Vec2f{7.0f, 1.0f}, Vec2f{2.0f, 5.0f})));
}

TEST_CASE("lerp blends endpoints", "[vec2][lerp]") {
    SECTION("scalar lerp") {
        REQUIRE(gmath::lerp(0.0f, 10.0f, 0.0f) == Catch::Approx(0.0f));
        REQUIRE(gmath::lerp(0.0f, 10.0f, 1.0f) == Catch::Approx(10.0f));
        REQUIRE(gmath::lerp(0.0f, 10.0f, 0.5f) == Catch::Approx(5.0f));
    }
    SECTION("vector lerp is component-wise") {
        Vec2f a{0.0f, 0.0f};
        Vec2f b{100.0f, 200.0f};
        REQUIRE(gmath::lerp(a, b, 0.0f) == a);
        REQUIRE(gmath::lerp(a, b, 1.0f) == b);
        Vec2f mid = gmath::lerp(a, b, 0.5f);
        REQUIRE(mid.x == Catch::Approx(50.0f));
        REQUIRE(mid.y == Catch::Approx(100.0f));
    }
}
