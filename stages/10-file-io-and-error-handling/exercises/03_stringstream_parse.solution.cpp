// Exercise 03 — Parsing lines with std::stringstream (solution)
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string stats_line = "30 12 8";
    std::stringstream ss(stats_line);

    int hp = 0, attack = 0, defense = 0;
    ss >> hp >> attack >> defense;  // whitespace-separated, converted by type
    std::cout << "stats: hp=" << hp << " attack=" << attack << " defense=" << defense << "\n";

    std::string item_line = "Health Potion|25|3";
    std::stringstream is(item_line);

    std::string name;
    std::string value_str;
    std::string count_str;
    std::getline(is, name, '|');       // stops at first '|', keeps the space in the name
    std::getline(is, value_str, '|');
    std::getline(is, count_str);       // no delimiter: takes the rest of the line

    int value = std::stoi(value_str);  // string -> int (throws if not a number)
    int count = std::stoi(count_str);
    std::cout << "item: name='" << name << "' value=" << value << " count=" << count << "\n";

    return 0;
}
