// Exercise 05 — bounce response with restitution (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

struct Vec2 {
    float x;
    float y;
};

float dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

Vec2 reflect_velocity(const Vec2& v, const Vec2& n, float e) {
    const float vn = dot(v, n);
    if (vn >= 0.0f) {
        return v;  // moving away from (or along) the surface
    }
    const float k = (1.0f + e) * vn;
    return Vec2{v.x - n.x * k, v.y - n.y * k};
}

TEST_CASE("perfect bounce reverses the normal component only") {
    Vec2 r = reflect_velocity(Vec2{100, 40}, Vec2{-1, 0}, 1.0f);
    REQUIRE(r.x == Catch::Approx(-100.0f));
    REQUIRE(r.y == Catch::Approx(40.0f));
}

TEST_CASE("zero restitution kills the normal component") {
    Vec2 r = reflect_velocity(Vec2{100, 40}, Vec2{-1, 0}, 0.0f);
    REQUIRE(r.x == Catch::Approx(0.0f));
    REQUIRE(r.y == Catch::Approx(40.0f));
}

TEST_CASE("half restitution keeps half the normal speed") {
    Vec2 r = reflect_velocity(Vec2{100, 0}, Vec2{-1, 0}, 0.5f);
    REQUIRE(r.x == Catch::Approx(-50.0f));
}

TEST_CASE("an object moving away is left alone") {
    Vec2 r = reflect_velocity(Vec2{-100, 0}, Vec2{-1, 0}, 1.0f);
    REQUIRE(r.x == Catch::Approx(-100.0f));
}
