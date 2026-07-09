// ============================================================================
// Tests for the Tilemap PARSER and queries — pure logic, no SFML, no window.
//
// This is exactly why Tilemap has no SFML in it: we can hand it text literals
// and assert on the parsed result, the same way stage 13 tested clamp/lerp.
// A malformed map must THROW, a valid one must expose the right dimensions and
// answer tile_at / solid_at correctly.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>

#include "game/tilemap.hpp"

using game::Tilemap;
using gmath::Vec2f;

// A tiny, hand-checkable 4x3 map with a solid border and two interior holes.
// tile ids:  1 1 1 1
//            1 0 0 1
//            1 1 1 1
static const std::string kSmallMap = "4 3 32\n"
                                     "1 1 1 1\n"
                                     "1 0 0 1\n"
                                     "1 1 1 1\n";

TEST_CASE("from_text reads the header dimensions", "[tilemap][parse]") {
    const Tilemap map = Tilemap::from_text(kSmallMap);
    REQUIRE(map.width() == 4);
    REQUIRE(map.height() == 3);
    REQUIRE(map.tile_size() == 32);
    // width * height tiles were stored.
    REQUIRE(map.tiles().size() == 12u);
}

TEST_CASE("tile_at returns the right id per cell", "[tilemap][tile_at]") {
    const Tilemap map = Tilemap::from_text(kSmallMap);

    SECTION("corners and border are solid id 1") {
        REQUIRE(map.tile_at(0, 0) == 1);
        REQUIRE(map.tile_at(3, 0) == 1);
        REQUIRE(map.tile_at(0, 2) == 1);
        REQUIRE(map.tile_at(3, 2) == 1);
    }
    SECTION("the interior holes are empty id 0") {
        REQUIRE(map.tile_at(1, 1) == 0);
        REQUIRE(map.tile_at(2, 1) == 0);
    }
    SECTION("out-of-range cells read as empty (0), never throw") {
        CHECK(map.tile_at(-1, 0) == 0);
        CHECK(map.tile_at(0, -1) == 0);
        CHECK(map.tile_at(4, 0) == 0);   // x == width
        CHECK(map.tile_at(0, 3) == 0);   // y == height
        CHECK(map.tile_at(999, 999) == 0);
    }
}

TEST_CASE("solid_at maps world coords to the right tile", "[tilemap][solid_at]") {
    const Tilemap map = Tilemap::from_text(kSmallMap);  // 32px tiles

    SECTION("a point inside a border tile is solid") {
        REQUIRE(map.solid_at(Vec2f{16.0f, 16.0f}));  // cell (0,0), id 1
    }
    SECTION("a point inside an interior hole is not solid") {
        REQUIRE_FALSE(map.solid_at(Vec2f{48.0f, 48.0f}));  // cell (1,1), id 0
    }
    SECTION("tile boundaries fall on multiples of tile_size") {
        // Row 1 is "1 0 0 1": columns 0 and 3 are solid, columns 1-2 empty.
        // x in [32,64) is column 1 (a hole); x in [96,128) is column 3 (a wall).
        CHECK_FALSE(map.solid_at(Vec2f{32.0f, 32.0f}));  // col 1, row 1 -> empty
        CHECK(map.solid_at(Vec2f{100.0f, 32.0f}));       // col 3, row 1 -> solid
    }
    SECTION("negative world coords floor to negative cells (off-map, empty)") {
        // -0.5 must floor to column -1 (off the map), NOT truncate to 0.
        REQUIRE_FALSE(map.solid_at(Vec2f{-0.5f, 16.0f}));
    }
}

TEST_CASE("malformed maps throw", "[tilemap][malformed]") {
    SECTION("empty / missing header") {
        REQUIRE_THROWS_AS(Tilemap::from_text(""), std::runtime_error);
    }
    SECTION("non-numeric header token") {
        REQUIRE_THROWS_AS(Tilemap::from_text("four 3 32\n"), std::runtime_error);
    }
    SECTION("non-positive dimension") {
        REQUIRE_THROWS_AS(Tilemap::from_text("0 3 32\n"), std::runtime_error);
        REQUIRE_THROWS_AS(Tilemap::from_text("4 3 0\n"), std::runtime_error);
    }
    SECTION("too few tiles (truncated body)") {
        REQUIRE_THROWS_AS(Tilemap::from_text("4 3 32\n1 1 1 1\n1 0 0 1\n"), std::runtime_error);
    }
    SECTION("too many tiles (trailing junk)") {
        REQUIRE_THROWS_AS(Tilemap::from_text(kSmallMap + "7 7 7\n"), std::runtime_error);
    }
    SECTION("a non-integer tile id in the body") {
        REQUIRE_THROWS_AS(Tilemap::from_text("2 1 32\n1 x\n"), std::runtime_error);
    }
}
