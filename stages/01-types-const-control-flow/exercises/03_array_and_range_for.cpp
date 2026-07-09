// Exercise 03 — std::array & range-for & auto (medium)
//
// std::array<T, N> is a FIXED-SIZE list of N values of type T. Unlike a Python list
// it can't grow or shrink, and every element must be the same type T. That rigidity
// is what makes it fast and predictable (the size is known at compile time).
//
// A range-for loops over every element without you managing an index:
//     for (int v : rolls) { ... }          // v is a COPY of each element
//     for (auto v : rolls) { ... }         // 'auto' asks the compiler to deduce v's type
//
// Python:  for v in rolls:      C++:  for (auto v : rolls) {
//
// Given a fixed array of five damage rolls: {3, 6, 2, 5, 4}
//
// TODO:
//   1. Include <array> and <iostream>.
//   2. Make `std::array<int, 5> rolls = {3, 6, 2, 5, 4};`.
//   3. With a range-for, compute the total of all rolls.
//   4. With the same or another range-for, find the biggest roll.
//   5. Print total, biggest, and the average (average = total / 5.0 — note the .0
//      so it's a real average, not integer division!).
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_array_and_range_for.cpp -o ex && ./ex

// TODO: includes

int main() {
    // TODO: declare the array, loop with range-for, print total / biggest / average.
    return 0;
}
