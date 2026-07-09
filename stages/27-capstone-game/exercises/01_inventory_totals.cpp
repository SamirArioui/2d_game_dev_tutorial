// Exercise 01 — Inventory totals (easy)
//
// Fill in the expected values (marked `// <-- fix`). Pure logic, Catch2 only.

#include <catch2/catch_test_macros.hpp>

#include "inventory.hpp"

using game::Inventory;
using game::Item;

TEST_CASE("inventory counts and total value", "[ex01]") {
    Inventory inv;
    inv.add(Item{"Health Potion", 15}, 2);
    inv.add(Item{"Iron Sword", 100}, 1);

    // TODO: two distinct items were added.
    REQUIRE(inv.distinct_count() == 0);  // <-- fix

    // TODO: how many Health Potions?
    REQUIRE(inv.count_of("Health Potion") == 0);  // <-- fix

    // TODO: total value = 15*2 + 100*1
    REQUIRE(inv.total_value() == 0);  // <-- fix
}
