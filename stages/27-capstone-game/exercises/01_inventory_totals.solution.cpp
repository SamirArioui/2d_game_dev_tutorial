// Exercise 01 — Inventory totals (solution)
#include <catch2/catch_test_macros.hpp>

#include "inventory.hpp"

using game::Inventory;
using game::Item;

TEST_CASE("inventory counts and total value", "[ex01]") {
    Inventory inv;
    inv.add(Item{"Health Potion", 15}, 2);
    inv.add(Item{"Iron Sword", 100}, 1);

    REQUIRE(inv.distinct_count() == 2);
    REQUIRE(inv.count_of("Health Potion") == 2);
    REQUIRE(inv.total_value() == 15 * 2 + 100);  // 130
}
