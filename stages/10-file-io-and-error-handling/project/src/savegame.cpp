// ============================================================================
// Stage 10 mini-project — Save/load (implementation)
// ============================================================================
#include "savegame.hpp"

#include <fstream>
#include <sstream>

bool save(const std::string& path, const SaveData& data) {
    std::ofstream out(path);  // opens/creates/truncates; closed by RAII at return
    if (!out) {
        return false;  // couldn't open (bad path, no permission): report via bool
    }

    // One STATS line...
    out << "STATS|" << data.stats.hp << "|" << data.stats.attack << "|" << data.stats.defense
        << "\n";

    // ...then one ITEM line per distinct item, with its carried quantity.
    for (const Item& it : data.items) {
        int count = 1;
        auto found = data.counts.find(it.name);
        if (found != data.counts.end()) {
            count = found->second;
        }
        out << "ITEM|" << it.name << "|" << it.value << "|" << count << "\n";
    }

    return true;
}  // `out` destructor flushes + closes the file here

std::optional<SaveData> load(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return std::nullopt;  // MISSING file -> empty optional (the "None" path)
    }

    SaveData data;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;  // tolerate blank lines
        }

        std::stringstream ss(line);
        std::string tag;
        std::getline(ss, tag, '|');  // the record type is the first '|'-field

        if (tag == "STATS") {
            std::string hp, attack, defense;
            // Each field must be present; a missing '|' section fails the getline.
            if (!std::getline(ss, hp, '|') || !std::getline(ss, attack, '|') ||
                !std::getline(ss, defense)) {
                return std::nullopt;  // CORRUPT line -> empty optional
            }
            // std::stoi throws std::invalid_argument on non-numeric text; catch it
            // and turn the exception into our "absent" result so callers get one
            // uniform failure channel (an empty optional) regardless of the cause.
            try {
                data.stats = Stats{std::stoi(hp), std::stoi(attack), std::stoi(defense)};
            } catch (const std::exception&) {
                return std::nullopt;
            }
        } else if (tag == "ITEM") {
            std::string name, value, count;
            if (!std::getline(ss, name, '|') || !std::getline(ss, value, '|') ||
                !std::getline(ss, count)) {
                return std::nullopt;
            }
            try {
                data.items.push_back(Item{name, std::stoi(value)});
                data.counts[name] = std::stoi(count);
            } catch (const std::exception&) {
                return std::nullopt;
            }
        } else {
            return std::nullopt;  // unknown record tag -> treat the file as corrupt
        }
    }

    return data;  // implicitly wrapped into std::optional<SaveData>
}
