// ===========================================================================
// Tests for ResourceManager<T> — the whole point is: LOAD ONCE, then cache.
//
// We can't (and shouldn't) test this with real sf::Texture files here — instead we hand the
// manager a lightweight fake resource that (a) satisfies the `bool loadFromFile(const string&)`
// contract and (b) COUNTS how many times it was actually loaded. That lets us assert the manager
// really caches, with no SFML and no disk.
// ===========================================================================

#include <string>

#include <catch2/catch_test_macros.hpp>

#include "res/resource_manager.hpp"

namespace {

// A stand-in for sf::Texture. It records every load so the tests can prove caching works.
struct CountingResource {
    static int total_loads;  // across ALL instances, reset at the start of each test

    bool loadFromFile(const std::string& path) {
        ++total_loads;
        // Pretend a specific path is a missing/corrupt file so we can test the failure path.
        return path != "does_not_exist";
    }
};

int CountingResource::total_loads = 0;

}  // namespace

TEST_CASE("get() loads on first use and caches thereafter", "[resource]") {
    CountingResource::total_loads = 0;
    res::ResourceManager<CountingResource> manager;

    CountingResource& first = manager.get("hero.png");
    REQUIRE(CountingResource::total_loads == 1);  // loaded exactly once

    CountingResource& again = manager.get("hero.png");
    REQUIRE(CountingResource::total_loads == 1);  // NOT reloaded — served from cache
    REQUIRE(&first == &again);                    // and it's the SAME instance (same address)

    REQUIRE(manager.is_cached("hero.png"));
    REQUIRE(manager.size() == 1);
}

TEST_CASE("different paths are cached independently", "[resource]") {
    CountingResource::total_loads = 0;
    res::ResourceManager<CountingResource> manager;

    manager.get("a.png");
    manager.get("b.png");
    manager.get("a.png");  // cached

    REQUIRE(CountingResource::total_loads == 2);  // a and b loaded once each; the third is a hit
    REQUIRE(manager.size() == 2);
    REQUIRE(manager.is_cached("a.png"));
    REQUIRE(manager.is_cached("b.png"));
    REQUIRE_FALSE(manager.is_cached("c.png"));
}

TEST_CASE("a failed load throws and is not cached", "[resource]") {
    CountingResource::total_loads = 0;
    res::ResourceManager<CountingResource> manager;

    REQUIRE_THROWS_AS(manager.get("does_not_exist"), std::runtime_error);
    REQUIRE(manager.size() == 0);
    REQUIRE_FALSE(manager.is_cached("does_not_exist"));
}

TEST_CASE("clear() empties the cache", "[resource]") {
    CountingResource::total_loads = 0;
    res::ResourceManager<CountingResource> manager;

    manager.get("a.png");
    REQUIRE(manager.size() == 1);
    manager.clear();
    REQUIRE(manager.size() == 0);

    // After clearing, the next get() must load again.
    manager.get("a.png");
    REQUIRE(CountingResource::total_loads == 2);
}
