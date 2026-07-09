// SUPPORT FILE (driver) for exercise 02 — do NOT edit.
//
// This translation unit only needs the DECLARATIONS to call the functions, so it includes
// the header. The DEFINITIONS come from the other .cpp at link time.
#include "02_split_across_files.hpp"

#include <iostream>

int main() {
    const float w = 4.0f;
    const float h = 3.0f;
    std::cout << "area      = " << rect_area(w, h) << "\n";
    std::cout << "perimeter = " << rect_perimeter(w, h) << "\n";
    return 0;
}
