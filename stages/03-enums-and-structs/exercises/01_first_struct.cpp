// Exercise 01 — Your first struct (easy)
//
// A struct bundles several named values into ONE new type. It's the C++ tool for "a thing
// with fields", closest to a Python dataclass (or a dict with fixed keys).
//
//   struct Monster {          // defines a NEW type called Monster
//       std::string name;     // fields (called "members")
//       int hp;
//   };                        // <-- the semicolon after a struct is REQUIRED
//
//   Monster m{"Goblin", 12};  // brace-init: fields fill in declaration order
//   std::cout << m.name;      // access a member with a dot, like Python attributes
//
// Python:  @dataclass
//          class Monster: name: str; hp: int
//
// TODO:
//   1. Include <iostream> and <string>.
//   2. Define a struct Monster with members: std::string name; int hp;
//      (Don't forget the semicolon after the closing brace.)
//   3. In main, create a Monster with brace-init, then print its name and hp using the dot.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_first_struct.cpp -o ex && ./ex

// TODO: includes

// TODO: define struct Monster { ... };

int main() {
    // TODO: make a Monster (e.g. {"Goblin", 12}) and print m.name and m.hp.
    return 0;
}
