// Exercise 04 — std::string basics (solution)
#include <iostream>
#include <string>

int main() {
    std::string hero = "Aria";
    std::string title = "the Bold";

    std::string full = hero + " " + title;   // '+' concatenates strings
    std::cout << "Full name : " << full << "\n";
    std::cout << "Length    : " << full.size() << "\n";      // number of characters
    std::cout << "First char: " << full[0] << "\n";          // a single char: 'A'
    std::cout << "Substring : " << full.substr(0, 4) << "\n"; // 4 chars from index 0 -> "Aria"

    // Range-for over the characters. A char literal uses single quotes: 'a', not "a".
    int vowels = 0;
    for (char c : full) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            ++vowels;
        }
    }
    std::cout << "Vowels    : " << vowels << "\n";

    return 0;
}
