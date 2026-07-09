// Exercise 02 — References (solution)
#include <iostream>

int main() {
    int score = 100;

    // `alias` is not a copy and not a pointer — it is another name for `score`.
    int& alias = score;

    alias += 50;  // touches the very same int
    std::cout << "score = " << score << " (changed through alias)\n";

    // A reference MUST bind to something at birth. This would NOT compile:
    //   int& bad;    // error: declaration of reference variable 'bad' requires an initializer
    // That guarantee is exactly why references can never be null — there is always a target.

    // Contrast with a pointer, which CAN be re-seated to point at something else:
    int a = 1;
    int b = 2;
    int* p = &a;   // p -> a
    p = &b;        // p -> b now (a reference could never do this)
    std::cout << "*p = " << *p << " (pointer re-seated to b)\n";

    return 0;
}
