// Exercise 04 — One Definition Rule: the duplicate symbol (medium-hard)
//
// Exercise 03 had ZERO definitions of a function (undefined reference). This is the mirror
// image: TWO definitions of the same function in one program. The One Definition Rule
// (ODR) says every non-inline function must be defined EXACTLY ONCE across the whole
// program — no more, no less.
//
// max_party_size() is already defined in the support file 04_duplicate_symbol_odr_lib.cpp.
// This file ALSO defines it below — so when you link both, the linker finds two bodies for
// one name and refuses:
//   duplicate symbol 'max_party_size()' in: ...  ld: 1 duplicate symbol ...
//
// See the error by compiling BOTH translation units together:
//   clang++ -std=c++17 -Wall -Wextra 04_duplicate_symbol_odr.cpp \
//           04_duplicate_symbol_odr_lib.cpp -o ex
//
// TODO: fix it. Delete the local definition below, and #include the header instead so this
//       file only has the DECLARATION — letting the single definition in the support .cpp
//       be the one everyone links against.

#include <iostream>
// TODO: replace the duplicate definition below with:  #include "04_duplicate_symbol_odr_lib.hpp"

// BUG: a SECOND definition of a function that is already defined in the support .cpp.
int max_party_size() {
    return 4;
}

int main() {
    std::cout << "Max party size: " << max_party_size() << "\n";
    return 0;
}
