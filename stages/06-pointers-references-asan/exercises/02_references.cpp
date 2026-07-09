// Exercise 02 — References (easy)
//
// A reference is an ALIAS: a second name for an existing variable. Unlike a pointer it
// cannot be null and cannot be re-seated to a different variable once created:
//
//   int  x = 10;
//   int& r = x;     // r is another name for x
//   r = 99;         // changes x, because r IS x
//
// PYTHON NOTE: this is the closest C++ gets to Python's "two names for one object". But a
// Python rebinding `r = other` makes the name r point elsewhere; a C++ reference can NEVER
// be rebound — `r = other` copies other's VALUE into x instead.
//
// TODO:
//   1. Make an int `score` = 100.
//   2. Make a reference `alias` bound to score.
//   3. Change score THROUGH the alias (alias += 50) and print score.
//   4. Then show a pointer being re-seated from one int to another (a reference can't do this).
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_references.cpp -o ex && ./ex

#include <iostream>

int main() {
    // TODO: implement the steps above.
    return 0;
}
