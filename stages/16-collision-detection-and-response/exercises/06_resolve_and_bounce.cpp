// Exercise 06 — resolve a collision: separate + bounce (medium-hard) — a drill.
//
// This ties the stage together. Detection handed you a Hit with a `penetration`
// vector. RESOLUTION does two things with it:
//   1. positional correction: move the body OUT of overlap (pos += penetration),
//   2. velocity response: bounce the body off the contact normal, which is just
//      normalize(penetration).
//
// You're given dot/length/normalize and reflect_velocity; implement resolve()
// to apply both steps. This is exactly what the mini-project does every time two
// things touch.
//
// TODO: implement resolve().
//
// Build & test:
//   cmake --build build --target 06_resolve_and_bounce && ./build/06_resolve_and_bounce

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>

struct Vec2 {
    float x;
    float y;
};

struct Body {
    Vec2 pos;
    Vec2 vel;
};

struct Hit {
    Vec2 penetration;  // add to pos to separate; its direction is the normal
};

float dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

Vec2 normalize(const Vec2& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len <= 1e-8f) {
        return Vec2{0.0f, 0.0f};
    }
    return Vec2{v.x / len, v.y / len};
}

Vec2 reflect_velocity(const Vec2& v, const Vec2& n, float e) {
    float vn = dot(v, n);
    if (vn >= 0.0f) {
        return v;
    }
    float k = (1.0f + e) * vn;
    return Vec2{v.x - n.x * k, v.y - n.y * k};
}

void resolve(Body& body, const Hit& hit, float restitution) {
    // TODO:
    //   1. move body.pos out of overlap by hit.penetration,
    //   2. set body.vel to reflect_velocity(vel, normalize(penetration), restitution).
    (void)body;
    (void)hit;
    (void)restitution;
}

TEST_CASE("a ball driven into a right-hand wall is pushed out and reversed") {
    // Penetration points left (-x): the body overshot to the right and must back
    // out. It was moving right (+x); after resolving it should move left.
    Body ball{Vec2{96.0f, 300.0f}, Vec2{120.0f, 0.0f}};
    resolve(ball, Hit{Vec2{-6.0f, 0.0f}}, 1.0f);
    REQUIRE(ball.pos.x == Catch::Approx(90.0f));   // moved out by the penetration
    REQUIRE(ball.vel.x == Catch::Approx(-120.0f)); // bounced back
    REQUIRE(ball.vel.y == Catch::Approx(0.0f));    // tangential unchanged
}

TEST_CASE("restitution 0 stops the ball at the surface") {
    Body ball{Vec2{96.0f, 300.0f}, Vec2{120.0f, 30.0f}};
    resolve(ball, Hit{Vec2{-6.0f, 0.0f}}, 0.0f);
    REQUIRE(ball.vel.x == Catch::Approx(0.0f));   // no bounce
    REQUIRE(ball.vel.y == Catch::Approx(30.0f));  // slides along the wall
}
