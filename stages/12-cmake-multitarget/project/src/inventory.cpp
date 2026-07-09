#include "game/inventory.hpp"

#include <algorithm>

namespace game {

void Inventory::add(const Item& item, int count) {
    auto it = counts_.find(item.name);
    if (it == counts_.end()) {
        // First time we've seen this item: record it in BOTH containers.
        items_.push_back(item);
        counts_[item.name] = count;
    } else {
        // Already known: just bump the running count.
        it->second += count;
    }
}

int Inventory::count_of(const std::string& name) const {
    auto it = counts_.find(name);
    return it == counts_.end() ? 0 : it->second;
}

int Inventory::distinct_count() const {
    return static_cast<int>(items_.size());
}

int Inventory::total_value() const {
    int total = 0;
    for (const Item& item : items_) {
        total += item.value * count_of(item.name);
    }
    return total;
}

std::vector<Item> Inventory::sorted_by_value() const {
    std::vector<Item> result = items_;  // copy, then sort the copy
    std::sort(result.begin(), result.end(),
              [](const Item& a, const Item& b) { return a.value > b.value; });
    return result;
}

std::vector<Item> Inventory::filter(const std::function<bool(const Item&)>& keep) const {
    std::vector<Item> result;
    for (const Item& item : items_) {
        if (keep(item)) {
            result.push_back(item);
        }
    }
    return result;
}

}  // namespace game
