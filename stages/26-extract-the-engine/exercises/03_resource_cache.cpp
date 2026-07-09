// Exercise 03 — ResourceManager caching (medium)
//
// eng::ResourceManager<T> works with ANY type that has
// `bool loadFromFile(const std::string&)`. Here we use a FAKE asset so we can
// test the caching with no SFML. Prove that asking for the same path twice loads
// it only ONCE.

#include <catch2/catch_test_macros.hpp>

#include <string>

#include "eng/resource_manager.hpp"

namespace {

// Stand-in for sf::Texture/sf::Font: it just records what path it "loaded".
struct FakeAsset {
    std::string path;
    bool loadFromFile(const std::string& p) {
        path = p;
        return true;
    }
};

}  // namespace

TEST_CASE("second get() of the same path is a cache hit", "[ex03]") {
    eng::ResourceManager<FakeAsset> assets;

    FakeAsset& first = assets.get("hero.png");
    FakeAsset& again = assets.get("hero.png");

    // TODO: get() returns a REFERENCE to the cached object. Are these the same
    // object? (Compare addresses.)
    REQUIRE(&first != &again);  // <-- fix

    // TODO: how many actual loads happened for one distinct path?
    REQUIRE(assets.load_count() == 0);  // <-- fix

    // A different path is a new load.
    assets.get("enemy.png");
    // TODO: now how many distinct resources are cached, and how many loads total?
    REQUIRE(assets.size() == 0);        // <-- fix
    REQUIRE(assets.load_count() == 0);  // <-- fix
}
