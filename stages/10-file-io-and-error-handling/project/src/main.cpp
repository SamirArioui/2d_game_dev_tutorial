// ============================================================================
// Stage 10 mini-project — Save/load (demo)
// ============================================================================
//
// Proves the round-trip (save then load equals the original) AND the empty-optional
// path (loading a file that isn't there yields no value, handled gracefully).
//
// Build & run (from this stage folder):
//   cmake -S . -B build && cmake --build build
//   ./build/stage10_save_load
// ============================================================================
#include "savegame.hpp"

#include <iostream>

namespace {

void print_save(const SaveData& d) {
    std::cout << "  Stats: hp=" << d.stats.hp << " attack=" << d.stats.attack
              << " defense=" << d.stats.defense << "\n";
    for (const Item& it : d.items) {
        int count = d.counts.count(it.name) ? d.counts.at(it.name) : 0;
        std::cout << "  Item: " << it.name << " x" << count << " (value " << it.value << ")\n";
    }
}

bool same(const SaveData& a, const SaveData& b) {
    if (a.stats.hp != b.stats.hp || a.stats.attack != b.stats.attack ||
        a.stats.defense != b.stats.defense) {
        return false;
    }
    if (a.items.size() != b.items.size() || a.counts != b.counts) {
        return false;
    }
    for (std::size_t i = 0; i < a.items.size(); ++i) {
        if (a.items[i].name != b.items[i].name || a.items[i].value != b.items[i].value) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    // Build a save payload: a Stats block + an inventory (note the space in a name).
    SaveData original;
    original.stats = Stats{30, 12, 8};
    original.items = {{"Health Potion", 25}, {"Iron Sword", 120}, {"Arrow", 5}};
    original.counts = {{"Health Potion", 3}, {"Iron Sword", 1}, {"Arrow", 60}};

    const std::string path = "savegame.txt";

    std::cout << "=== Saving ===\n";
    if (!save(path, original)) {
        std::cerr << "error: could not write " << path << "\n";
        return 1;
    }
    print_save(original);

    // --- The round-trip: load it back and compare ---
    std::cout << "\n=== Loading " << path << " ===\n";
    std::optional<SaveData> loaded = load(path);
    if (!loaded) {  // an optional is truthy when it holds a value
        std::cerr << "error: save file missing or corrupt\n";
        return 1;
    }
    print_save(*loaded);  // *loaded / loaded.value() unwraps it
    std::cout << "round-trip matches original? " << (same(original, *loaded) ? "yes" : "no")
              << "\n";

    // --- The empty-optional path: a file that doesn't exist ---
    std::cout << "\n=== Loading a missing file ===\n";
    std::optional<SaveData> missing = load("no_such_save.txt");
    if (missing) {
        std::cout << "unexpectedly found data\n";
    } else {
        // This is the normal, graceful "no save yet — start a new game" branch.
        std::cout << "no save found -> starting a fresh game (empty optional handled)\n";
    }

    return 0;
}
