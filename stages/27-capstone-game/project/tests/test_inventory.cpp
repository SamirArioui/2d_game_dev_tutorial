// ============================================================================
// Tests for the revived Inventory (same contract as stages 09-13). Kept here so
// the capstone's own suite is self-contained.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include <vector>

#include "inventory.hpp"

using game::Inventory;
using game::Item;

namespace {

struct InventoryFixture {
    Inventory inv;
    InventoryFixture() {
        inv.add(Item{"Health Potion", 15}, 3);
        inv.add(Item{"Iron Sword", 120}, 1);
        inv.add(Item{"Gold Coin", 5}, 40);
    }
};

}  // namespace

TEST_CASE_METHOD(InventoryFixture, "add tracks distinct items and counts", "[inventory]") {
    REQUIRE(inv.distinct_count() == 3);
    REQUIRE(inv.count_of("Health Potion") == 3);
    REQUIRE(inv.count_of("Iron Sword") == 1);
    REQUIRE(inv.count_of("Gold Coin") == 40);
    REQUIRE(inv.count_of("Dragon Egg") == 0);  // unknown -> 0, not an error
}

TEST_CASE_METHOD(InventoryFixture, "adding an existing item stacks its count", "[inventory]") {
    inv.add(Item{"Health Potion", 15});  // default count = 1
    REQUIRE(inv.count_of("Health Potion") == 4);
    REQUIRE(inv.distinct_count() == 3);  // still one distinct entry
}

TEST_CASE_METHOD(InventoryFixture, "total_value sums value * count", "[inventory]") {
    // 15*3 + 120*1 + 5*40 = 45 + 120 + 200 = 365
    REQUIRE(inv.total_value() == 365);
}

TEST_CASE_METHOD(InventoryFixture, "sorted_by_value orders highest first (returns a copy)",
                 "[inventory]") {
    const std::vector<Item> sorted = inv.sorted_by_value();
    REQUIRE(sorted.front().name == "Iron Sword");  // 120
    REQUIRE(sorted.back().name == "Gold Coin");     // 5
    REQUIRE(inv.items().front().name == "Health Potion");  // original order intact
}
