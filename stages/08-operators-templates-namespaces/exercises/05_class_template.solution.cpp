// Exercise 05 — Class templates (solution)
#include <iostream>

template <class T>
class Range {
public:
    Range(T lo, T hi) : lo_(lo), hi_(hi) {}

    bool contains(T v) const { return lo_ <= v && v <= hi_; }

    T clamp(T v) const {
        if (v < lo_) {
            return lo_;
        }
        if (v > hi_) {
            return hi_;
        }
        return v;
    }

private:
    T lo_;
    T hi_;
};

int main() {
    Range<int> level{1, 10};
    std::cout << "level.contains(7)? " << (level.contains(7) ? "yes" : "no") << "\n";  // yes
    std::cout << "level.clamp(42)   = " << level.clamp(42) << "\n";                    // 10

    Range<double> unit{0.0, 1.0};
    std::cout << "unit.clamp(2.5)   = " << unit.clamp(2.5) << "\n";                    // 1
    return 0;
}
