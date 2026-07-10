// ============================================================================
// Stage 09 mini-project — Inventory (implementation) — REFERENCE SOLUTION
// ============================================================================
#include "inventory.hpp"

#include <algorithm>  // std::sort, std::copy_if
#include <iterator>   // std::back_inserter

void Inventory::add(const std::string& name, int value, int count) {
    // Is this a brand-new item, or one we already carry?
    // .count() on the map is our fast "is this key present?" check.
    if (counts_.count(name) == 0) {
        items_.push_back(Item{name, value});  // first time: record the distinct item
    }
    counts_[name] += count;  // operator[] inserts 0 for a new key, then adds
}

int Inventory::total_value() const {
    int total = 0;
    for (const Item& it : items_) {
        // .at() throws if the key is missing — here it can't be, since every item
        // in items_ was registered in counts_ by add().
        total += it.value * counts_.at(it.name);
    }
    return total;
}

int Inventory::count_of(const std::string& name) const {
    auto found = counts_.find(name);           // find returns an iterator...
    return found == counts_.end() ? 0 : found->second;  // ...== end() means "not present"
}

std::vector<Item> Inventory::sorted_by_value(bool descending) const {
    std::vector<Item> copy = items_;  // sort a copy; don't disturb insertion order

    // The lambda comparator answers "should a come before b?". Capturing
    // `descending` lets one lambda serve both directions.
    std::sort(copy.begin(), copy.end(), [descending](const Item& a, const Item& b) {
        return descending ? a.value > b.value : a.value < b.value;
    });
    return copy;
}

std::vector<Item> Inventory::matching(const std::function<bool(const Item&)>& keep) const {
    std::vector<Item> out;
    // copy_if appends each item that satisfies the stored predicate.
    std::copy_if(items_.begin(), items_.end(), std::back_inserter(out), keep);
    return out;
}
