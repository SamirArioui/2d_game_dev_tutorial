// Exercise 06 — RAII: destructors that clean up (medium-hard)
//
// RAII = "Resource Acquisition Is Initialization". The pattern: ACQUIRE a resource in the
// CONSTRUCTOR, RELEASE it in the DESTRUCTOR. Because C++ destructors run deterministically when
// an object leaves scope, cleanup is automatic and exception-safe — you can't forget it.
//
// PYTHON NOTE: this is what `with open(...) as f:` does via __enter__/__exit__. But Python's
// __del__ runs "sometime later" at the GC's discretion; a C++ destructor runs at the exact
// moment the object goes out of scope, and objects are destroyed in REVERSE order of creation.
//
// Below, LiveCount tracks how many entities are currently alive using a shared counter.
// TODO:
//   1. In the constructor, ++count_ and print "spawned, alive = " << count_.
//   2. In the destructor, --count_ and print "died,    alive = " << count_.
//   3. In main, create LiveCounts in nested { } scopes and watch the count rise and fall
//      automatically as they go out of scope.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 06_raii.cpp -o ex && ./ex

#include <iostream>

class LiveCount {
public:
    // The shared counter is passed by reference and stored as a reference member — which is
    // why it must be set in the init list (references can never be assigned later).
    explicit LiveCount(int& count) : count_(count) {
        // TODO: ++count_; then print the new total.
    }

    ~LiveCount() {
        // TODO: --count_; then print the new total.
    }

private:
    int& count_;
};

int main() {
    int alive = 0;
    // TODO: create LiveCounts in nested { } scopes and observe `alive` rise and fall.
    return 0;
}
