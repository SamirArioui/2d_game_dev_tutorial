// Exercise 01 — Address-of and dereference (solution)
#include <iostream>

int main() {
    int hp = 30;

    // `&hp` is the address of hp; `int*` reads "pointer to int". p now refers to hp's box.
    int* p = &hp;

    std::cout << "hp         = " << hp << "\n";
    std::cout << "&hp (addr) = " << p << "\n";    // a pointer prints as a hex address
    std::cout << "*p  (deref)= " << *p << "\n";   // follow the pointer back to the value

    // Writing THROUGH the pointer changes the original variable — it is the same box.
    *p = 25;
    std::cout << "after *p = 25, hp = " << hp << "\n";

    return 0;
}
