// Exercise 03 — unordered_map vs map (medium)
//
// Python's dict is a HASH map: near-instant lookup, and (as of 3.7) it happens
// to preserve insertion order. The DIRECT C++ equivalent for "fast key/value
// lookup" is std::unordered_map, NOT std::map:
//
//   std::unordered_map<K,V>  — hash table. O(1) average lookup. Iteration order
//                              is UNSPECIFIED (do not rely on it).
//   std::map<K,V>            — balanced binary tree. O(log n) lookup. Always
//                              iterates in SORTED key order — something dict
//                              does not give you for free.
//
// Rule of thumb: reach for unordered_map (it's the dict analog). Choose map only
// when you specifically want keys visited in sorted order.
//
// Both are used like a dict: m[key] = v; m.at(key); m.count(key); m[key]++.
//
// Fill in the TODOs. Expected output (the sorted map block is the checkable part;
// the unordered block may print in any order):
//   -- counts (unordered_map, order may vary) --
//   ... three lines, any order ...
//   -- counts (map, sorted by key) --
//   arrow: 10
//   potion: 3
//   sword: 1
//   have a sword? yes
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 03_map_vs_unordered_map.cpp -o ex && ./ex

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    // A stream of items we "picked up" during play.
    std::vector<std::string> pickups = {"potion", "arrow", "arrow", "sword",
                                         "potion", "arrow", "potion", "arrow",
                                         "arrow", "arrow", "arrow", "arrow",
                                         "arrow", "arrow"};

    std::unordered_map<std::string, int> counts;

    // TODO 1: count each pickup. For a missing key, operator[] inserts it with a
    //         value-initialized 0 first, so counts[name]++ just works — same as
    //         Python's counts[name] = counts.get(name, 0) + 1.

    std::cout << "-- counts (unordered_map, order may vary) --\n";
    // TODO 2: print each  name: count  using a structured-binding range-for:
    //         for (const auto& [name, n] : counts) { ... }
    //         ([name, n] unpacks the key/value pair — like Python's items().)

    // TODO 3: copy `counts` into a std::map<std::string,int> called `sorted`.
    //         A map can be constructed straight from another map's begin/end:
    //         std::map<std::string,int> sorted(counts.begin(), counts.end());

    std::cout << "-- counts (map, sorted by key) --\n";
    // TODO 4: print each name: count from `sorted` (now guaranteed alphabetical).

    // TODO 5: print  have a sword? yes/no  using counts.count("sword")
    //         (.count returns 1 if the key exists, 0 if not — there are no dup keys).

    return 0;
}
