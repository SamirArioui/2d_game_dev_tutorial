// Exercise 01 — Writing a text file (easy)
//
// std::ofstream is an OUTPUT file stream. You use it exactly like std::cout, but
// the characters go to a file instead of the terminal. Opening happens in the
// constructor; CLOSING happens automatically in the destructor (RAII — see the
// scope block below). That's the C++ version of Python's `with open(...) as f:`.
//
//   Python:                              C++:
//   with open("hero.txt", "w") as f:     std::ofstream out("hero.txt");
//       f.write("Aria\n")                out << "Aria\n";
//   # f closed at end of `with`          // out closed when it leaves scope
//
// Fill in the TODOs. Expected: creates hero.txt, then prints
//   wrote hero.txt
// and hero.txt contains:
//   Aria
//   30
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 01_write_lines.cpp -o ex && ./ex && cat hero.txt

#include <fstream>
#include <iostream>

int main() {
    // An extra scope block so we can SEE the file close deterministically at `}`.
    {
        std::ofstream out("hero.txt");  // opens (creates/truncates) the file

        // Always check the stream opened. `!out` is true if the open failed
        // (bad path, no permission, ...). This is the return-code style of error
        // handling: inspect a status, don't throw.
        if (!out) {
            std::cerr << "error: could not open hero.txt for writing\n";
            return 1;  // non-zero exit code = failure (stage 00)
        }

        // TODO 1: write the name "Aria" followed by a newline, using  out << ...
        // TODO 2: write the number 30 followed by a newline.

    }  // <-- `out`'s destructor runs here and flushes + closes the file (RAII)

    std::cout << "wrote hero.txt\n";
    return 0;
}
