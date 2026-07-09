// Exercise 04 — Function overloading (solution)
#include <iostream>

// Same name, different parameter type -> two separate functions the compiler chooses between.
void describe(int level) {
    std::cout << "Level " << level << " (integer)\n";
}

void describe(double ratio) {
    std::cout << "Ratio " << ratio << " (real number)\n";
}

int max_of(int a, int b) {
    return a > b ? a : b;
}

double max_of(double a, double b) {
    return a > b ? a : b;
}

int main() {
    describe(5);      // integer literal -> int overload
    describe(0.75);   // double literal  -> double overload

    std::cout << "max_of(3, 9)     = " << max_of(3, 9) << "\n";       // int overload
    std::cout << "max_of(2.5, 1.5) = " << max_of(2.5, 1.5) << "\n";   // double overload

    return 0;
}
