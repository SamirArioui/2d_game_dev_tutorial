// Exercise 04 — Sorting with a lambda (medium)
//
// A lambda is an anonymous function you write inline — Python's `lambda`, but it
// can span multiple lines and has a body in { }:
//
//   Python:  key = lambda item: item.value
//   C++:     auto key = [](const Item& item) { return item.value; };
//
// std::sort takes a COMPARATOR, not a key function. The comparator answers one
// question: "should a come before b?" — i.e. return a < b for ascending order.
// (Python's sorted(key=...) is a convenience; C++ hands you the lower-level
// comparator, which is strictly more general.)
//
// Fill in the TODOs. Expected output:
//   -- by value, ascending --
//   Arrow (5)
//   Potion (25)
//   Sword (120)
//   -- by value, descending --
//   Sword (120)
//   Potion (25)
//   Arrow (5)
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 04_sort_lambda.cpp -o ex && ./ex

#include <algorithm>  // std::sort
#include <iostream>
#include <string>
#include <vector>

// Shared course type.
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
    // TODO 1: sort `items` by value ascending. std::sort(begin, end, comparator).
    //         The comparator returns true when a should come before b:
    //         [](const Item& a, const Item& b) { return a.value < b.value; }
    print_all(items);

    std::cout << "-- by value, descending --\n";
    // TODO 2: sort again, but descending — flip the comparison to a.value > b.value.
    print_all(items);

    return 0;
}
