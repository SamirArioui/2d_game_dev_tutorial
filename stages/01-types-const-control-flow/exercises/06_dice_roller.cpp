// Exercise 06 — Random dice roller (hard — combines everything)
//
// C++ randomness lives in <random> and is split into two pieces:
//   1. A GENERATOR — the engine of raw random bits. std::mt19937 is a good default
//      (a "Mersenne Twister"). You SEED it once; the same seed gives the same sequence.
//   2. A DISTRIBUTION — shapes those bits into the range/shape you want. For a fair die:
//      std::uniform_int_distribution<int> die(1, 6);  then call  die(generator).
//
// Python:  import random; random.randint(1, 6)
// C++:     std::mt19937 rng(seed); std::uniform_int_distribution<int> die(1, 6); die(rng);
// The split feels verbose, but it's deliberate: you control the engine and the shape
// separately, which matters for reproducible tests and for particle effects later.
//
// This drill ties together: <random>, std::array, a loop, auto, static_cast, and const.
//
// TODO:
//   1. Include <array>, <iostream>, <random>.
//   2. Seed a generator with a FIXED seed so runs are reproducible:
//        std::mt19937 rng(12345);
//      (Swap in std::random_device{}() later for real unpredictability.)
//   3. Make a d6 distribution: std::uniform_int_distribution<int> die(1, 6);
//   4. Declare `const int rolls = 20;` and `std::array<int, 6> counts{};` (all zeros).
//   5. Loop `rolls` times: bind each roll with `auto face = die(rng);`, then increment
//      counts[face - 1]  (faces 1..6 map to indices 0..5).
//   6. Print, for each face 1..6, its count and its percentage of the total. Compute the
//      percentage as: static_cast<double>(count) / rolls * 100.0
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 06_dice_roller.cpp -o ex && ./ex

// TODO: includes

int main() {
    // TODO: set up rng + distribution, roll into counts, print a histogram with percentages.
    return 0;
}
