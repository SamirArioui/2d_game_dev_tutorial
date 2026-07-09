// Exercise 04 — std::string basics (medium)
//
// std::string is C++'s text type. It behaves a lot like Python's str, with a few twists:
//
//   std::string a = "Aria";
//   std::string b = a + " the Bold";   // '+' concatenates (like Python)
//   b.size();                          // length (Python: len(b))
//   b[0];                              // indexing -> a single CHAR 'A' (not a 1-char string!)
//   b.substr(5, 3);                    // "the"  -> substring: start at 5, take 3 chars
//   (a == "Aria");                     // value comparison with == works (unlike Java)
//   for (char c : b) { ... }           // range-for over each character
//
// Big difference from Python: a std::string is MUTABLE and b[0] is a `char`, a single
// character type written with single quotes ('A'), distinct from a "string" in double quotes.
//
// TODO:
//   1. Include <iostream> and <string>.
//   2. Make  std::string hero = "Aria";  and  std::string title = "the Bold";
//   3. Build  full = hero + " " + title;  and print it.
//   4. Print full.size().
//   5. Print the first character: full[0].
//   6. Print full.substr(0, 4)  (should be "Aria").
//   7. Count the vowels in `full` with a range-for over its chars and an if.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_strings.cpp -o ex && ./ex

// TODO: includes

int main() {
    // TODO: build the full name, then print length, first char, substring, and vowel count.
    return 0;
}
