// ============================================================================
// Tests for the pure motion & tweening logic (motion.hpp).
//
// Integration and easing are exactly the kind of "feel" code that seems like it
// needs a running game to check — but each step is deterministic arithmetic, so
// we pin it down here without ever opening a window.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "motion.hpp"

using gmath::Vec2f;

TEST_CASE("semi-implicit Euler updates velocity, then moves with it", "[motion][integrate]") {
    // Start at rest, apply gravity (+y is down in screen space) for one 1s step.
    motion::Body body{Vec2f{0.0f, 0.0f}, Vec2f{0.0f, 0.0f}};
    const Vec2f gravity{0.0f, 10.0f};

    motion::integrate_semi_implicit(body, gravity, 1.0f);

    // Velocity first: 0 + 10*1 = 10. Then move with the NEW velocity: 0 + 10*1 = 10.
    REQUIRE(body.vel.y == Catch::Approx(10.0f));
    REQUIRE(body.pos.y == Catch::Approx(10.0f));
}

TEST_CASE("explicit Euler moves with the OLD velocity first", "[motion][integrate]") {
    motion::Body body{Vec2f{0.0f, 0.0f}, Vec2f{0.0f, 0.0f}};
    const Vec2f gravity{0.0f, 10.0f};

    motion::integrate_explicit(body, gravity, 1.0f);

    // Position uses the starting velocity (0), so it doesn't move on step one;
    // velocity then becomes 10. This lag is why we prefer semi-implicit.
    REQUIRE(body.vel.y == Catch::Approx(10.0f));
    REQUIRE(body.pos.y == Catch::Approx(0.0f));
}

TEST_CASE("many small steps under gravity gain speed steadily", "[motion][integrate]") {
    motion::Body body{Vec2f{0.0f, 0.0f}, Vec2f{0.0f, 0.0f}};
    const Vec2f gravity{0.0f, 100.0f};
    const float dt = 0.01f;

    for (int i = 0; i < 100; ++i) {  // 100 * 0.01s = 1 second total
        motion::integrate_semi_implicit(body, gravity, dt);
    }
    // After 1s under a=100, velocity ~= 100 (a*t).
    REQUIRE(body.vel.y == Catch::Approx(100.0f));
    // Falling distance is positive and on the order of ½at² ~ 50 (small step bias).
    REQUIRE(body.pos.y > 45.0f);
    REQUIRE(body.pos.y < 55.0f);
}

TEST_CASE("aim_velocity points at the target with the given speed", "[motion][aim]") {
    Vec2f origin{100.0f, 100.0f};
    Vec2f target{100.0f, 400.0f};  // straight down
    Vec2f v = motion::aim_velocity(origin, target, 250.0f);

    // Magnitude is exactly the requested speed ...
    REQUIRE(gmath::length(v) == Catch::Approx(250.0f));
    // ... and the direction is straight down (+y).
    REQUIRE(v.x == Catch::Approx(0.0f));
    REQUIRE(v.y == Catch::Approx(250.0f));

    SECTION("a diagonal target still gives exactly `speed`") {
        Vec2f d = motion::aim_velocity(Vec2f{0.0f, 0.0f}, Vec2f{3.0f, 4.0f}, 10.0f);
        REQUIRE(gmath::length(d) == Catch::Approx(10.0f));
        REQUIRE(d.x == Catch::Approx(6.0f));   // (3/5)*10
        REQUIRE(d.y == Catch::Approx(8.0f));   // (4/5)*10
    }
}

TEST_CASE("easing curves pass through 0 and 1", "[motion][easing]") {
    // Every easing function must satisfy f(0)=0 and f(1)=1 so the tween starts
    // at `a` and ends at `b`.
    REQUIRE(motion::ease_in_quad(0.0f) == Catch::Approx(0.0f));
    REQUIRE(motion::ease_in_quad(1.0f) == Catch::Approx(1.0f));
    REQUIRE(motion::ease_out_quad(0.0f) == Catch::Approx(0.0f));
    REQUIRE(motion::ease_out_quad(1.0f) == Catch::Approx(1.0f));
    REQUIRE(motion::smoothstep(0.0f) == Catch::Approx(0.0f));
    REQUIRE(motion::smoothstep(1.0f) == Catch::Approx(1.0f));

    SECTION("smoothstep is symmetric around the midpoint") {
        REQUIRE(motion::smoothstep(0.5f) == Catch::Approx(0.5f));
        // ease_in is below the straight line early; ease_out is above it.
        REQUIRE(motion::ease_in_quad(0.5f) < 0.5f);
        REQUIRE(motion::ease_out_quad(0.5f) > 0.5f);
    }
}

TEST_CASE("ping_pong sweeps 0->1->0 and repeats", "[motion][tween]") {
    REQUIRE(motion::ping_pong(0.0f) == Catch::Approx(0.0f));
    REQUIRE(motion::ping_pong(0.5f) == Catch::Approx(0.5f));
    REQUIRE(motion::ping_pong(1.0f) == Catch::Approx(1.0f));  // peak
    REQUIRE(motion::ping_pong(1.5f) == Catch::Approx(0.5f));  // coming back down
    REQUIRE(motion::ping_pong(2.0f) == Catch::Approx(0.0f));  // full period
    REQUIRE(motion::ping_pong(2.5f) == Catch::Approx(0.5f));  // repeats
}
