#include <iostream>

#include "mathlib.hpp"  // resolved via the include path INHERITED from the library

int main() {
    std::cout << "2 + 3 = " << add(2, 3) << "\n";
    return 0;
}
