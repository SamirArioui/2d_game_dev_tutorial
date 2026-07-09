// Exercise 01 — Memoization (solution)
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

int slow_calls = 0;

int slow_length(const std::string& path) {
    ++slow_calls;
    return static_cast<int>(path.size());
}

std::unordered_map<std::string, int> cache;

int cached_length(const std::string& path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
        return it->second;  // cache hit
    }
    const int value = slow_length(path);  // cache miss: do the slow work once
    cache[path] = value;
    return value;
}

int main() {
    assert(cached_length("hero.png") == 8);
    assert(slow_calls == 1);

    assert(cached_length("hero.png") == 8);
    assert(slow_calls == 1);

    assert(cached_length("tiles/grass.png") == 15);
    assert(slow_calls == 2);

    std::cout << "memoization works\n";
    return 0;
}
