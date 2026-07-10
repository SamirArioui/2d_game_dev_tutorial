// ============================================================================
// Tests for the pure game-math functions (clamp / lerp / distance).  (REFERENCE SOLUTION)
//
// This is the complete reference test file. The starter you fill in is one
// level up at ../../tests/test_math.cpp.
//
// These are the EASIEST things in the codebase to test, precisely because they
// are pure: no globals, no I/O, no randomness. Same inputs -> same output, so a
// test just states the known answer. That's the whole argument of the stage:
// keep game LOGIC pure and separate from rendering, and it stays this testable.
// ============================================================================

#include <catch2/catch_approx.hpp>       // Catch::Approx — tolerant float compare
#include <catch2/catch_test_macros.hpp>  // TEST_CASE / REQUIRE / CHECK / SECTION

#include "game/game_math.hpp"

using game::clamp;
using game::distance;
using game::lerp;

// A TEST_CASE is one named test. The second argument is a free-form list of
// [tags] you can filter on from the command line (e.g. `./tests "[math]"`).
// Python: like a `def test_clamp():` in a file pytest collects.
TEST_CASE("clamp keeps values inside [lo, hi]", "[math][clamp]") {
    // SECTIONs share the code ABOVE them but run independently — the case is
    // re-entered once per section. Great for "same setup, several situations".
    SECTION("value below the range snaps up to lo") {
        REQUIRE(clamp(-5.0f, 0.0f, 10.0f) == 0.0f);
    }
    SECTION("value above the range snaps down to hi") {
        REQUIRE(clamp(42.0f, 0.0f, 10.0f) == 10.0f);
    }
    SECTION("value already inside is returned unchanged") {
        REQUIRE(clamp(3.0f, 0.0f, 10.0f) == 3.0f);
    }
    SECTION("the endpoints are inclusive") {
        // REQUIRE aborts this section on failure; CHECK records the failure but
        // keeps going, so you see BOTH endpoint results from one run.
        CHECK(clamp(0.0f, 0.0f, 10.0f) == 0.0f);
        CHECK(clamp(10.0f, 0.0f, 10.0f) == 10.0f);
    }
}

TEST_CASE("lerp blends a and b by t", "[math][lerp]") {
    // Floats rarely land on an exact value, so compare with Approx (a tolerance
    // band) instead of ==. Python: like pytest.approx(...).
    REQUIRE(lerp(0.0f, 10.0f, 0.0f) == Catch::Approx(0.0f));   // t=0 -> a
    REQUIRE(lerp(0.0f, 10.0f, 1.0f) == Catch::Approx(10.0f));  // t=1 -> b
    REQUIRE(lerp(0.0f, 10.0f, 0.5f) == Catch::Approx(5.0f));   // halfway
    REQUIRE(lerp(10.0f, 20.0f, 0.25f) == Catch::Approx(12.5f));
}

TEST_CASE("distance is the straight-line length", "[math][distance]") {
    // The classic 3-4-5 right triangle: sqrt(3^2 + 4^2) == 5.
    REQUIRE(distance(0.0f, 0.0f, 3.0f, 4.0f) == Catch::Approx(5.0f));
    // Distance from a point to itself is 0, and it's symmetric.
    REQUIRE(distance(2.0f, 7.0f, 2.0f, 7.0f) == Catch::Approx(0.0f));
    REQUIRE(distance(1.0f, 1.0f, 4.0f, 5.0f) == Catch::Approx(distance(4.0f, 5.0f, 1.0f, 1.0f)));
}
