// ===========================================================================
// Unit tests for the component model.
//
// Everything here is window-free: components are plain data and systems are pure functions, so we
// can assert on them directly with no SFML and no display. That separation — logic testable,
// rendering not — is exactly what the component model buys us.
// ===========================================================================

#include <vector>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "ecs/component.hpp"
#include "ecs/entity.hpp"
#include "game/systems.hpp"

using ecs::Entity;
using ecs::Shape;
using ecs::Transform;
using ecs::Velocity;
using gmath::Vec2f;

TEST_CASE("Entity stores and retrieves components by type", "[entity]") {
    Entity e;

    SECTION("a fresh entity has nothing") {
        REQUIRE_FALSE(e.has<Transform>());
        REQUIRE(e.try_get<Transform>() == nullptr);
        REQUIRE(e.component_count() == 0);
    }

    SECTION("add makes has/get see the component") {
        e.add(Transform{Vec2f{3.0f, 4.0f}});
        REQUIRE(e.has<Transform>());
        REQUIRE(e.get<Transform>().pos == Vec2f{3.0f, 4.0f});
        REQUIRE(e.component_count() == 1);
    }

    SECTION("different component types are independent") {
        e.add(Transform{Vec2f{1.0f, 1.0f}});
        e.add(Velocity{Vec2f{5.0f, 0.0f}});
        REQUIRE(e.has<Transform>());
        REQUIRE(e.has<Velocity>());
        REQUIRE_FALSE(e.has<Shape>());
        REQUIRE(e.component_count() == 2);
    }

    SECTION("add twice REPLACES rather than duplicating") {
        e.add(Transform{Vec2f{1.0f, 1.0f}});
        e.add(Transform{Vec2f{9.0f, 9.0f}});
        REQUIRE(e.component_count() == 1);
        REQUIRE(e.get<Transform>().pos == Vec2f{9.0f, 9.0f});
    }

    SECTION("remove detaches a component") {
        e.add(Velocity{});
        e.remove<Velocity>();
        REQUIRE_FALSE(e.has<Velocity>());
    }
}

TEST_CASE("add returns a mutable reference to the stored component", "[entity]") {
    Entity e;
    Transform& t = e.add(Transform{});
    t.pos = Vec2f{7.0f, 8.0f};  // mutate through the returned reference...
    REQUIRE(e.get<Transform>().pos == Vec2f{7.0f, 8.0f});  // ...and it is the same stored object.
}

TEST_CASE("movement_system advances only entities with Transform AND Velocity", "[systems]") {
    std::vector<Entity> entities;
    entities.reserve(2);  // so the references below stay valid as the vector grows

    Entity& mover = entities.emplace_back();
    mover.add(Transform{Vec2f{0.0f, 0.0f}});
    mover.add(Velocity{Vec2f{10.0f, -20.0f}});

    Entity& stuck = entities.emplace_back();  // has a position but no velocity
    stuck.add(Transform{Vec2f{100.0f, 100.0f}});

    game::movement_system(entities, 0.5f);  // half a second

    REQUIRE(entities[0].get<Transform>().pos.x == Catch::Approx(5.0f));
    REQUIRE(entities[0].get<Transform>().pos.y == Catch::Approx(-10.0f));
    // The velocity-less entity is skipped entirely.
    REQUIRE(entities[1].get<Transform>().pos == Vec2f{100.0f, 100.0f});
}

TEST_CASE("bounce_in_bounds reflects velocity at the walls", "[systems]") {
    std::vector<Entity> entities;
    Entity& ball = entities.emplace_back();
    ball.add(Transform{Vec2f{-5.0f, 50.0f}});      // already past the left wall
    ball.add(Velocity{Vec2f{-100.0f, 0.0f}});      // moving further left
    ball.add(Shape{Vec2f{10.0f, 10.0f}});

    game::bounce_in_bounds(entities, Vec2f{640.0f, 480.0f});

    REQUIRE(ball.get<Transform>().pos.x == Catch::Approx(0.0f));  // clamped back inside
    REQUIRE(ball.get<Velocity>().vel.x == Catch::Approx(100.0f)); // and reversed
}

TEST_CASE("aabb_overlap detects overlapping and separated boxes", "[systems]") {
    // Two boxes that clearly overlap.
    REQUIRE(game::aabb_overlap(Vec2f{0, 0}, Vec2f{10, 10}, Vec2f{5, 5}, Vec2f{10, 10}));
    // Separated on the x axis -> no overlap.
    REQUIRE_FALSE(game::aabb_overlap(Vec2f{0, 0}, Vec2f{10, 10}, Vec2f{20, 0}, Vec2f{10, 10}));
    // Touching edges exactly do NOT count as overlap (strict inequality).
    REQUIRE_FALSE(game::aabb_overlap(Vec2f{0, 0}, Vec2f{10, 10}, Vec2f{10, 0}, Vec2f{10, 10}));
}
