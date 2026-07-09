// Exercise 04 — Function templates (solution)
#include <iostream>

template <class T>
T max_of(T a, T b) {
    return (a < b) ? b : a;   // only requires operator< on T
}

template <class T>
T clamp(T v, T lo, T hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

int main() {
    // The compiler stamps out max_of<int>, max_of<double>, clamp<int>, clamp<double> from these
    // calls, deducing T from the argument types. One source definition, many concrete functions.
    std::cout << "max_of(3, 9)        = " << max_of(3, 9) << "\n";           // 9
    std::cout << "max_of(2.5, 1.5)    = " << max_of(2.5, 1.5) << "\n";       // 2.5
    std::cout << "clamp(15, 0, 10)    = " << clamp(15, 0, 10) << "\n";       // 10
    std::cout << "clamp(-1.0,0.0,9.0) = " << clamp(-1.0, 0.0, 9.0) << "\n";  // 0
    return 0;
}
