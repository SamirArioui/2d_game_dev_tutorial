// Exercise 01 — Memoization (easy)
//
// A resource manager is really just MEMOIZATION applied to "load this file": do the expensive
// work once per key, then serve the stored answer. Warm up with the plain version.
//
// Python comparison: `@functools.lru_cache`, or a hand-rolled `if k not in cache: cache[k]=f(k)`.
//
// TODO: implement `cached_length` so it computes slow_length(path) only the FIRST time it sees a
//       given path, and returns the stored result on repeat calls.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_memoize.cpp -o ex && ./ex

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

int slow_calls = 0;

// Pretend this is expensive (like reading a file from disk).
int slow_length(const std::string& path) {
    ++slow_calls;
    return static_cast<int>(path.size());
}

std::unordered_map<std::string, int> cache;

int cached_length(const std::string& path) {
    // TODO:
    //   1. if `path` is already in `cache`, return the stored value;
    //   2. otherwise call slow_length(path), store it in `cache`, and return it.
    (void)path;
    return 0;
}

int main() {
    assert(cached_length("hero.png") == 8);
    assert(slow_calls == 1);

    assert(cached_length("hero.png") == 8);  // served from cache
    assert(slow_calls == 1);                 // slow_length was NOT called again

    assert(cached_length("tiles/grass.png") == 15);
    assert(slow_calls == 2);                 // a new key does the slow work once

    std::cout << "memoization works\n";
    return 0;
}
