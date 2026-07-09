// Exercise 05 — Save/load round-trip and the empty-optional path (medium–hard)
//
// This is the pattern the mini-project scales up: write a struct to a text file,
// read it back, and prove the loaded value equals the original (a "round-trip").
// The loader returns std::optional<Stats> so that a MISSING or CORRUPT file yields
// an empty optional instead of a crash or a half-filled struct.
//
// Fill in the TODOs. Expected output:
//   round-trip ok? yes
//   missing file -> (empty optional), as expected
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 05_round_trip.cpp -o ex && ./ex

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

// Shared course type.
struct Stats {
    int hp;
    int attack;
    int defense;
};

// Serialize as a single line: "hp attack defense".
void save_stats(const std::string& path, const Stats& s) {
    std::ofstream out(path);
    out << s.hp << " " << s.attack << " " << s.defense << "\n";
}  // file flushed + closed here (RAII)

// Load returns EMPTY if the file is missing OR the line doesn't parse.
std::optional<Stats> load_stats(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return std::nullopt;  // file missing / unreadable — the "None" path
    }

    std::string line;
    if (!std::getline(in, line)) {
        return std::nullopt;  // empty file
    }

    std::stringstream ss(line);
    Stats s{};
    // TODO 1: extract hp, attack, defense from ss. If the extraction fails (!ss),
    //         return std::nullopt (corrupt line). Otherwise return s.

    return std::nullopt;  // <- replace with the real logic
}

int main() {
    Stats original{30, 12, 8};
    save_stats("stats.txt", original);

    std::optional<Stats> loaded = load_stats("stats.txt");

    // TODO 2: set `ok` to true only if loaded.has_value() AND all three fields
    //         match `original`. Print  round-trip ok? yes/no
    bool ok = false;

    // The empty-optional path: a file that doesn't exist must load as empty.
    std::optional<Stats> missing = load_stats("does_not_exist.txt");
    std::cout << "missing file -> "
              << (missing.has_value() ? "UNEXPECTED value!" : "(empty optional), as expected")
              << "\n";

    return ok ? 0 : 1;
}
