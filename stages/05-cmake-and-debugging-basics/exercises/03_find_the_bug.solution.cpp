// Exercise 03 — Find the bug with lldb (solution)
//
// The bug: `count` was never incremented, so it stayed 0. Dividing by 0.0 (as a double) yields
// "inf", which is the wrong number the program printed. In lldb you'd see `count` stuck at 0
// while stepping the loop. The fix is one line: ++count each iteration.
#include <iostream>

double average(const int* values, int n) {
    int sum = 0;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        sum += values[i];
        ++count;   // <-- the missing line
    }
    return static_cast<double>(sum) / count;
}

int main() {
    const int scores[3] = {10, 20, 30};
    std::cout << "average = " << average(scores, 3) << "\n";
    return 0;
}
