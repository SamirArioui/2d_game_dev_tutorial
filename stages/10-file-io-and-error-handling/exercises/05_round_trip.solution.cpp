// Exercise 05 — Save/load round-trip and the empty-optional path (solution)
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

struct Stats {
    int hp;
    int attack;
    int defense;
};

void save_stats(const std::string& path, const Stats& s) {
    std::ofstream out(path);
    out << s.hp << " " << s.attack << " " << s.defense << "\n";
}

std::optional<Stats> load_stats(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return std::nullopt;
    }

    std::string line;
    if (!std::getline(in, line)) {
        return std::nullopt;
    }

    std::stringstream ss(line);
    Stats s{};
    if (!(ss >> s.hp >> s.attack >> s.defense)) {
        return std::nullopt;  // corrupt line
    }
    return s;
}

int main() {
    Stats original{30, 12, 8};
    save_stats("stats.txt", original);

    std::optional<Stats> loaded = load_stats("stats.txt");
    bool ok = loaded.has_value() && loaded->hp == original.hp &&
              loaded->attack == original.attack && loaded->defense == original.defense;
    std::cout << "round-trip ok? " << (ok ? "yes" : "no") << "\n";

    std::optional<Stats> missing = load_stats("does_not_exist.txt");
    std::cout << "missing file -> "
              << (missing.has_value() ? "UNEXPECTED value!" : "(empty optional), as expected")
              << "\n";

    return ok ? 0 : 1;
}
