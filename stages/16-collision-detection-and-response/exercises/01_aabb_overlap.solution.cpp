// Exercise 01 — do two AABBs overlap? (solution).

#include <catch2/catch_test_macros.hpp>

struct AABB {
    float x;
    float y;
    float w;
    float h;
};

bool overlaps(const AABB& a, const AABB& b) {
    return a.x < b.x + b.w && a.x + a.w > b.x &&  // overlap on x
           a.y < b.y + b.h && a.y + a.h > b.y;    // overlap on y
}

TEST_CASE("overlapping boxes report true") {
    REQUIRE(overlaps(AABB{0, 0, 10, 10}, AABB{5, 5, 10, 10}));
}

TEST_CASE("separated boxes report false") {
    REQUIRE_FALSE(overlaps(AABB{0, 0, 10, 10}, AABB{20, 0, 10, 10}));
    REQUIRE_FALSE(overlaps(AABB{0, 0, 10, 10}, AABB{0, 50, 10, 10}));
}

TEST_CASE("edge-touching boxes are NOT overlapping (strict)") {
    REQUIRE_FALSE(overlaps(AABB{0, 0, 10, 10}, AABB{10, 0, 10, 10}));
}
