#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace game {

// A single stackable item. Plain data — like a small dataclass in Python.
struct Item {
    std::string name;
    int value;  // e.g. gold value; used for sorting
};

// A bag of items. Internally keeps two containers in sync:
//   - a vector of the DISTINCT items in insertion order, and
//   - a map from item name -> how many the player holds.
// This mirrors the STL stage: std::vector + std::unordered_map (Python: list + dict).
class Inventory {
public:
    // Add `count` copies of `item`. If an item with that name already exists its
    // count increases; the item is only appended to the vector the first time.
    void add(const Item& item, int count = 1);

    // How many of `name` the player holds (0 if none). `const`: cannot mutate.
    int count_of(const std::string& name) const;

    // Number of DISTINCT items (i.e. vector size).
    int distinct_count() const;

    // Sum of value*count across everything.
    int total_value() const;

    // A COPY of the items, sorted by value (highest first). Returning a copy keeps
    // the caller from disturbing our internal order.
    std::vector<Item> sorted_by_value() const;

    // A COPY of the items for which `keep(item)` is true. The std::function lets the
    // caller pass any predicate — a lambda, a free function, etc. Python equivalent:
    // `[i for i in items if keep(i)]`.
    std::vector<Item> filter(const std::function<bool(const Item&)>& keep) const;

    // Read-only view of the raw items, for tests and save/load.
    const std::vector<Item>& items() const { return items_; }

private:
    std::vector<Item> items_;
    std::unordered_map<std::string, int> counts_;
};

}  // namespace game
