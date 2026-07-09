// Exercise 02 — const & static_cast (easy-medium)
//
// Two ideas that trip up Python programmers:
//
// (A) const — a promise to the compiler that a value will NOT change after init.
//     If you try to reassign it, the program won't compile. Python has no real
//     equivalent (a leading-underscore name is only a convention).
//
// (B) Integer division truncates. In C++, `int / int` throws away the remainder:
//        7 / 2  ==  3   (not 3.5!)
//     To get a real fraction you must convert one side to a floating type first,
//     using static_cast<double>(...). This is EXPLICIT on purpose: C++ makes you
//     say "yes, I really mean to change this value's type here".
//
// Given: total_xp = 250 earned over battles = 4.
//
// TODO:
//   1. Declare `const int max_level = 99;` and print it.
//        (Optional: uncomment the line that tries to change it and read the error.)
//   2. Print the WRONG average using plain integer division: total_xp / battles.
//   3. Print the CORRECT average by converting to double first with static_cast<double>.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_const_and_cast.cpp -o ex && ./ex

#include <iostream>

int main() {
    const int max_level = 99;
    int total_xp = 250;
    int battles = 4;

    // TODO 1: print max_level.
    // max_level = 100;   // <-- uncomment to see: "cannot assign to variable 'max_level' with const-qualified type"

    // TODO 2: print the integer-division result (expect 62, the .5 is lost).

    // TODO 3: print the real average using static_cast<double>(total_xp) / battles (expect 62.5).

    return 0;
}
