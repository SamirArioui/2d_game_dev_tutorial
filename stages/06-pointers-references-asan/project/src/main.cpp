// ============================================================================
// Stage 06 mini-project — pointer swap + a heap-allocated singly linked list
// ============================================================================
// Combines every idea from this stage:
//   - pointers (Node*) and references (Node*& / int&)
//   - nullptr as both the end-of-list marker and the empty-list value
//   - new / delete to build and tear down nodes on the heap
//   - a pointer-based swap
//   - clean ownership: every `new` has a matching `delete`, so this program is SILENT under
//     AddressSanitizer. A correct program produces NO ASan report — that is the point.
//
// A singly linked list is a chain of heap nodes, each holding a value and a pointer to the
// next node; the last node points at nullptr. Python's `list` is a dynamic array, not a
// linked list, and it hides this pointer-chasing entirely — here you own every link.
//
// Build & run (plain):
//   cmake -S . -B build && cmake --build build
//   ./build/swap_and_list
//
// Build & run (under AddressSanitizer — ON by default for this stage):
//   the ENABLE_ASAN option defaults to ON, so this binary is ALREADY instrumented.
//   A clean run prints the list and exits with no ASan error.
// ============================================================================

#include <iostream>

// A single link in the chain. `next` points at the following node, or nullptr for the last.
struct Node {
    int value;
    Node* next;
};

// Swap two ints through pointers (the stage's pointer-swap requirement).
void swap_ints(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Add a new node holding `value` at the FRONT of the list. We take the head pointer BY
// REFERENCE (Node*&) so we can re-seat the caller's head to the new node.
void push_front(Node*& head, int value) {
    Node* node = new Node{value, head};   // the new node points at the old head
    head = node;                          // head now points at the new node
}

// Walk the chain from head to nullptr, printing each value. `const Node*` because traversal
// doesn't modify the nodes; we take head BY VALUE (a copy of the pointer) so advancing our
// local cursor doesn't disturb the caller's head.
void print_list(const Node* head) {
    std::cout << "[";
    for (const Node* cur = head; cur != nullptr; cur = cur->next) {
        std::cout << cur->value;
        if (cur->next != nullptr) {
            std::cout << " -> ";
        }
    }
    std::cout << "]\n";
}

// Free every node. This is the manual version of what a destructor does automatically in
// stage 07 and a smart pointer does in stage 11. Skip it and you leak the whole chain.
void free_list(Node*& head) {
    Node* cur = head;
    while (cur != nullptr) {
        Node* next = cur->next;   // grab next BEFORE deleting cur (else that read is a UAF)
        delete cur;
        cur = next;
    }
    head = nullptr;               // the list is now empty
}

int main() {
    // --- part 1: pointer-based swap ---
    int a = 7;
    int b = 99;
    std::cout << "before swap: a=" << a << " b=" << b << "\n";
    swap_ints(&a, &b);
    std::cout << "after  swap: a=" << a << " b=" << b << "\n\n";

    // --- part 2: build a linked list on the heap ---
    Node* head = nullptr;         // an empty list is just a null head
    push_front(head, 3);
    push_front(head, 2);
    push_front(head, 1);          // list is now 1 -> 2 -> 3

    std::cout << "list:       ";
    print_list(head);

    // --- part 3: hand back every node ---
    free_list(head);              // a matching delete for every new; ASan stays silent
    std::cout << "after free: ";
    print_list(head);             // empty list -> []

    return 0;
}
