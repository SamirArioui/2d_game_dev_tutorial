// Exercise 02 — solution
#include <catch2/catch_test_macros.hpp>

#include <vector>

TEST_CASE("a vector's size changes are isolated per section", "[ex02]") {
    std::vector<int> v(5);
    REQUIRE(v.size() == 5);

    // Catch2 runs the case body ONCE PER leaf section. Both sections below start
    // from the exact same freshly-built `v` of size 5.
    SECTION("growing") {
        v.push_back(99);
        REQUIRE(v.size() == 6);
    }
    SECTION("shrinking") {
        // If setup leaked from "growing", v would be size 6 here — it isn't.
        v.resize(2);
        REQUIRE(v.size() == 2);
    }
}
