// ============================================================================
// Stage 27 capstone — Inventory (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the game::Inventory methods declared in
// inventory.hpp — the same contract you built in stages 09-13, revived for the
// capstone. The starter already compiles and links (each body has a placeholder),
// but the inventory tests in tests/ are RED until you fill the TODOs in.
//
// A complete reference is in ../solution/src/inventory.cpp — try it yourself first.
// ============================================================================
#include "inventory.hpp"

#include <algorithm>

namespace game {

void Inventory::add(const Item& item, int count) {
    // TODO(stage 27): if `item.name` is new, record it in BOTH items_ (to keep
    // insertion order) and counts_; otherwise just add `count` to its running total.
    (void)item;
    (void)count;
}

int Inventory::count_of(const std::string& name) const {
    // TODO(stage 27): return the stored count for `name`, or 0 if it's absent.
    (void)name;
    return 0;  // placeholder
}

int Inventory::distinct_count() const {
    // TODO(stage 27): return how many DISTINCT items are in the bag.
    return 0;  // placeholder
}

int Inventory::total_value() const {
    // TODO(stage 27): sum item.value * count_of(item.name) over items_.
    return 0;  // placeholder
}

std::vector<Item> Inventory::sorted_by_value() const {
    // TODO(stage 27): return a COPY of items_ sorted by value, highest first
    // (std::sort with a lambda). The bag's own order must stay untouched.
    return {};  // placeholder
}

std::vector<Item> Inventory::filter(const std::function<bool(const Item&)>& keep) const {
    // TODO(stage 27): return the items for which the predicate `keep` returns true.
    (void)keep;
    return {};  // placeholder
}

}  // namespace game
