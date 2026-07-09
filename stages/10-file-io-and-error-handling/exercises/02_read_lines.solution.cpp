// Exercise 02 — Reading a text file line by line (solution)
#include <fstream>
#include <iostream>
#include <string>

int main() {
    {
        std::ofstream out("party.txt");
        out << "Aria\n" << "Ranger\n" << "30\n";
    }

    std::ifstream in("party.txt");
    if (!in) {
        std::cerr << "error: could not open party.txt\n";
        return 1;
    }

    std::string line;
    int number = 1;
    while (std::getline(in, line)) {  // false at end-of-file
        std::cout << number << ": " << line << "\n";
        ++number;
    }

    return 0;
}
