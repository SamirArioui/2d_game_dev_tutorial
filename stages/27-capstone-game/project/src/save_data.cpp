// ============================================================================
// Stage 27 capstone — save/load (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement save() and load() for a SaveData (stats +
// inventory + player position), extending the stage-10/13 save system with the
// player's world position. The starter already compiles and links (each body has
// a placeholder), but the save/load tests in tests/ are RED until you fill the
// TODOs in.
//
// A complete reference is in ../solution/src/save_data.cpp — try it yourself first.
// ============================================================================
#include "save_data.hpp"

#include <fstream>

namespace game {

// Suggested file format (whitespace separated):
//   hp attack defense
//   px py                        <- player world position (floats)
//   N                            <- number of distinct item lines that follow
//   <count> <value> <name>       <- repeated N times; `name` is the rest of the line
//
// `name` is last on its line so it may contain spaces, e.g. "Health Potion".

bool save(const std::string& path, const SaveData& data) {
    // TODO(stage 27): open an std::ofstream on `path` (return false if it won't
    // open). Write the stats line, then "px py", then the item count N, then one
    // "count value name" line per distinct item (count via data.inventory.count_of).
    // Return true only if every write succeeded.
    (void)path;
    (void)data;
    return false;  // placeholder
}

std::optional<SaveData> load(const std::string& path) {
    // TODO(stage 27): open an std::ifstream (return std::nullopt if the file is
    // MISSING). Read the stats, then the position, then N; for each of the N items
    // read count + value, consume the separating space, std::getline the rest of
    // the line as the name, and inv.add(Item{name, value}, count). Return
    // std::nullopt on ANY malformed line; otherwise the filled SaveData.
    (void)path;
    return std::nullopt;  // placeholder
}

}  // namespace game
