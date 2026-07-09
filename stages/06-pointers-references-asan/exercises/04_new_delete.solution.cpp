// Exercise 04 — new / delete and the heap (solution)
#include <iostream>

int main() {
    // --- a single heap-allocated int ---
    int* hp = new int(50);            // one int on the heap, initialised to 50
    std::cout << "heap hp = " << *hp << "\n";
    delete hp;                        // give it back; exactly one delete per new
    hp = nullptr;                     // good habit: null it out so we can't reuse it by accident

    // --- a heap-allocated array ---
    int n = 5;
    int* squares = new int[n];        // n ints on the heap; note new[] pairs with delete[]
    for (int i = 0; i < n; ++i) {
        squares[i] = i * i;           // you index a pointer just like an array
    }
    std::cout << "squares:";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << squares[i];
    }
    std::cout << "\n";
    delete[] squares;                 // delete[] for ANYTHING made with new[]
    squares = nullptr;

    return 0;
}
