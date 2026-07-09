// Exercise 05 — A struct shared across translation units (medium)
//
// The Enemy TYPE is defined once, in 05_shared_struct.hpp. Both this file and the driver
// (05_shared_struct_main.cpp) include that header, so both agree on what an Enemy is. This
// file provides the DEFINITIONS of the functions the header declares.
//
// This is exactly the pattern the mini-project uses with `struct Player`: the type in a
// header, the functions split into a .cpp, all linked together.
//
// TODO: define total_threat and print_enemy.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_shared_struct.cpp 05_shared_struct_main.cpp -o ex
//   ./ex

#include "05_shared_struct.hpp"

#include <iostream>

// TODO: total_threat(enemy)  ->  enemy.hp + enemy.attack

// TODO: print_enemy(enemy)   ->  print e.g.:  Goblin (hp 12, threat 16)
//       Hint: reuse total_threat here.
