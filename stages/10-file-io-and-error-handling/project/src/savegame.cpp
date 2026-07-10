// ============================================================================
// Stage 10 mini-project — Save/load (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement save() and load() using what the README taught
// (std::ofstream / std::ifstream with RAII, std::getline + std::stringstream to
// parse '|'-delimited fields, and std::optional to signal "missing or corrupt")
// so the demo in src/main.cpp proves both the round-trip AND the empty-optional
// path shown in the README.
//
// The declarations and the file format are in savegame.hpp. The starter already
// compiles and links (each body has a placeholder), but the round-trip fails until
// you fill the TODOs in.
//
// A complete reference is in ../solution/src/savegame.cpp — try it yourself first.
//
// Build & run (from this stage folder):
//   cmake -S . -B build && cmake --build build
//   cd build && ./stage10_save_load     # run from build/ so savegame.txt lands there
// ============================================================================
#include "savegame.hpp"

#include <fstream>
#include <sstream>

bool save(const std::string& path, const SaveData& data) {
    // TODO(stage 10): open an std::ofstream on `path`; return false if it failed to
    // open. Write one "STATS|hp|attack|defense" line, then one
    // "ITEM|name|value|count" line per item in data.items (look the count up in
    // data.counts). Return true on success (RAII closes the file at return).
    return false;  // placeholder
}

std::optional<SaveData> load(const std::string& path) {
    // TODO(stage 10): open an std::ifstream on `path`; return std::nullopt if the
    // file is MISSING. Read it line by line with std::getline; split each line on
    // '|' with an std::stringstream. Dispatch on the first field ("STATS" / "ITEM"),
    // parsing the numeric fields with std::stoi (wrap in try/catch). Return
    // std::nullopt on any malformed line; otherwise return the filled SaveData.
    return std::nullopt;  // placeholder
}
