// Exercise 05 — Combat summary (solution)
#include <iostream>
#include <string>

// Shared course contract: same shape reused by later stages.
struct Stats {
    int hp;
    int attack;
    int defense;
};

enum class DamageType { Physical, Fire, Ice };

// One multiplier per damage type, chosen with a switch. default keeps us safe if the enum
// grows later and we forget to update this table.
float damage_multiplier(DamageType type) {
    switch (type) {
        case DamageType::Physical:
            return 1.0f;
        case DamageType::Fire:
            return 1.5f;
        case DamageType::Ice:
            return 0.75f;
        default:
            return 1.0f;
    }
}

int main() {
    Stats attacker{0, 14, 0};   // only attack matters for the attacker here
    Stats defender{40, 0, 6};   // only defense matters for the defender here

    DamageType type = DamageType::Fire;

    int raw = attacker.attack - defender.defense;   // 14 - 6 = 8
    if (raw < 0) {
        raw = 0;   // never heal the target with a "negative" hit
    }

    float mult = damage_multiplier(type);
    int final_damage = static_cast<int>(raw * mult);  // 8 * 1.5 = 12.0 -> 12

    std::cout << "Fire attack: " << attacker.attack << " - " << defender.defense
              << " = " << raw << " raw, x" << mult
              << " -> " << final_damage << " damage\n";

    return 0;
}
