// Exercise 02 — The member-initializer list (easy-medium)
//
// A constructor can set members in TWO places:
//   (a) the member-initializer list, after the ':' and before the '{'   <-- preferred
//   (b) by assignment inside the { } body
//
//   Character(std::string n, int h) : name_(n), hp_(h) { }    // (a) construct directly
//   Character(std::string n, int h) { name_ = n; hp_ = h; }   // (b) default-construct, then assign
//
// Form (a) constructs each member ONCE, with its final value. Form (b) first default-constructs
// the member and THEN overwrites it — wasteful for a std::string, and IMPOSSIBLE for `const`
// members or references (they must be initialised, never assigned).
//
// PYTHON NOTE: Python only has the body form (assignments in __init__). There is no init list,
// because Python objects don't construct typed members up front.
//
// NOTE: this starter does NOT compile yet — the const member `max_hp_` must be set in an init
// list, and the body below doesn't. Fixing that is the exercise.
//
// TODO:
//   1. Replace the body assignments with a member-initializer list (form (a)).
//   2. Include the const member max_hp_ in that list — it can ONLY be set there.
//   3. In main, build a Character and print its fields via the getters.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_member_init_list.cpp -o ex && ./ex

#include <iostream>
#include <string>

class Character {
public:
    // TODO: initialise name_, hp_, and the const max_hp_ using a member-init list instead.
    Character(std::string name, int hp, int max_hp) {
        // (remove these and use an init list; max_hp_ being const makes assignment impossible)
    }

    const std::string& name() const { return name_; }
    int hp() const { return hp_; }
    int max_hp() const { return max_hp_; }

private:
    std::string name_;
    int hp_;
    const int max_hp_;   // const member: MUST be set in the init list, never by assignment
};

int main() {
    // TODO: Character hero{"Aria", 30, 30}; then print name / hp / max_hp.
    return 0;
}
