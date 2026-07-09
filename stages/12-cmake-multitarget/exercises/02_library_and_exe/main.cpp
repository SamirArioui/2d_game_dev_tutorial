#include <iostream>

#include "greeter.hpp"  // found because greeter.hpp sits next to main.cpp

int main() {
    std::cout << greeting("game dev") << "\n";
    return 0;
}
