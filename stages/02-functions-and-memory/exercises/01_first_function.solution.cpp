// Exercise 01 — Your first function & value semantics (solution)
#include <iostream>

// Returns the sum. a and b are copies of the caller's arguments.
int add(int a, int b) {
    return a + b;
}

// 'level' is passed BY VALUE: this is a local copy. The += only touches the copy.
void level_up(int level) {
    level += 1;
    std::cout << "Inside level_up, level is now " << level << "\n";
}

int main() {
    std::cout << "add(7, 5) = " << add(7, 5) << "\n";

    int hero_level = 3;
    level_up(hero_level);   // passes a COPY of hero_level
    // The caller's variable is untouched because level_up worked on its own copy.
    std::cout << "Back in main, hero_level is still " << hero_level << "\n";

    return 0;
}
