// ============================================================================
// Stage 10 mini-project — Save/load (header) — REFERENCE SOLUTION
// ============================================================================
//
// Persists the stage-9 inventory PLUS a Stats block to a line-based text file,
// and loads it back. Exercises every concept of the stage:
//   - std::ofstream to write, std::ifstream to read (RAII auto-close)
//   - std::getline + std::stringstream to parse each line into typed fields
//   - std::optional<SaveData> as the loader's return type: EMPTY on a missing or
//     corrupt file (the "maybe absent" result — C++'s answer to returning None)
//
// File format (one record per line, '|'-delimited so item names may contain spaces):
//   STATS|<hp>|<attack>|<defense>
//   ITEM|<name>|<value>|<count>
//   ITEM|<name>|<value>|<count>
//   ...
// ============================================================================
#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// Shared course types (carried locally so this stage is self-contained).
struct Item {
    std::string name;
    int value;
};

struct Stats {
    int hp;
    int attack;
    int defense;
};

// The whole save payload: a Stats block plus an inventory (distinct items in a
// vector, quantities keyed by name in an unordered_map — the stage-9 shape).
struct SaveData {
    Stats stats{};
    std::vector<Item> items;
    std::unordered_map<std::string, int> counts;
};

// Write `data` to `path`. Returns false only if the file couldn't be opened.
bool save(const std::string& path, const SaveData& data);

// Read a SaveData from `path`. Returns an EMPTY optional if the file is missing
// or any line is malformed — never a half-populated struct.
std::optional<SaveData> load(const std::string& path);
