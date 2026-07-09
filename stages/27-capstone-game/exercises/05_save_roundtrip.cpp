// Exercise 05 — Save/load round-trip (medium-hard)
//
// Save a SaveData to a temp file, load it back, and confirm every field
// survived — including the player position added for the capstone.

#include <catch2/catch_test_macros.hpp>

#include <cstdio>
#include <optional>
#include <string>

#include "eng/math.hpp"
#include "inventory.hpp"
#include "save_data.hpp"

using game::Item;
using game::SaveData;
using game::Stats;

TEST_CASE("round-trip preserves stats, position and inventory", "[ex05]") {
    const std::string path = "ex05_roundtrip.tmp";

    SaveData original;
    original.stats = Stats{40, 9, 5};
    original.player_pos = gmath::Vec2f{200.0f, 128.0f};
    original.inventory.add(Item{"Gold Coin", 5}, 7);

    REQUIRE(game::save(path, original));
    const std::optional<SaveData> loaded = game::load(path);

    REQUIRE(loaded.has_value());

    // TODO: fill in the values you saved above.
    REQUIRE(loaded->stats.hp == 0);            // <-- fix
    REQUIRE(loaded->player_pos.x == 0.0f);     // <-- fix
    REQUIRE(loaded->inventory.count_of("Gold Coin") == 0);  // <-- fix

    std::remove(path.c_str());
}
