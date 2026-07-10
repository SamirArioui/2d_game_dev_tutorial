// player.hpp — a TYPE shared across translation units.
//
// The struct DEFINITION lives in the header (not in a .cpp) because every file that uses a
// Player must see its exact layout to lay out variables and read fields. #pragma once keeps
// it to one copy per translation unit; because every copy is byte-for-byte identical, the
// One Definition Rule is satisfied even though several .cpp files include it.
#pragma once

#include <string>

struct Player {
    std::string name;
    int hp;
};

// A free function DECLARATION that operates on the shared type. Its body is in player.cpp,
// so main.cpp and player.cpp share both the TYPE (via this header) and the FUNCTION
// (via the linker).
void print_player(const Player& player);
