// Exercise 03 — Pass by const reference (solution)
#include <array>
#include <iostream>

// const std::array<int, 5>& : we bind directly to the caller's array (no 5-int copy)
// and the 'const' guarantees this function can only read it, never modify it.
int sum(const std::array<int, 5>& values) {
    int total = 0;
    for (int v : values) {
        total += v;
    }
    // values[0] = 0;  // <-- would fail: "cannot assign to ... const-qualified type"
    return total;
}

int main() {
    std::array<int, 5> party_hp = {30, 24, 40, 18, 33};
    std::cout << "Party total HP: " << sum(party_hp) << "\n";
    return 0;
}
