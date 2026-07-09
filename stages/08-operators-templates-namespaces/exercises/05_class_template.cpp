// Exercise 05 — Class templates (medium-hard)
//
// A class template parameterises a whole class on a type. Here `Range<T>` stores a lo/hi pair of
// some numeric type and answers questions about it. One definition works for Range<int>,
// Range<double>, ... — the compiler generates a distinct, type-checked class per T you use.
//
// PYTHON NOTE: Python classes are already "generic" (a list holds anything). C++ templates give
// you that reuse while keeping each instantiation strongly typed and as fast as hand-written code.
//
// TODO:
//   1. Implement contains(v): true when lo_ <= v <= hi_.
//   2. Implement clamp(v): confine v to [lo_, hi_].
//   3. In main, use a Range<int> and a Range<double>.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_class_template.cpp -o ex && ./ex

#include <iostream>

template <class T>
class Range {
public:
    Range(T lo, T hi) : lo_(lo), hi_(hi) {}

    // Both are const methods: they only read the range.
    bool contains(T v) const {
        // TODO
        return false;  // <-- replace
    }

    T clamp(T v) const {
        // TODO
        return v;  // <-- replace
    }

private:
    T lo_;
    T hi_;
};

int main() {
    Range<int> level{1, 10};
    std::cout << "level.contains(7)? " << (level.contains(7) ? "yes" : "no") << "\n";
    std::cout << "level.clamp(42)   = " << level.clamp(42) << "\n";

    Range<double> unit{0.0, 1.0};
    std::cout << "unit.clamp(2.5)   = " << unit.clamp(2.5) << "\n";
    return 0;
}
