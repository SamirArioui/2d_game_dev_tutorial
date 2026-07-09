// SUPPORT FILE for exercise 05 — do NOT edit.
//
// A TYPE shared across translation units. The struct DEFINITION belongs in a header so
// every .cpp that uses it sees the SAME layout.
//
// Types are the deliberate exception to "definitions go in .cpp files": a struct/class
// definition may — and must — appear in every TU that uses it. #pragma once keeps it to
// one copy per TU, and the ODR allows this precisely because every copy is identical.
#pragma once

#include <string>

struct Enemy {
    std::string name;
    int hp;
    int attack;
};

// Declarations of free functions that operate on the shared type. Their bodies go in a .cpp.
int  total_threat(const Enemy& enemy);   // a simple hp + attack score
void print_enemy(const Enemy& enemy);
