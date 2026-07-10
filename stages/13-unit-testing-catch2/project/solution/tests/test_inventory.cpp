// ============================================================================
// Tests for the Inventory (sort-by-value + std::function filter).  (REFERENCE SOLUTION)
//
// This is the complete reference test file. The starter you fill in is one
// level up at ../../tests/test_inventory.cpp.
//
// Shows a FIXTURE: a struct whose members are the shared setup. Catch2 builds a
// fresh instance for every TEST_CASE_METHOD, so each test starts from an
// identical, un-mutated inventory. Python: exactly what a pytest fixture gives
// you, but the "fixture" here is just the base struct's data members.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include <string>
#include <vector>

#include "game/inventory.hpp"

using game::Inventory;
using game::Item;

namespace {

// The fixture. Its constructor runs before each test that inherits it, so every
// test gets its own freshly-populated `inv`.
struct InventoryFixture {
    Inventory inv;

    InventoryFixture() {
        inv.add(Item{"Health Potion", 15}, 3);
        inv.add(Item{"Iron Sword", 120}, 1);
        inv.add(Item{"Gold Coin", 1}, 57);
    }
};

}  // namespace

TEST_CASE_METHOD(InventoryFixture, "add tracks distinct items and counts", "[inventory]") {
    REQUIRE(inv.distinct_count() == 3);
    REQUIRE(inv.count_of("Health Potion") == 3);
    REQUIRE(inv.count_of("Iron Sword") == 1);
    REQUIRE(inv.count_of("Gold Coin") == 57);
    // Unknown items are simply absent -> 0, not an error.
    REQUIRE(inv.count_of("Dragon Egg") == 0);
}

TEST_CASE_METHOD(InventoryFixture, "adding an existing item stacks its count", "[inventory]") {
    inv.add(Item{"Health Potion", 15}, 2);
    // Count went up, but it's still ONE distinct item (not a new entry).
    REQUIRE(inv.count_of("Health Potion") == 5);
    REQUIRE(inv.distinct_count() == 3);
}

TEST_CASE_METHOD(InventoryFixture, "total_value sums value * count", "[inventory]") {
    // 120*1 + 15*3 + 1*57 = 120 + 45 + 57 = 222.
    REQUIRE(inv.total_value() == 222);
}

TEST_CASE_METHOD(InventoryFixture, "sorted_by_value orders highest value first", "[inventory]") {
    const std::vector<Item> sorted = inv.sorted_by_value();
    REQUIRE(sorted.size() == 3);
    REQUIRE(sorted[0].name == "Iron Sword");     // 120
    REQUIRE(sorted[1].name == "Health Potion");  // 15
    REQUIRE(sorted[2].name == "Gold Coin");      // 1
    // Sorting returns a COPY: the inventory's own order is untouched.
    REQUIRE(inv.items().front().name == "Health Potion");
}

TEST_CASE_METHOD(InventoryFixture, "filter keeps only matching items", "[inventory]") {
    // The predicate is a lambda passed as a std::function. Python: filter(pred, items).
    const std::vector<Item> valuable =
        inv.filter([](const Item& item) { return item.value >= 15; });
    REQUIRE(valuable.size() == 2);  // Iron Sword + Health Potion; Gold Coin (1) drops out

    // A predicate that matches nothing yields an empty result (not an error).
    const std::vector<Item> none = inv.filter([](const Item&) { return false; });
    REQUIRE(none.empty());
}
