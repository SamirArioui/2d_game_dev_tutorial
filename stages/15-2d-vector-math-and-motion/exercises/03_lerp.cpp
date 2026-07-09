// Exercise 03 — linear interpolation (medium) — a Catch2 drill.
//
// lerp(a, b, t) = a + (b - a) * t. At t=0 you get a, at t=1 you get b, and in
// between you slide straight from one to the other. It is the backbone of smooth
// movement, colour fades, camera follow — everything that "eases" toward a goal.
//
// TODO: implement the scalar lerp and the component-wise vector lerp.
//
// Build & test:
//   cmake --build build --target 03_lerp && ./build/03_lerp

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

struct Vec2 {
    float x;
    float y;
};

float lerp(float a, float b, float t) {
    // TODO: return a blended toward b by t.
    (void)a;
    (void)b;
    (void)t;
    return 0.0f;
}

Vec2 lerp(const Vec2& a, const Vec2& b, float t) {
    // TODO: lerp each component (reuse the scalar lerp above).
    (void)a;
    (void)b;
    (void)t;
    return Vec2{0.0f, 0.0f};
}

TEST_CASE("scalar lerp hits both endpoints and the midpoint") {
    REQUIRE(lerp(0.0f, 10.0f, 0.0f) == Catch::Approx(0.0f));
    REQUIRE(lerp(0.0f, 10.0f, 1.0f) == Catch::Approx(10.0f));
    REQUIRE(lerp(0.0f, 10.0f, 0.5f) == Catch::Approx(5.0f));
    REQUIRE(lerp(10.0f, 20.0f, 0.25f) == Catch::Approx(12.5f));
}

TEST_CASE("vector lerp is component-wise") {
    Vec2 a{0.0f, 0.0f};
    Vec2 b{100.0f, 200.0f};
    Vec2 mid = lerp(a, b, 0.5f);
    REQUIRE(mid.x == Catch::Approx(50.0f));
    REQUIRE(mid.y == Catch::Approx(100.0f));
}
