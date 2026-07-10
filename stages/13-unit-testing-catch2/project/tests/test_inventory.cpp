// ============================================================================
// Tests for the Inventory (sort-by-value + std::function filter).   (YOUR TASK)
//
// This is the STARTER. The Inventory type is already complete — YOUR job is to
// write the assertions. Each case below says what to prove: replace every
// `// TODO(stage 13): assert ...` with a real REQUIRE(...), then DELETE the
// `FAIL("implement me")` line so the case can go green. Until you do, `ctest` is
// RED on purpose. Reference: ../../solution/tests/test_inventory.cpp.
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
    // TODO(stage 13): assert inv.distinct_count() == 3
    // TODO(stage 13): assert count_of is 3 / 1 / 57 for Health Potion / Iron Sword / Gold Coin
    // TODO(stage 13): assert an unknown item ("Dragon Egg") has count 0 (absent, not an error)

    FAIL("implement me — write the assertions above, then delete this line");
}

TEST_CASE_METHOD(InventoryFixture, "adding an existing item stacks its count", "[inventory]") {
    inv.add(Item{"Health Potion", 15}, 2);
    // TODO(stage 13): assert count_of("Health Potion") == 5  (the count stacked)
    // TODO(stage 13): assert distinct_count() is STILL 3  (no new distinct entry)

    FAIL("implement me — write the assertions above, then delete this line");
}

TEST_CASE_METHOD(InventoryFixture, "total_value sums value * count", "[inventory]") {
    // TODO(stage 13): assert inv.total_value() == 222  (120*1 + 15*3 + 1*57)

    FAIL("implement me — write the assertions above, then delete this line");
}

TEST_CASE_METHOD(InventoryFixture, "sorted_by_value orders highest value first", "[inventory]") {
    const std::vector<Item> sorted = inv.sorted_by_value();
    // TODO(stage 13): assert sorted.size() == 3
    // TODO(stage 13): assert the order by value: sorted[0]=="Iron Sword" (120),
    //   sorted[1]=="Health Potion" (15), sorted[2]=="Gold Coin" (1)
    // TODO(stage 13): assert sorting returned a COPY — inv.items().front().name is
    //   still "Health Potion" (the inventory's own order is untouched)

    FAIL("implement me — write the assertions above, then delete this line");
}

TEST_CASE_METHOD(InventoryFixture, "filter keeps only matching items", "[inventory]") {
    // The predicate is a lambda passed as a std::function. Python: filter(pred, items).
    const std::vector<Item> valuable =
        inv.filter([](const Item& item) { return item.value >= 15; });
    // TODO(stage 13): assert valuable.size() == 2  (Iron Sword + Health Potion; Gold Coin drops)

    // A predicate that matches nothing yields an empty result (not an error).
    const std::vector<Item> none = inv.filter([](const Item&) { return false; });
    // TODO(stage 13): assert none.empty()

    FAIL("implement me — write the assertions above, then delete this line");
}
