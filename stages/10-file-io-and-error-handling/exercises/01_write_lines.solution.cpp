// Exercise 01 — Writing a text file (solution)
#include <fstream>
#include <iostream>

int main() {
    {
        std::ofstream out("hero.txt");
        if (!out) {
            std::cerr << "error: could not open hero.txt for writing\n";
            return 1;
        }
        out << "Aria" << "\n";
        out << 30 << "\n";
    }  // out closed here by its destructor (RAII)

    std::cout << "wrote hero.txt\n";
    return 0;
}
