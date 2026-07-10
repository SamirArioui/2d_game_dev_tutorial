// ============================================================================
// Stage 09 mini-project — Inventory (demo) — REFERENCE SOLUTION
// ============================================================================
//
// Builds an inventory, then shows the three headline skills of the stage:
//   1. keyed counts via std::unordered_map (count_of / total_value)
//   2. sorting with a lambda comparator (sorted_by_value)
//   3. filtering with a STORED std::function predicate (matching)
//
// This is the REFERENCE SOLUTION. The starter you implement lives two levels up
// in ../../src (fill in inventory.cpp). Build this finished version on its own:
//   cmake -S project/solution -B build-solution && cmake --build build-solution
//   ./build-solution/stage09_inventory
// ============================================================================
#include "inventory.hpp"

#include <functional>
#include <iostream>

namespace {

// Small helper so the demo output stays tidy.
void print_items(const std::vector<Item>& items) {
    for (const Item& it : items) {
        std::cout << "  " << it.name << " (value " << it.value << ")\n";
    }
}

}  // namespace

int main() {
    Inventory inv;

    // add(name, value, count). Note the repeated "Arrow" — it stays one entry in
    // the item list but its count climbs in the unordered_map.
    inv.add("Health Potion", 25, 3);
    inv.add("Iron Sword", 120);
    inv.add("Arrow", 5, 40);
    inv.add("Arrow", 5, 20);  // same key: bumps count to 60, no duplicate entry
    inv.add("Shield", 60);

    std::cout << "=== Inventory ===\n";
    for (const Item& it : inv.items()) {
        std::cout << "  " << it.name << " x" << inv.count_of(it.name) << " (value "
                  << it.value << " each)\n";
    }
    std::cout << "total value (value * quantity): " << inv.total_value() << "\n\n";

    // --- 2. Sort by value with a lambda comparator ---
    std::cout << "=== Sorted by value (descending) ===\n";
    print_items(inv.sorted_by_value(/*descending=*/true));
    std::cout << "\n";

    // --- 3. Filter with a stored std::function predicate ---
    // The rule lives in a variable, so it could be chosen at runtime (a menu, a
    // config file, a difficulty setting) rather than hard-coded into a loop.
    const int min_value = 50;
    std::function<bool(const Item&)> is_valuable = [min_value](const Item& it) {
        return it.value >= min_value;
    };

    std::cout << "=== Items worth >= " << min_value << " ===\n";
    print_items(inv.matching(is_valuable));

    return 0;
}
