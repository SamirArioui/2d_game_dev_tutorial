// ============================================================================
// Stage 09 mini-project — Inventory (header) — REFERENCE SOLUTION
// ============================================================================
//
// This exercises EVERY concept from the stage:
//   - std::vector<Item>                       — the list of distinct items
//   - std::unordered_map<std::string,int>     — name -> how many we hold (dict analog)
//   - std::string as a key
//   - <algorithm> (std::sort) + a lambda comparator (sorted_by_value)
//   - a STORED std::function<bool(const Item&)> predicate (matching)
//
// The two containers are kept in sync: `items_` holds one entry per distinct item,
// and `counts_` maps that item's name to the quantity carried.
// ============================================================================
#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// Shared course type — a single inventory item and its unit value.
struct Item {
    std::string name;
    int value;
};

class Inventory {
public:
    // Add `count` copies of an item. Adding a name we already hold just bumps the
    // count; the item appears only once in items_.
    void add(const std::string& name, int value, int count = 1);

    // Total worth = each item's value times how many we carry.
    int total_value() const;

    // How many of `name` we hold (0 if none). Uses the unordered_map for O(1) lookup.
    int count_of(const std::string& name) const;

    // A COPY of the items sorted by value (ascending, or descending). Returns a
    // copy so the caller can sort a view without disturbing insertion order.
    std::vector<Item> sorted_by_value(bool descending = false) const;

    // The items for which `keep` returns true. The caller supplies the rule as a
    // stored std::function, so the same method drives "value >= N", "name is X", etc.
    std::vector<Item> matching(const std::function<bool(const Item&)>& keep) const;

    // Read-only access to the underlying items, in insertion order.
    const std::vector<Item>& items() const { return items_; }

private:
    std::vector<Item> items_;                     // distinct items, insertion order
    std::unordered_map<std::string, int> counts_; // name -> quantity carried
};
