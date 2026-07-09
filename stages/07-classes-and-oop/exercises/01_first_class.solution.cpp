// Exercise 01 — Your first class (solution)
#include <iostream>

class Health {
public:
    // Constructor: the parameter `hp` initialises the member `hp_` via the init list. `explicit`
    // stops surprising implicit conversions (e.g. a bare int silently becoming a Health).
    explicit Health(int hp) : hp_(hp) {}

    // `const` method: reading hp_ never changes the object.
    int hp() const { return hp_; }

    // Mutating method: no `const`, because it changes hp_.
    void heal(int amount) { hp_ += amount; }

private:
    int hp_;   // private: only Health's own methods can touch it
};

int main() {
    Health target{20};
    target.heal(5);
    std::cout << "hp = " << target.hp() << "\n";   // 25
    return 0;
}
