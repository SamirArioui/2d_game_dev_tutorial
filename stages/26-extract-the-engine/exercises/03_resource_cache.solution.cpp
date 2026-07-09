// Exercise 03 — ResourceManager caching (solution)
#include <catch2/catch_test_macros.hpp>

#include <string>

#include "eng/resource_manager.hpp"

namespace {

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

    REQUIRE(&first == &again);       // same cached object, stable address
    REQUIRE(assets.load_count() == 1);  // loaded once, served from cache after

    assets.get("enemy.png");
    REQUIRE(assets.size() == 2);        // two distinct resources cached
    REQUIRE(assets.load_count() == 2);  // one load per distinct path
}
