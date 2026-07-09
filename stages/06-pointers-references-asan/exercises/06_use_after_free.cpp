// Exercise 06 — the planted bug: use-after-free (medium-hard)
//
// This program COMPILES and even seems to work — that is exactly what makes memory bugs so
// dangerous. It has a real defect: it reads heap memory AFTER freeing it (a "use-after-free",
// a.k.a. a dangling pointer). Your job is to catch it with AddressSanitizer, read the report,
// and fix it (compare with 06_use_after_free.solution.cpp).
//
// STEP 1 — build normally and run. It may print something plausible, or garbage, or crash —
// undefined behaviour is unpredictable, which is the whole problem:
//   clang++ -std=c++17 -Wall -Wextra 06_use_after_free.cpp -o ex && ./ex
//
// STEP 2 — build again WITH AddressSanitizer and run. ASan instruments every memory access
// and stops the program the instant it touches freed memory:
//   clang++ -std=c++17 -g -fsanitize=address,undefined -fno-omit-frame-pointer \
//           06_use_after_free.cpp -o ex_asan && ./ex_asan
//
// Read the report top-down:
//   "ERROR: AddressSanitizer: heap-use-after-free ..."   <- the KIND of bug
//   "READ of size 4 ..."          + a stack trace        <- WHERE you touched freed memory
//   "freed by thread ... here:"   + a stack trace        <- WHERE you called delete
//   "previously allocated ... here:"                      <- WHERE you called new
// The two traces point straight at the delete and the offending access.
//
// STEP 3 — fix the bug, rebuild under ASan, and confirm the report is gone.

#include <iostream>

struct Node {
    int value;
    Node* next;
};

int main() {
    Node* n = new Node{42, nullptr};

    delete n;   // <-- the memory is handed back to the system right here

    // BUG: `n` is now a DANGLING pointer. It still holds the old address, but that memory is
    // no longer ours. Reading through it is a use-after-free.
    std::cout << "node value = " << n->value << "\n";   // <-- ASan fires on this line

    return 0;
}
