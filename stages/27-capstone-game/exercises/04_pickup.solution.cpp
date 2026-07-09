// Exercise 04 — Item pickup (solution)
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
        {"Iron Sword", 120, {500.0f, 500.0f}, 20.0f, false},
    };
    game::Inventory inv;

    const AABB player{{96.0f, 96.0f}, {24.0f, 24.0f}};
    const std::vector<Vec2f> picked = game::try_pickup(items, player, inv);

    REQUIRE(picked.size() == 1);
    REQUIRE(inv.count_of("Gold Coin") == 1);
    REQUIRE(items[1].collected == false);  // sword too far -> untouched
}
