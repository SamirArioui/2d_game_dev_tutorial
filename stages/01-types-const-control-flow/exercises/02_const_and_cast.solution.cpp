// Exercise 02 — const & static_cast (solution)
#include <iostream>

int main() {
    const int max_level = 99;   // const = "this never changes after here"
    int total_xp = 250;
    int battles = 4;

    std::cout << "Max level: " << max_level << "\n";

    // Integer division: the result type is int, so the fractional part is dropped.
    std::cout << "XP per battle (integer division): " << total_xp / battles << "\n";  // 62

    // static_cast<double> turns ONE operand into a double; that forces the whole
    // division to be done in double, keeping the fraction. The cast is explicit so
    // the reader can see exactly where the type change happens.
    std::cout << "XP per battle (real average)    : "
              << static_cast<double>(total_xp) / battles << "\n";  // 62.5

    return 0;
}
