// ============================================================================
// Stage 03 mini-project — Combat summary with structs, enums & strings
// ============================================================================
//
// A hero equips an item and lands a typed attack on an enemy; the program prints a
// formatted combat report. It introduces the data types the rest of the course builds
// on, so the shapes below are FIXED CONTRACTS reused by later stages.
//
// This ONE program exercises every concept from stage 03:
//   - struct                    (Stats and Item bundle named fields)
//   - brace / aggregate init    (Stats{...}, Item{...})
//   - enum class + switch        (DamageType drives damage_multiplier via a switch)
//   - std::string                (names, and building the report text)
//   - (struct vs class is covered in the README; both structs here keep public data)
//
// Shared contracts (do not change their shape — stages 09, 10, 27 reuse them):
//   struct Stats { int hp, attack, defense; };
//   struct Item  { std::string name; int value; };
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o combat
//   ./combat
//   echo $?        # -> 0
// ============================================================================

#include <iostream>
#include <string>

// --- Shared data contracts -------------------------------------------------------------
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

// --- Damage typing ---------------------------------------------------------------------
enum class DamageType { Physical, Fire, Ice };

// Human-readable name for a damage type (used when building the report string).
std::string type_name(DamageType type) {
    switch (type) {
        case DamageType::Physical:
            return "Physical";
        case DamageType::Fire:
            return "Fire";
        case DamageType::Ice:
            return "Ice";
        default:
            return "Unknown";
    }
}

// The damage-type table: how much each type scales the raw hit. A switch over the enum is
// the idiomatic way to express "one value per case". The default protects us if the enum
// ever gains a case we forget to handle here.
float damage_multiplier(DamageType type) {
    switch (type) {
        case DamageType::Physical:
            return 1.0f;   // baseline
        case DamageType::Fire:
            return 1.5f;   // strong against this enemy
        case DamageType::Ice:
            return 0.75f;  // resisted
        default:
            return 1.0f;
    }
}

// Compute final damage from attacker/defender stats and a damage type.
// raw = attack - defense (floored at 0), then scaled by the type multiplier and
// truncated back to an int with static_cast (from stage 01).
int compute_damage(const Stats& attacker, const Stats& defender, DamageType type) {
    int raw = attacker.attack - defender.defense;
    if (raw < 0) {
        raw = 0;   // a hit never restores the target's HP
    }
    return static_cast<int>(raw * damage_multiplier(type));
}

int main() {
    // Brace / aggregate initialization: fields fill in declaration order.
    Stats hero{30, 14, 8};      // hp=30, attack=14, defense=8
    Stats enemy{40, 10, 6};     // hp=40, attack=10, defense=6

    Item weapon{"Ember Blade", 120};   // name (a std::string) + gold value

    std::string hero_name = "Aria the Bold";   // the recurring course hero
    DamageType attack_type = DamageType::Fire; // the Ember Blade deals Fire damage

    int damage = compute_damage(hero, enemy, attack_type);
    int enemy_hp_after = enemy.hp - damage;
    if (enemy_hp_after < 0) {
        enemy_hp_after = 0;
    }

    // Build the report with std::string concatenation + streaming.
    std::string divider = "------------------------------";

    std::cout << divider << "\n";
    std::cout << "        COMBAT SUMMARY\n";
    std::cout << divider << "\n";
    std::cout << "Attacker : " << hero_name << "\n";
    std::cout << "Weapon   : " << weapon.name << " (worth " << weapon.value << " gold)\n";
    std::cout << "Attack   : " << type_name(attack_type)
              << " (x" << damage_multiplier(attack_type) << ")\n";
    std::cout << divider << "\n";
    std::cout << "Hero  attack : " << hero.attack << "\n";
    std::cout << "Enemy defense: " << enemy.defense << "\n";
    std::cout << "Damage dealt : " << damage << "\n";
    std::cout << "Enemy HP     : " << enemy.hp << " -> " << enemy_hp_after << "\n";
    std::cout << divider << "\n";

    // A tiny verdict using the struct data.
    if (enemy_hp_after == 0) {
        std::cout << enemy_hp_after << " HP left — the enemy is defeated!\n";
    } else {
        std::cout << "The enemy staggers but fights on.\n";
    }

    return 0;
}
