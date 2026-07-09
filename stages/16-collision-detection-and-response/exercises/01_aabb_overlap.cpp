// Exercise 01 — do two AABBs overlap? (easy) — a Catch2 drill.
//
// An axis-aligned bounding box (AABB) is the workhorse collider: a rectangle
// that never rotates. Two AABBs overlap iff they overlap on BOTH the x and y
// axes. Each box here is stored as top-left (x, y) plus size (w, h); remember
// SFML's +y points DOWN, so "top" is the smaller y.
//
// The test (the "separating axis" idea in its simplest form): they overlap when
//   a.x < b.x+b.w  AND  a.x+a.w > b.x  AND  a.y < b.y+b.h  AND  a.y+a.h > b.y
//
// TODO: implement overlaps().
//
// Build & test (from exercises/):
//   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
//   cmake --build build --target 01_aabb_overlap
//   ./build/01_aabb_overlap          # starts RED

#include <catch2/catch_test_macros.hpp>

struct AABB {
    float x;  // top-left corner
    float y;
    float w;  // size
    float h;
};

bool overlaps(const AABB& a, const AABB& b) {
    // TODO: return true only if the boxes overlap on both axes.
    (void)a;
    (void)b;
    return false;
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
