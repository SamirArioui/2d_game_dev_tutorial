// Exercise 02 — Split a mini-library across files (solution)
//
// Compile with the driver:
//   clang++ -std=c++17 -Wall -Wextra 02_split_across_files.solution.cpp \
//           02_split_across_files_main.cpp -o ex && ./ex
#include "02_split_across_files.hpp"

// Definitions. Signatures match the header exactly, so the driver's calls resolve at link.
float rect_area(float width, float height) {
    return width * height;
}

float rect_perimeter(float width, float height) {
    return 2.0f * (width + height);
}
