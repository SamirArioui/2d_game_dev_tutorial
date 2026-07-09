// Exercise 03 — enum class + switch (medium)
//
// An enum class is a small set of NAMED options that form their own type. Use it instead
// of "magic numbers" or bare strings for a fixed vocabulary (damage types, game states,
// directions, ...).
//
//   enum class DamageType { Physical, Fire, Ice };   // three named values of a new type
//   DamageType t = DamageType::Fire;                  // you MUST qualify with DamageType::
//
// Why "enum class" and not a plain "enum"? Two safety wins:
//   - SCOPED: the names live inside DamageType, so `Fire` from one enum can't clash with
//     `Fire` from another. You always write DamageType::Fire.
//   - TYPED: it will NOT silently convert to int. `int n = DamageType::Fire;` is an error;
//     you must ask explicitly with static_cast<int>(DamageType::Fire) (that gives 0, 1, 2...).
//
// Python's closest match is enum.Enum.
//
// TODO:
//   1. Define  enum class DamageType { Physical, Fire, Ice };
//   2. Write  const char* type_name(DamageType t)  that returns "Physical"/"Fire"/"Ice"
//      using a switch. Include a default case that returns "Unknown".
//   3. In main, print the name of each of the three values.
//   4. Print static_cast<int>(DamageType::Ice) to show the underlying integer (2).
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_enum_class.cpp -o ex && ./ex

#include <iostream>

// TODO: define enum class DamageType { ... };

// TODO: define const char* type_name(DamageType t) with a switch.

int main() {
    // TODO: print type_name for Physical, Fire, Ice.
    // TODO: print static_cast<int>(DamageType::Ice).
    return 0;
}
