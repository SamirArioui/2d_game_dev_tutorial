// Exercise 03 — The undefined reference (solution)
//
// The fix is simply to PROVIDE the body the linker was looking for. Nothing about the
// declaration or the call site changes — the compile step was always fine; the program
// just lacked a definition to link against.
#include <iostream>

int armor_rating(int base, int level);   // declaration

int main() {
    std::cout << "Armor at level 5: " << armor_rating(10, 5) << "\n";
    return 0;
}

// The missing definition. Now the linker resolves the reference from main.
int armor_rating(int base, int level) {
    return base + level * 2;
}
