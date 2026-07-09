// Exercise 04 — std::optional and the three error-handling styles (medium)
//
// "It might not be there" is everywhere in games: a save file that doesn't exist,
// a config key that's missing, a parse that fails. C++17's std::optional<T> models
// exactly that — a value that may or may not be present. It is the direct analog
// of a Python function that returns either a value or None.
//
//   std::optional<int> o;             // empty  (Python: None)
//   o = 42;                           // now holds a value
//   o.has_value()                     // Python: o is not None
//   *o  /  o.value()                  // read the value (UB / throws if empty)
//   o.value_or(-1)                    // the value, or -1 if empty (Python: o if o is not None else -1)
//
// THREE ways to report "this can fail", and when to use each:
//   1. return code / status flag  — old C style; easy to ignore, loses the value.
//   2. exceptions (throw/try/catch) — for TRULY exceptional, rare failures you
//      can't handle locally; they unwind the stack.
//   3. std::optional               — for EXPECTED "maybe absent" results, where
//      absence is normal, not exceptional. This is the Pythonic "return None".
//
// Fill in the TODOs. Expected output:
//   parse '42'  -> 42
//   parse 'oops' -> (none), using fallback 0
//   parse '  7 ' -> 7
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 04_optional.cpp -o ex && ./ex

#include <iostream>
#include <optional>
#include <sstream>
#include <string>

// Return the parsed int, or an EMPTY optional if `s` isn't a whole number.
// (Compare: a Python `def parse_int(s) -> int | None`.)
std::optional<int> parse_int(const std::string& s) {
    std::stringstream ss(s);
    int value = 0;
    ss >> value;  // sets the fail bit if there's no number at the front

    // TODO 1: if the extraction failed (!ss), return an empty optional.
    //         Return {} or std::nullopt to mean "no value".

    // A trailing non-space character (e.g. "12x") means it wasn't a clean int.
    char leftover = 0;
    if (ss >> leftover) {
        return std::nullopt;
    }

    // TODO 2: otherwise return the parsed value (just `return value;` — it wraps
    //         itself into the optional automatically).
    return std::nullopt;  // <- replace this
}

int main() {
    // TODO 3: parse "42". If it has_value(), print  parse '42'  -> 42
    //         Use *result or result.value() to read it.

    // parse "oops" — this one should be empty; show value_or as the fallback.
    std::optional<int> bad = parse_int("oops");
    std::cout << "parse 'oops' -> " << (bad.has_value() ? "has value" : "(none)")
              << ", using fallback " << bad.value_or(0) << "\n";

    // TODO 4: parse "  7 " and print  parse '  7 ' -> 7   (leading/trailing spaces are ok:
    //         >> skips leading whitespace, and a trailing space is not "leftover").

    return 0;
}
