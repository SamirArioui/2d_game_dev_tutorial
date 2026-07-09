// Exercise 06 — the planted bug: use-after-free (solution)
//
// The fix: use the memory BEFORE deleting it, and don't touch the pointer afterwards. Nulling
// the pointer after delete turns any accidental later use into an obvious nullptr crash
// instead of a silent, corrupting use-after-free.
#include <iostream>

struct Node {
    int value;
    Node* next;
};

int main() {
    Node* n = new Node{42, nullptr};

    // Use the memory while it is still ours...
    std::cout << "node value = " << n->value << "\n";

    // ...then release it, and drop the now-dangling pointer.
    delete n;
    n = nullptr;   // a later `n->value` would now be a clear nullptr deref, not a silent UAF

    return 0;
}
