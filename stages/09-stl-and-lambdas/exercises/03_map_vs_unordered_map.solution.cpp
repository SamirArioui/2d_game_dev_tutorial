// Exercise 03 — unordered_map vs map (solution)
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> pickups = {"potion", "arrow", "arrow", "sword",
                                         "potion", "arrow", "potion", "arrow",
                                         "arrow", "arrow", "arrow", "arrow",
                                         "arrow", "arrow"};

    std::unordered_map<std::string, int> counts;
    for (const std::string& name : pickups) {
        counts[name]++;  // operator[] default-inserts 0 for a new key, then ++
    }

    std::cout << "-- counts (unordered_map, order may vary) --\n";
    for (const auto& [name, n] : counts) {  // C++17 structured bindings
        std::cout << name << ": " << n << "\n";
    }

    // Feeding an unordered_map's pairs into a std::map re-sorts them by key.
    std::map<std::string, int> sorted(counts.begin(), counts.end());

    std::cout << "-- counts (map, sorted by key) --\n";
    for (const auto& [name, n] : sorted) {
        std::cout << name << ": " << n << "\n";
    }

    std::cout << "have a sword? " << (counts.count("sword") ? "yes" : "no") << "\n";

    return 0;
}
