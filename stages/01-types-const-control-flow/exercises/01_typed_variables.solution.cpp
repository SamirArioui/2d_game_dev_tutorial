// Exercise 01 — Typed variables (solution)
#include <iostream>

int main() {
    // Each variable's type is fixed at declaration and checked by the compiler.
    int level = 3;                // whole numbers
    float attack_power = 12.5f;   // 'f' = a single-precision float literal (not a double)
    double crit_multiplier = 2.0; // double = double-precision; the default for decimals
    bool is_boss = false;         // true / false only

    std::cout << "Level          : " << level << "\n";
    std::cout << "Attack power   : " << attack_power << "\n";
    std::cout << "Crit multiplier: " << crit_multiplier << "\n";
    // A bool streams as 1/0 unless you ask for std::boolalpha. 0 here means "not a boss".
    std::cout << "Is boss        : " << is_boss << "\n";

    return 0;
}
