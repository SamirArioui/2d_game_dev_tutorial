// Exercise 05 — Exit codes & stderr (medium-hard)
//
// Goal: understand that main returns a status to the operating system, and that programs have
// TWO output streams. This is how games/tools report "something went wrong".
//
// Background:
//   - std::cout  = standard output (normal output).
//   - std::cerr  = standard error (for error/diagnostic messages). Both usually show in your
//                  terminal, but they're separate streams so errors can be logged/redirected
//                  independently.
//   - `return N;` from main sets the program's EXIT CODE. 0 = success, non-zero = failure.
//     The shell stores the last exit code in $?  ->  check it with:  echo $?
//
// TODO: pretend an asset failed to load. Make the program:
//   1. print to std::cout:  "Loading assets..."
//   2. print to std::cerr:  "ERROR: could not find player.png"
//   3. return the exit code 1 (failure).
//
// Compile, run, and inspect the exit code:
//   clang++ -std=c++17 -Wall -Wextra 05_exit_code.cpp -o ex
//   ./ex
//   echo $?        # should print 1
//
// Bonus: redirect just the error stream to a file and confirm only the error goes there:
//   ./ex 2> errors.log
//   cat errors.log

#include <iostream>

int main() {
    // TODO: implement the three steps above.
    return 0;  // change this
}
