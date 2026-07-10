#pragma once

// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
// ============================================================================
// game::Inventory — the SAME inventory contract from stages 09-13, revived for
// the capstone (the course throughline: character sheet -> inventory -> save/load
// -> ... -> this game reuses inventory + save/load).
//
// It lives in namespace `game` (not `eng`): an inventory is GAME logic, not an
// engine subsystem. The engine knows nothing about items. This is the healthy
// boundary — the engine is generic, the game is specific.
// ============================================================================

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace game {

// A single stackable item — plain data (like a small Python dataclass).
struct Item {
    std::string name;
    int value{0};  // e.g. worth in gold; used for sorting/HUD
};

// A bag of items, keeping a vector of DISTINCT items (insertion order) in sync
// with a name -> count map. Mirrors the STL stage: std::vector + unordered_map.
class Inventory {
public:
    void add(const Item& item, int count = 1);

    int count_of(const std::string& name) const;
    int distinct_count() const;
    int total_value() const;

    std::vector<Item> sorted_by_value() const;
    std::vector<Item> filter(const std::function<bool(const Item&)>& keep) const;

    const std::vector<Item>& items() const { return items_; }

private:
    std::vector<Item> items_;
    std::unordered_map<std::string, int> counts_;
};

}  // namespace game
