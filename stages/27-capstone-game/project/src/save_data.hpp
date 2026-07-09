#pragma once

// ============================================================================
// game::Stats / game::SaveData + save/load — the stage-10/13 save system,
// extended for the capstone with the player's WORLD POSITION so "load" drops you
// back exactly where you saved.
//
// std::optional<SaveData> load(...) is the C++ "value OR nothing": the caller is
// forced by the type to handle the missing/corrupt-file case (Python's
// `SaveData | None`, but checked at compile time).
// ============================================================================

#include <optional>
#include <string>

#include "eng/math.hpp"  // gmath::Vec2f — the engine's pure vector type (no SFML)
#include "inventory.hpp"

namespace game {

// Core character stats — plain data (the contract from stage 03 onward).
struct Stats {
    int hp{0};
    int attack{0};
    int defense{0};
};

// Everything one save file holds: stats, the inventory, and where the player is.
struct SaveData {
    Stats stats;
    Inventory inventory;
    gmath::Vec2f player_pos;
};

// Write `data` to `path`. Returns true on a fully-written file.
bool save(const std::string& path, const SaveData& data);

// Read a SaveData back. Returns std::nullopt if the file is missing or malformed.
std::optional<SaveData> load(const std::string& path);

}  // namespace game
