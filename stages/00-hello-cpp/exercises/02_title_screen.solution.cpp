// Exercise 02 — Title screen (solution)
#include <iostream>

int main() {
    // One statement, several lines using "\n" for the internal breaks...
    std::cout << "****************************\n"
              << "*      GOBLIN QUEST        *\n"
              << "*      press ENTER         *\n";
    // ...and std::endl to finish the last line (ends the line AND flushes the stream).
    std::cout << "****************************" << std::endl;
    return 0;
}
