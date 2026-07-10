// ============================================================================
// Stage 12 mini-project — console game, reorganized as library + executable
// ============================================================================
//
// This file is the `app` EXECUTABLE target. All the game logic lives in the
// separate `game` LIBRARY target (see project/src). The app just:
//   - builds a character (stats + inventory) using the library,
//   - formats everything for the console with fmt::format / fmt::print, and
//   - saves + reloads the data to prove the whole library works together.
//
// `fmt` is an EXTERNAL dependency pulled in by CMake's FetchContent (see
// CMakeLists.txt) — the C++ analogue of `pip install fmt` inside your build.
// fmt::format is essentially Python's str.format / f-strings for C++:
//   Python:  f"{name:<16} x{count:<3}"
//   C++:     fmt::format("{:<16} x{:<3}", name, count)
// ============================================================================

#include <fmt/format.h>  // fmt::format and fmt::print

#include <optional>
#include <string>

#include "game/game_math.hpp"
#include "game/inventory.hpp"
#include "game/save_data.hpp"

using namespace game;

int main() {
    // --- Build the character using the `game` library -----------------------
    const Stats stats{30, 12, 8};

    Inventory inv;
    inv.add(Item{"Health Potion", 15}, 3);
    inv.add(Item{"Iron Sword", 120}, 1);
    inv.add(Item{"Gold Coin", 1}, 57);

    // --- Format it with fmt -------------------------------------------------
    // The mini-language after ':' is like Python's: fill+align+width.
    //   {:=^40} -> fill '=', center, width 40
    //   {:<10}  -> left-align, width 10
    //   {:>3}   -> right-align, width 3
    fmt::print("{:=^40}\n", " CHARACTER SHEET ");
    fmt::print("{:<10}{:>3}\n", "HP", stats.hp);
    fmt::print("{:<10}{:>3}\n", "Attack", stats.attack);
    fmt::print("{:<10}{:>3}\n", "Defense", stats.defense);

    fmt::print("{:-^40}\n", " INVENTORY (by value) ");
    for (const Item& item : inv.sorted_by_value()) {
        fmt::print("  {:<16} x{:<3} ({} gold each)\n", item.name, inv.count_of(item.name),
                   item.value);
    }
    fmt::print("{:-^40}\n", "");
    fmt::print("Total inventory value: {} gold\n", inv.total_value());

    // fmt::format returns a std::string instead of printing — handy for building
    // a message to log or return. {:.2f} = 2 decimals, exactly like Python.
    const std::string dist_line =
        fmt::format("Distance (0,0)->(3,4) = {:.2f}", distance(0.0f, 0.0f, 3.0f, 4.0f));
    fmt::print("{}\n", dist_line);
    fmt::print("Half HP (lerp 0->{} @0.5) = {:.1f}\n", stats.hp,
               lerp(0.0f, static_cast<float>(stats.hp), 0.5f));

    // --- Round-trip through the save/load part of the library ---------------
    const std::string path = "savegame.txt";
    if (save(path, SaveData{stats, inv})) {
        if (const std::optional<SaveData> loaded = load(path)) {
            fmt::print("Saved and reloaded {} distinct item(s) from '{}'.\n",
                       loaded->inventory.distinct_count(), path);
        }
    }
    return 0;
}
