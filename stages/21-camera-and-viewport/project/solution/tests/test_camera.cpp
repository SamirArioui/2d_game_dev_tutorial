// ============================================================================
// Tests for the PURE camera maths — clamp, world<->screen, parallax. No SFML.
//
// The camera *behaviour* (does the view scroll on screen?) needs a display and
// isn't tested. But the arithmetic underneath it — clamping the center to the
// level, and converting between screen and world — is pure and must be exact,
// so it gets the stage-13 treatment here.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "game/camera.hpp"

using game::clamp_view_center;
using game::parallax_offset;
using game::screen_to_world;
using game::world_to_screen;
using gmath::Vec2f;

TEST_CASE("clamp_view_center keeps the view inside the level", "[camera][clamp]") {
    const Vec2f view{800.0f, 600.0f};
    const Vec2f level{2000.0f, 1500.0f};

    SECTION("a centered target is left untouched") {
        const Vec2f c = clamp_view_center(Vec2f{1000.0f, 750.0f}, view, level);
        REQUIRE(c.x == Catch::Approx(1000.0f));
        REQUIRE(c.y == Catch::Approx(750.0f));
    }
    SECTION("near the top-left, the center snaps to half the view size") {
        const Vec2f c = clamp_view_center(Vec2f{0.0f, 0.0f}, view, level);
        REQUIRE(c.x == Catch::Approx(400.0f));  // half of 800
        REQUIRE(c.y == Catch::Approx(300.0f));  // half of 600
    }
    SECTION("near the bottom-right, it snaps to level - half") {
        const Vec2f c = clamp_view_center(Vec2f{9999.0f, 9999.0f}, view, level);
        REQUIRE(c.x == Catch::Approx(2000.0f - 400.0f));
        REQUIRE(c.y == Catch::Approx(1500.0f - 300.0f));
    }
    SECTION("a level smaller than the view is simply centered") {
        const Vec2f small{400.0f, 300.0f};  // smaller than the 800x600 view
        const Vec2f c = clamp_view_center(Vec2f{123.0f, 45.0f}, view, small);
        REQUIRE(c.x == Catch::Approx(200.0f));  // half the level
        REQUIRE(c.y == Catch::Approx(150.0f));
    }
}

TEST_CASE("screen_to_world and world_to_screen are inverses", "[camera][coords]") {
    const Vec2f center{1000.0f, 750.0f};
    const Vec2f view{800.0f, 600.0f};
    const Vec2f window{800.0f, 600.0f};

    SECTION("the window center maps to the view center") {
        const Vec2f w = screen_to_world(Vec2f{400.0f, 300.0f}, center, view, window);
        REQUIRE(w.x == Catch::Approx(1000.0f));
        REQUIRE(w.y == Catch::Approx(750.0f));
    }
    SECTION("the top-left pixel maps to the view's top-left world corner") {
        const Vec2f w = screen_to_world(Vec2f{0.0f, 0.0f}, center, view, window);
        REQUIRE(w.x == Catch::Approx(600.0f));  // 1000 - 400
        REQUIRE(w.y == Catch::Approx(450.0f));  // 750 - 300
    }
    SECTION("round-trip: world -> screen -> world returns the start") {
        const Vec2f start{1234.0f, 567.0f};
        const Vec2f px = world_to_screen(start, center, view, window);
        const Vec2f back = screen_to_world(px, center, view, window);
        REQUIRE(back.x == Catch::Approx(start.x));
        REQUIRE(back.y == Catch::Approx(start.y));
    }
    SECTION("a zoomed view (view != window) still round-trips") {
        const Vec2f zoom_view{400.0f, 300.0f};  // half size -> 2x zoom
        const Vec2f start{980.0f, 720.0f};
        const Vec2f px = world_to_screen(start, center, zoom_view, window);
        const Vec2f back = screen_to_world(px, center, zoom_view, window);
        REQUIRE(back.x == Catch::Approx(start.x));
        REQUIRE(back.y == Catch::Approx(start.y));
    }
}

TEST_CASE("parallax_offset scrolls distant layers slower", "[camera][parallax]") {
    const Vec2f cam{1000.0f, 500.0f};

    SECTION("factor 1.0 scrolls with the world") {
        const Vec2f o = parallax_offset(cam, 1.0f);
        REQUIRE(o.x == Catch::Approx(1000.0f));
        REQUIRE(o.y == Catch::Approx(500.0f));
    }
    SECTION("factor 0.0 never scrolls (pinned)") {
        const Vec2f o = parallax_offset(cam, 0.0f);
        REQUIRE(o.x == Catch::Approx(0.0f));
        REQUIRE(o.y == Catch::Approx(0.0f));
    }
    SECTION("a factor in between scrolls proportionally") {
        const Vec2f o = parallax_offset(cam, 0.3f);
        REQUIRE(o.x == Catch::Approx(300.0f));
        REQUIRE(o.y == Catch::Approx(150.0f));
    }
}
