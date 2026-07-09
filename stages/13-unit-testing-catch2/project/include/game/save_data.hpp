#pragma once

#include <optional>
#include <string>

#include "game/inventory.hpp"

namespace game {

// The character's core stats. Plain data.
struct Stats {
    int hp;
    int attack;
    int defense;
};

// Everything a save file holds: the stats plus the inventory.
struct SaveData {
    Stats stats;
    Inventory inventory;
};

// Write `data` to `path`. Returns true on success (file opened & fully written).
bool save(const std::string& path, const SaveData& data);

// Try to read a SaveData back from `path`.
//   - returns the data on success,
//   - returns std::nullopt if the file is missing or malformed.
// std::optional is C++'s "a value OR nothing" — like a Python function returning
// `SaveData | None`, except the emptiness is part of the return TYPE, so the
// caller is forced to check before using it.
std::optional<SaveData> load(const std::string& path);

}  // namespace game
