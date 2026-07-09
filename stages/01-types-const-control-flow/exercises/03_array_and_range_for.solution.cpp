// Exercise 03 — std::array & range-for & auto (solution)
#include <array>
#include <iostream>

int main() {
    // Fixed-size array of 5 ints. The <int, 5> is part of the type: it can never resize.
    std::array<int, 5> rolls = {3, 6, 2, 5, 4};

    int total = 0;
    int biggest = rolls[0];  // start with the first element as the running maximum

    // range-for: 'auto' here deduces int (the element type). 'v' is a copy each pass.
    for (auto v : rolls) {
        total += v;
        if (v > biggest) {
            biggest = v;
        }
    }

    std::cout << "Rolls total : " << total << "\n";
    std::cout << "Biggest roll: " << biggest << "\n";
    // total is an int; dividing by 5.0 (a double literal) makes the average a double.
    std::cout << "Average roll: " << total / 5.0 << "\n";

    return 0;
}
