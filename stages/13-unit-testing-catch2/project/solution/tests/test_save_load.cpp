// ============================================================================
// Tests for save/load — the round-trip property and the empty-optional case.  (REFERENCE SOLUTION)
//
// This is the complete reference test file. The starter you fill in is one
// level up at ../../tests/test_save_load.cpp.
//
// Save/load DOES touch the outside world (a file), so it isn't as pure as the
// math functions. We still make it testable by:
//   - writing to a throwaway temp file the test owns, and
//   - checking the key PROPERTY (what we saved is what we load back) rather than
//     the exact bytes on disk.
// This is the "pure logic vs side-effects" theme: side-effects are testable, but
// they take a little more care (setup + cleanup) than a pure function does.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include <cstdio>    // std::remove (delete the temp file)
#include <optional>
#include <string>

#include "game/inventory.hpp"
#include "game/save_data.hpp"

using game::Inventory;
using game::Item;
using game::load;
using game::save;
using game::SaveData;
using game::Stats;

TEST_CASE("save then load returns equivalent data (round-trip)", "[saveload]") {
    const std::string path = "roundtrip_save.tmp";

    // Arrange: build a SaveData with some stats and a small inventory.
    SaveData original;
    original.stats = Stats{30, 12, 8};
    original.inventory.add(Item{"Health Potion", 15}, 3);
    original.inventory.add(Item{"Iron Sword", 120}, 1);

    // Act: write it, then read it back.
    REQUIRE(save(path, original));
    const std::optional<SaveData> loaded = load(path);

    // Assert: the optional is engaged, and every field survived the trip.
    REQUIRE(loaded.has_value());
    CHECK(loaded->stats.hp == 30);
    CHECK(loaded->stats.attack == 12);
    CHECK(loaded->stats.defense == 8);
    CHECK(loaded->inventory.distinct_count() == 2);
    CHECK(loaded->inventory.count_of("Health Potion") == 3);
    CHECK(loaded->inventory.count_of("Iron Sword") == 1);
    CHECK(loaded->inventory.total_value() == 120 + 15 * 3);

    std::remove(path.c_str());  // clean up so the next run starts fresh
}

TEST_CASE("load of a missing file returns an empty optional", "[saveload]") {
    // The key contract: load reports "nothing" via std::nullopt instead of
    // crashing or returning a half-built object. Python: the `None` branch.
    const std::optional<SaveData> missing = load("this_file_does_not_exist.tmp");
    REQUIRE_FALSE(missing.has_value());
}

TEST_CASE("load of a malformed file returns an empty optional", "[saveload]") {
    const std::string path = "malformed_save.tmp";

    {
        // Write garbage that can't be parsed as a header.
        std::FILE* f = std::fopen(path.c_str(), "w");
        REQUIRE(f != nullptr);
        std::fputs("not a valid save file\n", f);
        std::fclose(f);
    }

    const std::optional<SaveData> bad = load(path);
    REQUIRE_FALSE(bad.has_value());

    std::remove(path.c_str());
}
