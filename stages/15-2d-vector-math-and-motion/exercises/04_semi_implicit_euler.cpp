// Exercise 04 — semi-implicit Euler integration (medium-hard) — a Catch2 drill.
//
// "Integrating" motion means: given velocity and acceleration, step position and
// velocity forward by dt. The ORDER matters:
//
//   explicit (naive):  pos += vel*dt;  then  vel += accel*dt;   (move with OLD vel)
//   semi-implicit:     vel += accel*dt; then  pos += vel*dt;    (move with NEW vel)
//
// Semi-implicit is one line reordered but far more stable — it's what games use
// for gravity. This drill has you implement it and see it differ from explicit.
//
// TODO: implement step_semi_implicit() (update velocity FIRST, then position).
//
// Build & test:
//   cmake --build build --target 04_semi_implicit_euler && ./build/04_semi_implicit_euler

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
    // TODO: update body.vel using accel*dt, THEN update body.pos using the new vel.
    (void)body;
    (void)accel;
    (void)dt;
}

TEST_CASE("one gravity step updates velocity then moves with it") {
    Body b{Vec2{0.0f, 0.0f}, Vec2{0.0f, 0.0f}};
    step_semi_implicit(b, Vec2{0.0f, 10.0f}, 1.0f);
    REQUIRE(b.vel.y == Catch::Approx(10.0f));  // velocity first: 0 + 10*1
    REQUIRE(b.pos.y == Catch::Approx(10.0f));  // then move with the NEW velocity
}

TEST_CASE("horizontal velocity is unaffected by vertical gravity") {
    Body b{Vec2{0.0f, 0.0f}, Vec2{5.0f, 0.0f}};
    step_semi_implicit(b, Vec2{0.0f, 10.0f}, 0.5f);
    REQUIRE(b.vel.x == Catch::Approx(5.0f));
    REQUIRE(b.pos.x == Catch::Approx(2.5f));  // 5 * 0.5
}
