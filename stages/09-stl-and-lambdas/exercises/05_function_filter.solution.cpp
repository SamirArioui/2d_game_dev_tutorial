// Exercise 05 — Stored std::function filter (solution)
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Item {
    std::string name;
    int value;
};

std::vector<Item> filter(const std::vector<Item>& items,
                         const std::function<bool(const Item&)>& keep) {
    std::vector<Item> out;
    std::copy_if(items.begin(), items.end(), std::back_inserter(out), keep);
    return out;
}

int main() {
    std::vector<Item> items = {{"Potion", 25}, {"Sword", 120}, {"Arrow", 5}};

    // A predicate stored in a variable — a first-class callable value.
    std::function<bool(const Item&)> is_valuable = [](const Item& i) {
        return i.value >= 20;
    };

    std::cout << "valuable items (value >= 20): "
              << std::count_if(items.begin(), items.end(), is_valuable) << "\n";
    for (const Item& it : filter(items, is_valuable)) {
        std::cout << "  " << it.name << " (" << it.value << ")\n";
    }

    // A closure: the lambda captures `threshold` by copy so the rule is data-driven.
    int threshold = 100;
    std::function<bool(const Item&)> expensive = [threshold](const Item& i) {
        return i.value >= threshold;
    };

    std::cout << "items worth >= 100: "
              << std::count_if(items.begin(), items.end(), expensive) << "\n";
    for (const Item& it : filter(items, expensive)) {
        std::cout << "  " << it.name << " (" << it.value << ")\n";
    }

    return 0;
}
