// Exercise 04 — semi-implicit Euler integration (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

struct Vec2 {
    float x;
    float y;
};

struct Body {
    Vec2 pos;
    Vec2 vel;
};

void step_semi_implicit(Body& body, const Vec2& accel, float dt) {
    body.vel.x += accel.x * dt;   // velocity first ...
    body.vel.y += accel.y * dt;
    body.pos.x += body.vel.x * dt;  // ... then move with the UPDATED velocity
    body.pos.y += body.vel.y * dt;
}

TEST_CASE("one gravity step updates velocity then moves with it") {
    Body b{Vec2{0.0f, 0.0f}, Vec2{0.0f, 0.0f}};
    step_semi_implicit(b, Vec2{0.0f, 10.0f}, 1.0f);
    REQUIRE(b.vel.y == Catch::Approx(10.0f));
    REQUIRE(b.pos.y == Catch::Approx(10.0f));
}

TEST_CASE("horizontal velocity is unaffected by vertical gravity") {
    Body b{Vec2{0.0f, 0.0f}, Vec2{5.0f, 0.0f}};
    step_semi_implicit(b, Vec2{0.0f, 10.0f}, 0.5f);
    REQUIRE(b.vel.x == Catch::Approx(5.0f));
    REQUIRE(b.pos.x == Catch::Approx(2.5f));
}
