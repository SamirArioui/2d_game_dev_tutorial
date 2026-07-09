// ============================================================================
// Stage 00 mini-project — Character-sheet printer
// ============================================================================
//
// Combines everything from stage 00:
//   - #include to bring in the iostream library
//   - int main() as the program's entry point
//   - a multi-line std::cout block mixing text and NUMERIC LITERALS
//   - a clean `return 0;` to report success
//
// The values are hardcoded literals for now. In stage 01 they become real typed
// variables (int / std::string / ...); in stage 02 the derived stats get computed
// by functions. This character (Aria the Bold) recurs throughout the course.
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o character_sheet
//   ./character_sheet
//   echo $?        # -> 0
// ============================================================================

#include <iostream>

int main() {
    // Header banner. "\n" ends each interior line; std::endl ends the block and flushes.
    std::cout << "============================\n";
    std::cout << "   CHARACTER SHEET\n";
    std::cout << "============================\n";

    // Identity block — text values.
    std::cout << " Name : " << "Aria the Bold" << "\n";
    std::cout << " Class: " << "Ranger" << "\n";
    std::cout << " Level: " << 3 << "\n";

    std::cout << "----------------------------\n";

    // Combat stats — integer literals sent straight to the stream (no quotes).
    std::cout << " HP     : " << 30 << "\n";
    std::cout << " Attack : " << 12 << "\n";
    std::cout << " Defense: " << 8 << "\n";

    std::cout << "============================" << std::endl;

    return 0;  // success
}
