// Exercise 04 — Sorting with a lambda (solution)
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Item {
    std::string name;
    int value;
};

void print_all(const std::vector<Item>& items) {
    for (const Item& it : items) {
        std::cout << it.name << " (" << it.value << ")\n";
    }
}

int main() {
    std::vector<Item> items = {{"Potion", 25}, {"Sword", 120}, {"Arrow", 5}};

    std::cout << "-- by value, ascending --\n";
    std::sort(items.begin(), items.end(),
              [](const Item& a, const Item& b) { return a.value < b.value; });
    print_all(items);

    std::cout << "-- by value, descending --\n";
    std::sort(items.begin(), items.end(),
              [](const Item& a, const Item& b) { return a.value > b.value; });
    print_all(items);

    return 0;
}
