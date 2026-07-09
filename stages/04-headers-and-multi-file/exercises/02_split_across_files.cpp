// Exercise 02 — Split a mini-library across files (medium)
//
// The DECLARATIONS live in 02_split_across_files.hpp; a driver with main() lives in the
// support file 02_split_across_files_main.cpp. THIS file is where the DEFINITIONS go.
//
// Notice we #include the header here too. A .cpp should include its OWN header so the
// compiler can check your definitions against the declarations — a mismatched signature
// then becomes a friendly compile error instead of a mysterious linker error later.
//
// TODO: define rect_area and rect_perimeter to match the header.
//
// Compile ALL the translation units together, then run:
//   clang++ -std=c++17 -Wall -Wextra 02_split_across_files.cpp 02_split_across_files_main.cpp -o ex
//   ./ex
//
// Experiment (optional): open the .hpp, delete its `#pragma once`, and add a second
// `#include "02_split_across_files.hpp"` in the driver. You'll get a redefinition error —
// that's exactly what the include guard prevents. Put it back afterward.

#include "02_split_across_files.hpp"

// TODO: define rect_area(width, height)  ->  width * height

// TODO: define rect_perimeter(width, height)  ->  2 * (width + height)
