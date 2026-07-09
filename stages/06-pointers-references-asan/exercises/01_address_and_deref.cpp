// Exercise 01 — Address-of and dereference (easy)
//
// Goal: meet the two operators that make pointers work:
//   &x   "address of x"      -> gives you a pointer to x's box in memory
//   *p   "the thing p points at" (dereference) -> read or write THROUGH the pointer
//
// PYTHON NOTE: Python hides all of this. `x = 5` binds the name `x` to an int object; you
// never see its address and you can't write "through" the name. C++ makes the distinction
// between a variable (a labelled box in memory) and its address explicit.
//
// TODO:
//   1. Make an int `hp` = 30.
//   2. Make a pointer `p` that holds the ADDRESS of hp   (int* p = &hp;).
//   3. Print hp, the address stored in p, and *p (the value p points at).
//   4. Assign THROUGH the pointer: *p = 25;  then print hp again — it changed!
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_address_and_deref.cpp -o ex && ./ex

#include <iostream>

int main() {
    // TODO: implement the four steps above.
    return 0;
}
