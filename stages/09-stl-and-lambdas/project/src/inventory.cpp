// ============================================================================
// Stage 09 mini-project — Inventory (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement each method below using what the README taught
// (std::vector, std::unordered_map, std::sort + a lambda comparator, and a stored
// std::function predicate) so the demo in src/main.cpp prints the keyed / sorted /
// filtered output shown in the README.
//
// The declarations you are implementing against are in inventory.hpp. The starter
// already compiles and links (each body has a placeholder), but the output is wrong
// until you fill the TODOs in.
//
// A complete reference is in ../solution/src/inventory.cpp — try it yourself first.
//
// Build & run (from this stage folder):
//   cmake -S . -B build && cmake --build build
//   ./build/stage09_inventory
// ============================================================================
#include "inventory.hpp"

#include <algorithm>  // std::sort, std::copy_if
#include <iterator>   // std::back_inserter

void Inventory::add(const std::string& name, int value, int count) {
    // TODO(stage 09): if `name` is not already a key in counts_, push_back a new
    // Item{name, value} onto items_ so each DISTINCT item appears exactly once.
    // Then add `count` to counts_[name] (operator[] inserts 0 for a new key first).
}

int Inventory::total_value() const {
    // TODO(stage 09): sum each item's value * counts_.at(it.name) over items_.
    return 0;  // placeholder
}

int Inventory::count_of(const std::string& name) const {
    // TODO(stage 09): look `name` up in counts_ (find) and return its quantity,
    // or 0 if the key is not present.
    return 0;  // placeholder
}

std::vector<Item> Inventory::sorted_by_value(bool descending) const {
    // TODO(stage 09): copy items_, then std::sort the copy with a LAMBDA comparator
    // that captures `descending` (a.value < b.value ascending, a.value > b.value
    // descending). Return the sorted copy so insertion order is left untouched.
    return {};  // placeholder
}

std::vector<Item> Inventory::matching(const std::function<bool(const Item&)>& keep) const {
    // TODO(stage 09): return the items for which the stored predicate `keep` returns
    // true — e.g. std::copy_if(items_.begin(), items_.end(), std::back_inserter(out), keep).
    return {};  // placeholder
}
