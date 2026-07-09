// Exercise 04 — Item pickup (medium)
//
// try_pickup collects every not-yet-collected item the player overlaps, adds it
// to the inventory, and returns the picked positions.

#include <catch2/catch_test_macros.hpp>

#include <vector>

#include "eng/collision.hpp"
#include "inventory.hpp"
#include "world.hpp"

using gmath::AABB;
using gmath::Vec2f;

TEST_CASE("only overlapping items are collected", "[ex04]") {
    std::vector<game::WorldItem> items = {
        {"Gold Coin", 5, {100.0f, 100.0f}, 20.0f, false},
        {"Iron Sword", 120, {500.0f, 500.0f}, 20.0f, false},  // far from the player
    };
    game::Inventory inv;

    // Player overlapping ONLY the coin.
    const AABB player{{96.0f, 96.0f}, {24.0f, 24.0f}};
    const std::vector<Vec2f> picked = game::try_pickup(items, player, inv);

    // TODO: how many items were picked this call?
    REQUIRE(picked.size() == 99);  // <-- fix

    // TODO: the coin count in the inventory?
    REQUIRE(inv.count_of("Gold Coin") == 99);  // <-- fix

    // TODO: was the far-away sword collected?
    REQUIRE(items[1].collected == true);  // <-- fix
}
