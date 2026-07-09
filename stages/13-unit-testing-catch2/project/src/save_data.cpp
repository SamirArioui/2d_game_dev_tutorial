#include "game/save_data.hpp"

#include <fstream>

namespace game {

// File format (whitespace separated):
//   hp attack defense
//   N                          <- number of distinct item lines that follow
//   <count> <value> <name>     <- repeated N times; `name` is the rest of the line
//
// Putting `name` last lets it contain spaces, e.g. "Health Potion".

bool save(const std::string& path, const SaveData& data) {
    std::ofstream out(path);
    if (!out) {
        return false;
    }
    out << data.stats.hp << ' ' << data.stats.attack << ' ' << data.stats.defense << '\n';

    const std::vector<Item>& items = data.inventory.items();
    out << items.size() << '\n';
    for (const Item& item : items) {
        out << data.inventory.count_of(item.name) << ' ' << item.value << ' ' << item.name << '\n';
    }
    // The stream converts to false if any write failed (disk full, etc.).
    return static_cast<bool>(out);
}

std::optional<SaveData> load(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return std::nullopt;  // file missing / unreadable
    }

    SaveData data{};
    if (!(in >> data.stats.hp >> data.stats.attack >> data.stats.defense)) {
        return std::nullopt;  // malformed header
    }

    int item_lines = 0;
    if (!(in >> item_lines) || item_lines < 0) {
        return std::nullopt;
    }

    for (int i = 0; i < item_lines; ++i) {
        int count = 0;
        int value = 0;
        if (!(in >> count >> value)) {
            return std::nullopt;
        }
        in.get();  // consume the single space before the name
        std::string name;
        if (!std::getline(in, name) || name.empty()) {
            return std::nullopt;
        }
        data.inventory.add(Item{name, value}, count);
    }

    return data;
}

}  // namespace game
