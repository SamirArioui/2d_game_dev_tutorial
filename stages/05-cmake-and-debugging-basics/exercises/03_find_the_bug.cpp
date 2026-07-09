// Exercise 03 — Find the bug with lldb
//
// This should print the average of {10, 20, 30}, which is 20. It prints something else. Rather
// than staring at the code, practice driving the debugger — the skill that matters here.
//
// Build WITH debug symbols (-g) and NO optimization (-O0, so the code maps 1:1 to your source):
//   clang++ -std=c++17 -Wall -Wextra -g -O0 03_find_the_bug.cpp -o ex
//   lldb ./ex
//
// A first lldb session (lldb commands mirror Python's pdb — shown in parentheses):
//   (lldb) breakpoint set --name average   # set a breakpoint on the function  (pdb: b average)
//   (lldb) run                             # start the program                 (pdb: run)
//   (lldb) next                            # execute one line, step OVER calls  (pdb: n)
//   (lldb) print sum                       # inspect a variable                (pdb: p sum)
//   (lldb) print count                     # ... and another
//   (lldb) continue                        # resume until exit                 (pdb: c)
//   (lldb) quit                            # leave lldb
//
// Short forms exist: `b` = breakpoint set --name, `r` = run, `n` = next, `s` = step (INTO calls),
// `p` = print, `c` = continue, `bt` = backtrace (the call stack).
//
// Step through the loop and watch `sum` and `count`. One of them never changes the way it should.
//
// TODO: use lldb to locate the bug, then fix it so the program prints 20.

#include <iostream>

double average(const int* values, int n) {
    int sum = 0;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        sum += values[i];
        // Something that should happen once per element is missing here...
    }
    return static_cast<double>(sum) / count;
}

int main() {
    const int scores[3] = {10, 20, 30};
    std::cout << "average = " << average(scores, 3) << "\n";
    return 0;
}
