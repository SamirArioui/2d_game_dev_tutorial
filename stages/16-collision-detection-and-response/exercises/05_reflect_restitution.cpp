// Exercise 05 — bounce response with restitution (medium) — a Catch2 drill.
//
// Detection told us WHERE we hit; response decides how the velocity changes. To
// bounce a velocity v off a surface with outward unit normal n:
//
//   v' = v - (1 + e) * dot(v, n) * n
//
// e is the coefficient of RESTITUTION (bounciness): e=1 is a perfect elastic
// bounce (reverses the normal component, keeps the tangential one), e=0 kills the
// normal component (no bounce). Only reflect when the object is moving INTO the
// surface (dot(v,n) < 0), else it can stick and jitter.
//
// TODO: implement reflect_velocity().
//
// Build & test:
//   cmake --build build --target 05_reflect_restitution && ./build/05_reflect_restitution

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
    // TODO:
    //   vn = dot(v, n);
    //   if vn >= 0 return v;                 // already moving away
    //   return v - n * ((1 + e) * vn);
    (void)v;
    (void)n;
    (void)e;
    return Vec2{0.0f, 0.0f};
}

TEST_CASE("perfect bounce reverses the normal component only") {
    Vec2 r = reflect_velocity(Vec2{100, 40}, Vec2{-1, 0}, 1.0f);  // right-hand wall
    REQUIRE(r.x == Catch::Approx(-100.0f));  // reversed
    REQUIRE(r.y == Catch::Approx(40.0f));    // tangential untouched
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
