// ============================================================================
// Tests for every collision predicate + the bounce response (collision.hpp).
//
// Collision "feels" like something you can only check by watching balls bounce
// — but each predicate is a pure function with an exact answer, so we can assert
// both the DETECTION (hit or miss) and the PENETRATION vector directly. That is
// the stage's whole point: get the maths right here, headless, before trusting
// it in the window.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "collision.hpp"

using collide::AABB;
using collide::Circle;
using collide::Hit;
using gmath::Vec2f;

TEST_CASE("aabb_vs_aabb detects overlap and misses", "[aabb]") {
    AABB a{Vec2f{0.0f, 0.0f}, Vec2f{10.0f, 10.0f}};

    SECTION("clearly separated boxes do not collide") {
        AABB far{Vec2f{100.0f, 100.0f}, Vec2f{10.0f, 10.0f}};
        REQUIRE_FALSE(collide::aabb_vs_aabb(a, far).has_value());
    }

    SECTION("edge-touching boxes are not overlapping") {
        AABB touching{Vec2f{10.0f, 0.0f}, Vec2f{10.0f, 10.0f}};  // shares the x=10 edge
        REQUIRE_FALSE(collide::aabb_vs_aabb(a, touching).has_value());
    }

    SECTION("overlapping boxes report the shallow-axis penetration") {
        // b overlaps a by 5 on x and 10 on y -> back out along x (the shorter).
        AABB b{Vec2f{5.0f, 0.0f}, Vec2f{10.0f, 10.0f}};
        auto hit = collide::aabb_vs_aabb(a, b);
        REQUIRE(hit.has_value());
        REQUIRE(hit->penetration.x == Catch::Approx(-5.0f));  // push A left, off B
        REQUIRE(hit->penetration.y == Catch::Approx(0.0f));
    }

    SECTION("adding the penetration to A separates the boxes") {
        AABB b{Vec2f{5.0f, 0.0f}, Vec2f{10.0f, 10.0f}};
        auto hit = collide::aabb_vs_aabb(a, b);
        REQUIRE(hit.has_value());
        AABB moved{a.pos + hit->penetration, a.size};
        // After moving, they should be at most touching (no overlap).
        REQUIRE_FALSE(collide::aabb_vs_aabb(moved, b).has_value());
    }
}

TEST_CASE("circle_vs_circle detects overlap and depth", "[circle]") {
    SECTION("far apart circles miss") {
        Circle a{Vec2f{0.0f, 0.0f}, 5.0f};
        Circle b{Vec2f{100.0f, 0.0f}, 5.0f};
        REQUIRE_FALSE(collide::circle_vs_circle(a, b).has_value());
    }

    SECTION("overlapping circles report penetration toward A") {
        Circle a{Vec2f{0.0f, 0.0f}, 5.0f};
        Circle b{Vec2f{8.0f, 0.0f}, 5.0f};  // centers 8 apart, radii sum 10 -> depth 2
        auto hit = collide::circle_vs_circle(a, b);
        REQUIRE(hit.has_value());
        REQUIRE(hit->penetration.x == Catch::Approx(-2.0f));  // push A away from B (-x)
        REQUIRE(hit->penetration.y == Catch::Approx(0.0f));
    }

    SECTION("just-touching circles are not overlapping") {
        Circle a{Vec2f{0.0f, 0.0f}, 5.0f};
        Circle b{Vec2f{10.0f, 0.0f}, 5.0f};  // distance == radius sum
        REQUIRE_FALSE(collide::circle_vs_circle(a, b).has_value());
    }
}

TEST_CASE("circle_vs_aabb: center outside the box", "[circle][aabb]") {
    AABB box{Vec2f{0.0f, 0.0f}, Vec2f{100.0f, 100.0f}};

    SECTION("a circle beyond the right face by less than its radius overlaps") {
        Circle c{Vec2f{104.0f, 50.0f}, 10.0f};  // 4 px past the right face
        auto hit = collide::circle_vs_aabb(c, box);
        REQUIRE(hit.has_value());
        REQUIRE(hit->penetration.x == Catch::Approx(6.0f));  // radius(10) - dist(4)
        REQUIRE(hit->penetration.y == Catch::Approx(0.0f));
    }

    SECTION("a circle clearly outside misses") {
        Circle c{Vec2f{200.0f, 50.0f}, 10.0f};
        REQUIRE_FALSE(collide::circle_vs_aabb(c, box).has_value());
    }

    SECTION("a corner-region circle uses the diagonal normal") {
        Circle c{Vec2f{103.0f, 104.0f}, 10.0f};  // off the bottom-right corner
        auto hit = collide::circle_vs_aabb(c, box);
        REQUIRE(hit.has_value());
        // Pushed up-and-right, away from the corner (both components positive).
        REQUIRE(hit->penetration.x > 0.0f);
        REQUIRE(hit->penetration.y > 0.0f);
    }
}

TEST_CASE("circle_vs_aabb: center inside ejects through the nearest face", "[circle][aabb]") {
    AABB box{Vec2f{0.0f, 0.0f}, Vec2f{100.0f, 100.0f}};
    // Center at (10, 50): nearest face is the LEFT one (10 away).
    Circle c{Vec2f{10.0f, 50.0f}, 6.0f};
    auto hit = collide::circle_vs_aabb(c, box);
    REQUIRE(hit.has_value());
    // Eject left by (distance-to-face 10 + radius 6) so the circle fully clears.
    REQUIRE(hit->penetration.x == Catch::Approx(-16.0f));
    REQUIRE(hit->penetration.y == Catch::Approx(0.0f));
}

TEST_CASE("reflect_velocity bounces with restitution", "[response]") {
    const Vec2f left_normal{-1.0f, 0.0f};  // outward normal of a right-hand wall

    SECTION("perfect bounce (e=1) reverses the normal component") {
        Vec2f v{100.0f, 40.0f};  // moving right (into the wall) and down
        Vec2f r = collide::reflect_velocity(v, left_normal, 1.0f);
        REQUIRE(r.x == Catch::Approx(-100.0f));  // reversed
        REQUIRE(r.y == Catch::Approx(40.0f));    // tangential component untouched
    }

    SECTION("no bounce (e=0) kills the normal component") {
        Vec2f v{100.0f, 40.0f};
        Vec2f r = collide::reflect_velocity(v, left_normal, 0.0f);
        REQUIRE(r.x == Catch::Approx(0.0f));
        REQUIRE(r.y == Catch::Approx(40.0f));
    }

    SECTION("half restitution keeps half the incoming normal speed") {
        Vec2f v{100.0f, 0.0f};
        Vec2f r = collide::reflect_velocity(v, left_normal, 0.5f);
        REQUIRE(r.x == Catch::Approx(-50.0f));
    }

    SECTION("an object already moving away is left unchanged") {
        Vec2f v{-100.0f, 0.0f};  // already moving away from the wall
        Vec2f r = collide::reflect_velocity(v, left_normal, 1.0f);
        REQUIRE(r.x == Catch::Approx(-100.0f));
        REQUIRE(r.y == Catch::Approx(0.0f));
    }
}
