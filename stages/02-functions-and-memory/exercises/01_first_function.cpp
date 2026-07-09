// Exercise 01 — Your first function & value semantics (easy)
//
// A C++ function must declare the type it RETURNS and the type of every parameter:
//
//   int add(int a, int b) {   // returns int; takes two ints
//       return a + b;
//   }
//
// Python:  def add(a, b): return a + b     (no types written)
// C++:     int add(int a, int b) { return a + b; }
//
// By default parameters are passed BY VALUE — the function gets its own COPY. Changing
// a parameter inside the function does NOT affect the caller's variable. (Next exercise
// shows how to opt out of that with references.)
//
// TODO:
//   1. Above main, define  int add(int a, int b)  that returns a + b.
//   2. Define  void level_up(int level)  that does `level += 1;` then prints the new value.
//      Because level is passed by value, the caller's variable will NOT change — that's
//      the whole point of this drill.
//   3. In main: print add(7, 5). Then make `int hero_level = 3;`, call level_up(hero_level),
//      and print hero_level afterwards to prove it is STILL 3.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_first_function.cpp -o ex && ./ex

#include <iostream>

// TODO: define add() and level_up() here.

int main() {
    // TODO: call add(7, 5) and print it.
    // TODO: make hero_level = 3, call level_up(hero_level), print hero_level (still 3).
    return 0;
}
