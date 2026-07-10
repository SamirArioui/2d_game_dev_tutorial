// ============================================================================
// Tests for the pure game-math functions (clamp / lerp / distance).   (YOUR TASK)
//
// This is the STARTER. The code under test (the `game` library) is already
// complete — YOUR job is to write the assertions. Each SECTION / case below
// says what to prove: replace every `// TODO(stage 13): assert ...` with a real
// REQUIRE(...) / CHECK(...), then DELETE the `FAIL("implement me")` line so the
// case can go green. Until you do, `ctest` is RED on purpose.
//
// A complete reference is in ../../solution/tests/test_math.cpp — try it first.
//
// These are the EASIEST things in the codebase to test, precisely because they
// are pure: no globals, no I/O, no randomness. Same inputs -> same output, so a
// test just states the known answer. That's the whole argument of the stage:
// keep game LOGIC pure and separate from rendering, and it stays this testable.
// ============================================================================

#include <catch2/catch_approx.hpp>       // Catch::Approx — tolerant float compare
#include <catch2/catch_test_macros.hpp>  // TEST_CASE / REQUIRE / CHECK / SECTION / FAIL

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
        // TODO(stage 13): assert clamp(-5, 0, 10) == 0  (below range snaps up to lo)
    }
    SECTION("value above the range snaps down to hi") {
        // TODO(stage 13): assert clamp(42, 0, 10) == 10  (above range snaps down to hi)
    }
    SECTION("value already inside is returned unchanged") {
        // TODO(stage 13): assert clamp(3, 0, 10) == 3  (in-range value unchanged)
    }
    SECTION("the endpoints are inclusive") {
        // TODO(stage 13): CHECK both endpoints — clamp(0,0,10)==0 AND clamp(10,0,10)==10.
        //   Use CHECK (soft) here, not REQUIRE, so you see BOTH results from one run.
    }

    FAIL("implement me — write the assertions above, then delete this line");
}

TEST_CASE("lerp blends a and b by t", "[math][lerp]") {
    // Floats rarely land on an exact value, so compare with Catch::Approx (a
    // tolerance band) instead of ==. Python: like pytest.approx(...).
    // TODO(stage 13): assert lerp(0, 10, 0)    == Catch::Approx(0)     (t=0 -> a)
    // TODO(stage 13): assert lerp(0, 10, 1)    == Catch::Approx(10)    (t=1 -> b)
    // TODO(stage 13): assert lerp(0, 10, 0.5)  == Catch::Approx(5)     (halfway)
    // TODO(stage 13): assert lerp(10, 20, 0.25) == Catch::Approx(12.5)

    FAIL("implement me — write the assertions above, then delete this line");
}

TEST_CASE("distance is the straight-line length", "[math][distance]") {
    // TODO(stage 13): assert the classic 3-4-5 triangle: distance(0,0,3,4) == Catch::Approx(5)
    // TODO(stage 13): assert distance from a point to itself is 0
    // TODO(stage 13): assert distance is symmetric: distance(a,b) == distance(b,a)

    FAIL("implement me — write the assertions above, then delete this line");
}
