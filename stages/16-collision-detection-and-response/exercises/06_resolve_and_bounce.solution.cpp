// Exercise 06 — resolve a collision: separate + bounce (solution).

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
    Vec2 penetration;
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
    // 1. positional correction — separate the shapes.
    body.pos.x += hit.penetration.x;
    body.pos.y += hit.penetration.y;
    // 2. velocity response — bounce off the contact normal.
    Vec2 normal = normalize(hit.penetration);
    body.vel = reflect_velocity(body.vel, normal, restitution);
}

TEST_CASE("a ball driven into a right-hand wall is pushed out and reversed") {
    Body ball{Vec2{96.0f, 300.0f}, Vec2{120.0f, 0.0f}};
    resolve(ball, Hit{Vec2{-6.0f, 0.0f}}, 1.0f);
    REQUIRE(ball.pos.x == Catch::Approx(90.0f));
    REQUIRE(ball.vel.x == Catch::Approx(-120.0f));
    REQUIRE(ball.vel.y == Catch::Approx(0.0f));
}

TEST_CASE("restitution 0 stops the ball at the surface") {
    Body ball{Vec2{96.0f, 300.0f}, Vec2{120.0f, 30.0f}};
    resolve(ball, Hit{Vec2{-6.0f, 0.0f}}, 0.0f);
    REQUIRE(ball.vel.x == Catch::Approx(0.0f));
    REQUIRE(ball.vel.y == Catch::Approx(30.0f));
}
