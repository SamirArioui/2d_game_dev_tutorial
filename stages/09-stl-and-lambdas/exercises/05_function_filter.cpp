// Exercise 05 — Stored std::function filter (medium–hard)
//
// In Python functions are first-class values: you can store one in a variable and
// pass it around. C++ can do this too. A lambda has its own unnamed type, so to
// STORE one in a variable (or a struct field, or pass it as a parameter) you wrap
// it in std::function<Signature>:
//
//   std::function<bool(const Item&)> pred = [](const Item& i){ return i.value >= 20; };
//
// std::function<bool(const Item&)> means "any callable taking a const Item& and
// returning bool" — a predicate. We can then hand it to <algorithm> functions
// like std::count_if / std::copy_if, or store it for later.
//
// Lambdas can also CAPTURE surrounding variables (a closure), just like Python:
//   int min_value = 15;
//   auto pred = [min_value](const Item& i) { return i.value >= min_value; };
//   // [min_value] copies min_value into the lambda so it stays valid afterwards.
//
// Fill in the TODOs. Expected output:
//   valuable items (value >= 20): 2
//     Potion (25)
//     Sword (120)
//   items worth >= 100: 1
//     Sword (120)
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 05_function_filter.cpp -o ex && ./ex

#include <algorithm>   // std::count_if, std::copy_if
#include <functional>  // std::function
#include <iostream>
#include <iterator>    // std::back_inserter
#include <string>
#include <vector>

struct Item {
    std::string name;
    int value;
};

// Return the items for which `keep` returns true. `keep` is a stored predicate;
// the caller decides the rule (value >= N, name starts with..., anything).
std::vector<Item> filter(const std::vector<Item>& items,
                         const std::function<bool(const Item&)>& keep) {
    std::vector<Item> out;
    // copy_if writes each element that satisfies `keep` to the output iterator.
    // back_inserter turns push_back into an iterator so copy_if can append.
    std::copy_if(items.begin(), items.end(), std::back_inserter(out), keep);
    return out;
}

int main() {
    std::vector<Item> items = {{"Potion", 25}, {"Sword", 120}, {"Arrow", 5}};

    // TODO 1: declare a std::function<bool(const Item&)> named `is_valuable` that
    //         returns true when value >= 20.

    // TODO 2: print "valuable items (value >= 20): " and the COUNT, using
    //         std::count_if(items.begin(), items.end(), is_valuable).

    // TODO 3: print each valuable item indented as "  Name (value)" by calling
    //         filter(items, is_valuable) and looping over the result.

    // TODO 4: capture a threshold. Set  int threshold = 100;  then build a lambda
    //         that CAPTURES it — [threshold](const Item& i){ return i.value >= threshold; }
    //         Print "items worth >= 100: " + count, then the matching items,
    //         reusing filter().

    return 0;
}
