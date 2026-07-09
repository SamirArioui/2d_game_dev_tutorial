// SUPPORT FILE (driver) for exercise 05 — do NOT edit.
//
// This TU includes the header, so it knows the Enemy layout and the function signatures.
// The function bodies are linked in from 05_shared_struct.cpp.
#include "05_shared_struct.hpp"

int main() {
    const Enemy goblin{"Goblin", 12, 4};
    const Enemy troll{"Troll", 40, 9};

    print_enemy(goblin);
    print_enemy(troll);
    return 0;
}
