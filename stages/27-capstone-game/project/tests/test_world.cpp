// ============================================================================
// Tests for the capstone's pure gameplay logic: tile collision (resolve_move)
// and item pickup (try_pickup). Both are plain functions over data, so we test
// the actual game rules with no window.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include <vector>

#include "eng/collision.hpp"
#include "eng/math.hpp"
#include "eng/tilemap.hpp"
#include "inventory.hpp"
#include "world.hpp"

using gmath::AABB;
using gmath::Vec2f;

namespace {

// 4 wide, 3 tall, 32px tiles. A solid wall column at x = 2 (world 64..96).
eng::Tilemap wall_map() {
    eng::Tilemap map;
    const bool ok = map.load_from_string(
        "4 3 32\n"
        "0 0 1 0\n"
        "0 0 1 0\n"
        "0 0 1 0\n");
    REQUIRE(ok);
    return map;
}

}  // namespace

TEST_CASE("moving into a wall stops flush against it", "[world][collision]") {
    const eng::Tilemap map = wall_map();
    const AABB body{{10.0f, 40.0f}, {16.0f, 16.0f}};  // in the floor, left of the wall

    // Try to move right by 60px; the wall's left edge is at x = 64.
    const Vec2f result = game::resolve_move(map, body, Vec2f{60.0f, 0.0f});

    // The 16px-wide body is snapped so its RIGHT edge sits on the wall (64),
    // i.e. its left edge is at 48. It does NOT pass through.
    REQUIRE(result.x == 48.0f);
    REQUIRE(result.y == 40.0f);  // no vertical motion requested
}

TEST_CASE("moving off the map edge is blocked (out-of-bounds is solid)", "[world][collision]") {
    const eng::Tilemap map = wall_map();
    const AABB body{{40.0f, 40.0f}, {16.0f, 16.0f}};

    // Push left past x = 0; the body should stop with its left edge at 0.
    const Vec2f result = game::resolve_move(map, body, Vec2f{-60.0f, 0.0f});
    REQUIRE(result.x == 0.0f);
}

TEST_CASE("free movement with no obstacle applies the full delta", "[world][collision]") {
    const eng::Tilemap map = wall_map();
    const AABB body{{10.0f, 40.0f}, {16.0f, 16.0f}};

    // A small step that hits nothing: the body moves the whole way.
    const Vec2f result = game::resolve_move(map, body, Vec2f{10.0f, 0.0f});
    REQUIRE(result.x == 20.0f);
    REQUIRE(result.y == 40.0f);
}

TEST_CASE("picking up an overlapping item adds it to the inventory once", "[world][pickup]") {
    std::vector<game::WorldItem> items = {
        {"Health Potion", 15, {100.0f, 100.0f}, 20.0f, false},
        {"Gold Coin", 5, {400.0f, 400.0f}, 20.0f, false},  // far away
    };
    game::Inventory inv;

    // Player overlapping only the first item.
    const AABB player{{95.0f, 95.0f}, {24.0f, 24.0f}};

    const std::vector<Vec2f> picked = game::try_pickup(items, player, inv);

    REQUIRE(picked.size() == 1);                 // exactly one item collected
    REQUIRE(items[0].collected == true);         // marked so it won't re-trigger
    REQUIRE(items[1].collected == false);        // the far one is untouched
    REQUIRE(inv.count_of("Health Potion") == 1);
    REQUIRE(inv.count_of("Gold Coin") == 0);

    // Walking over the same (now-collected) item again does nothing.
    const std::vector<Vec2f> again = game::try_pickup(items, player, inv);
    REQUIRE(again.empty());
    REQUIRE(inv.count_of("Health Potion") == 1);
}
