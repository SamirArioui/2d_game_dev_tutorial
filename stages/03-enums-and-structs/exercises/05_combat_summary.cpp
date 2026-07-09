// Exercise 05 — Combat summary (medium-hard — combines the stage)
//
// Bring structs + enum class + switch + strings together, and reuse static_cast from
// stage 01. This is a smaller rehearsal for the mini-project.
//
// The pieces (these match the course's shared contracts — later stages reuse them):
//   struct Stats { int hp, attack, defense; };
//   enum class DamageType { Physical, Fire, Ice };
//   float damage_multiplier(DamageType);   // switch: Physical 1.0, Fire 1.5, Ice 0.75
//
// The damage formula:
//   raw   = attacker.attack - defender.defense      (an int; never let it go below 0)
//   final = static_cast<int>(raw * damage_multiplier(type))
//
// TODO:
//   1. Define struct Stats, enum class DamageType, and float damage_multiplier(DamageType)
//      using a switch (with a default of 1.0f).
//   2. In main, make an attacker Stats{0, 14, 0} and a defender Stats{40, 0, 6} (brace init).
//   3. For a Fire attack: compute raw = attack - defense, clamp it to >= 0, multiply by the
//      Fire multiplier, static_cast to int -> final damage.
//   4. Print a line like:  "Fire attack: 14 - 6 = 8 raw, x1.5 -> 12 damage".
//      (Use the damage_multiplier value in the message so the numbers add up.)
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_combat_summary.cpp -o ex && ./ex

#include <iostream>
#include <string>

// TODO: struct Stats { int hp, attack, defense; };
// TODO: enum class DamageType { Physical, Fire, Ice };
// TODO: float damage_multiplier(DamageType type) { switch ... }

int main() {
    // TODO: build attacker/defender, compute Fire damage, print the summary line.
    return 0;
}
