// Exercise 03 — enum class + switch (solution)
#include <iostream>

// A new type whose only values are these three names. Underlying ints are 0, 1, 2.
enum class DamageType { Physical, Fire, Ice };

// Map each enum value to a readable name. Returning const char* (a string literal) is
// fine and cheap here. The default guards against a value we forgot to handle.
const char* type_name(DamageType t) {
    switch (t) {
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

int main() {
    std::cout << type_name(DamageType::Physical) << "\n";
    std::cout << type_name(DamageType::Fire) << "\n";
    std::cout << type_name(DamageType::Ice) << "\n";

    // An enum class does NOT convert to int on its own — you must ask explicitly.
    std::cout << "Ice's underlying value is " << static_cast<int>(DamageType::Ice) << "\n";  // 2

    return 0;
}
