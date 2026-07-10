// ============================================================================
// Tests for the capstone save system: the round-trip property (what we save is
// what we load), now including the PLAYER POSITION, plus the empty-optional
// cases (missing / malformed file).
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include <cstdio>  // std::remove, std::fopen
#include <optional>
#include <string>

#include "eng/math.hpp"
#include "inventory.hpp"
#include "save_data.hpp"

using game::Item;
using game::load;
using game::save;
using game::SaveData;
using game::Stats;

TEST_CASE("save then load round-trips stats, inventory AND position", "[saveload]") {
    const std::string path = "capstone_roundtrip.tmp";

    SaveData original;
    original.stats = Stats{30, 12, 8};
    original.player_pos = gmath::Vec2f{123.5f, 456.25f};  // exact powers-of-two fractions
    original.inventory.add(Item{"Health Potion", 15}, 3);
    original.inventory.add(Item{"Iron Sword", 120}, 1);

    REQUIRE(save(path, original));
    const std::optional<SaveData> loaded = load(path);

    REQUIRE(loaded.has_value());
    CHECK(loaded->stats.hp == 30);
    CHECK(loaded->stats.attack == 12);
    CHECK(loaded->stats.defense == 8);
    CHECK(loaded->player_pos.x == 123.5f);
    CHECK(loaded->player_pos.y == 456.25f);
    CHECK(loaded->inventory.distinct_count() == 2);
    CHECK(loaded->inventory.count_of("Health Potion") == 3);
    CHECK(loaded->inventory.count_of("Iron Sword") == 1);
    CHECK(loaded->inventory.total_value() == 120 + 15 * 3);

    std::remove(path.c_str());
}

TEST_CASE("load of a missing file returns nullopt", "[saveload]") {
    const std::optional<SaveData> missing = load("no_such_capstone_save.tmp");
    REQUIRE_FALSE(missing.has_value());
}

TEST_CASE("load of a malformed file returns nullopt", "[saveload]") {
    const std::string path = "capstone_malformed.tmp";
    {
        std::FILE* f = std::fopen(path.c_str(), "w");
        REQUIRE(f != nullptr);
        std::fputs("this is not a save file\n", f);
        std::fclose(f);
    }

    const std::optional<SaveData> bad = load(path);
    REQUIRE_FALSE(bad.has_value());

    std::remove(path.c_str());
}
