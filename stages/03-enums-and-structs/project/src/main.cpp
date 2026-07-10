// ============================================================================
// Stage 03 mini-project — Combat summary with structs, enums & strings  (YOUR TASK)
// ============================================================================
//
// This is the STARTER. The shared data contracts (Stats, Item, DamageType) and
// the function SIGNATURES are given — do not change their shape, later stages
// (09, 10, 27) reuse them. Your job: implement the three function bodies and the
// body of main(), combining every concept from this stage — two `struct`s (one
// with a std::string member), brace initialization, an `enum class` driving a
// `switch`, `std::string` building the report, and `static_cast` for the damage.
//
// It compiles and runs, but the functions are stubs and main() prints nothing
// yet, so there is no report until you implement them. The compiler's "unused"
// warnings mark what still needs work. A complete reference is in
// ../solution/src/main.cpp — try it yourself first.
//
// GOAL — make the program print exactly this:
//
//   ------------------------------
//           COMBAT SUMMARY
//   ------------------------------
//   Attacker : Aria the Bold
//   Weapon   : Ember Blade (worth 120 gold)
//   Attack   : Fire (x1.5)
//   ------------------------------
//   Hero  attack : 14
//   Enemy defense: 6
//   Damage dealt : 12
//   Enemy HP     : 40 -> 28
//   ------------------------------
//   The enemy staggers but fights on.
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o combat
//   ./combat
//   echo $?        # -> 0
// ============================================================================

#include <iostream>
#include <string>

// --- Shared data contracts (given — keep their shape) ----------------------------------
// A struct's members are PUBLIC by default, which is exactly what we want for plain data.
struct Stats {
    int hp;
    int attack;
    int defense;
};

struct Item {
    std::string name;   // a std::string member — the reason Item isn't just numbers
    int value;
};

// --- Damage typing (given contract) ----------------------------------------------------
enum class DamageType { Physical, Fire, Ice };

// Human-readable name for a damage type (used when building the report string).
std::string type_name(DamageType type) {
    // TODO(stage 03): switch over `type` and return "Physical" / "Fire" / "Ice"
    // (with a default of "Unknown").
    return "Unknown";
}

// The damage-type table: how much each type scales the raw hit. A switch over the enum is
// the idiomatic way to express "one value per case". The default protects us if the enum
// ever gains a case we forget to handle here.
float damage_multiplier(DamageType type) {
    // TODO(stage 03): switch over `type`: Physical -> 1.0f, Fire -> 1.5f, Ice -> 0.75f
    // (default -> 1.0f).
    return 1.0f;
}

// Compute final damage from attacker/defender stats and a damage type.
int compute_damage(const Stats& attacker, const Stats& defender, DamageType type) {
    // TODO(stage 03): raw = attacker.attack - defender.defense, floored at 0; then return
    // static_cast<int>(raw * damage_multiplier(type)).
    return 0;
}

int main() {
    // Brace / aggregate initialization (given): fields fill in declaration order.
    Stats hero{30, 14, 8};      // hp=30, attack=14, defense=8
    Stats enemy{40, 10, 6};     // hp=40, attack=10, defense=6

    Item weapon{"Ember Blade", 120};   // name (a std::string) + gold value

    std::string hero_name = "Aria the Bold";   // the recurring course hero
    DamageType attack_type = DamageType::Fire; // the Ember Blade deals Fire damage

    // TODO(stage 03): compute the hit, then the enemy's HP after it:
    //   int damage = compute_damage(hero, enemy, attack_type);
    //   int enemy_hp_after = enemy.hp - damage;  (floor at 0)

    // TODO(stage 03): build and print the report EXACTLY as in the GOAL block above.
    //   - use a std::string divider "------------------------------"
    //   - stream hero_name, weapon.name + weapon.value, type_name(attack_type) and
    //     damage_multiplier(attack_type), hero.attack, enemy.defense, the damage, and
    //     "enemy.hp -> enemy_hp_after"
    //   - finish with the verdict: if enemy_hp_after == 0 the enemy is defeated,
    //     otherwise it "staggers but fights on."

    return 0;
}
