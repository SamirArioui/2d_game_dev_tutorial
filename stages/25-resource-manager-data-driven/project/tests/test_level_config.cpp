// Tests for the data-driven level parser. It reads from any istream, so we feed it in-memory
// strings — no files needed, and the parser proves it ignores comments/blank lines and reads the
// directives it knows.
#include <sstream>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "game/level_config.hpp"

using game::LevelConfig;
using game::parse_level;

TEST_CASE("parses title, size, and a list of entities", "[config]") {
    std::istringstream in(
        "# a level definition\n"
        "title  Green Hills\n"
        "size   640 480\n"
        "\n"
        "entity 100 200 tiles/grass.png\n"
        "entity 300 150 tiles/wall.png\n");

    const LevelConfig level = parse_level(in);

    REQUIRE(level.title == "Green Hills");  // multi-word title, trimmed
    REQUIRE(level.width == 640);
    REQUIRE(level.height == 480);
    REQUIRE(level.entities.size() == 2);
    REQUIRE(level.entities[0].texture == "tiles/grass.png");
    REQUIRE(level.entities[0].x == Catch::Approx(100.0f));
    REQUIRE(level.entities[0].y == Catch::Approx(200.0f));
    REQUIRE(level.entities[1].texture == "tiles/wall.png");
}

TEST_CASE("comments and blank lines are ignored", "[config]") {
    std::istringstream in(
        "\n"
        "   # full-line comment\n"
        "size 32 32   # trailing comment after a directive\n"
        "\n");

    const LevelConfig level = parse_level(in);
    REQUIRE(level.width == 32);
    REQUIRE(level.height == 32);
    REQUIRE(level.entities.empty());
}

TEST_CASE("unknown directives are skipped, not fatal", "[config]") {
    std::istringstream in(
        "title Test\n"
        "gravity 9.8\n"          // not a directive we understand (yet)
        "entity 0 0 a.png\n");

    const LevelConfig level = parse_level(in);
    REQUIRE(level.title == "Test");
    REQUIRE(level.entities.size() == 1);  // the unknown line didn't derail parsing
}

TEST_CASE("an empty stream yields an empty, default level", "[config]") {
    std::istringstream in("");
    const LevelConfig level = parse_level(in);
    REQUIRE(level.title.empty());
    REQUIRE(level.width == 0);
    REQUIRE(level.entities.empty());
}
