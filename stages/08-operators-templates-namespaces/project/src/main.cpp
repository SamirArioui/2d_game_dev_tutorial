// ============================================================================
// Stage 08 mini-project — the gmath::Vec2<T> math lib in action    (YOUR TASK)
// ============================================================================
// This driver is COMPLETE — you do not edit it. Your task is to implement the math library it
// exercises: fill in the stubbed bodies in ../include/vec2.hpp (the four operators + clamp<T>).
// Until you do, this program compiles and runs but prints wrong numbers; once vec2.hpp is
// correct, it prints the expected output below.
//
// Exercises every idea from this stage:
//   - a class TEMPLATE:    gmath::Vec2<T>   (used here as Vec2f = Vec2<float>)
//   - OPERATOR overloading: +   -   *(scalar)   ==
//   - a function TEMPLATE:  gmath::clamp<T>, deduced for int, float and double
//   - a NAMESPACE:          everything qualified as gmath::
//
// Coming from Python: the operators are C++'s __add__/__sub__/__mul__/__eq__; the class template
// is what duck typing would look like if it were checked at compile time and specialised per
// type; and `gmath::` is the module qualifier, like `gmath.Vec2`.
//
// A complete reference is in ../solution/ (build it standalone) — try it yourself first.
//
// Build & run:
//   cmake -S . -B build && cmake --build build
//   ./build/vec2_demo
//
// GOAL — once vec2.hpp is implemented, the program prints exactly:
//   player   = (1, 2)
//   velocity = (3, -0.5)
//   next     = (7, 1)
//   back == player? yes
//
//   clamp(15, 0, 10)        = 10
//   clamp(-3.5f, 0.f, 1.f)  = 0
//   clamp(0.42, 0.0, 1.0)   = 0.42
// ============================================================================

#include <iostream>

#include "vec2.hpp"

int main() {
    using gmath::Vec2f;   // bring just this one name into scope (like `from gmath import Vec2f`)

    // Vec2f is Vec2<float>. Brace-init sets x then y.
    Vec2f player{1.0f, 2.0f};
    Vec2f velocity{3.0f, -0.5f};

    // operator*(scalar) then operator+ : one step of "position = position + velocity * dt".
    // '*' binds tighter than '+', so this reads as player + (velocity * dt).
    float dt = 2.0f;
    Vec2f next = player + velocity * dt;

    // Vec2 has no operator<< (kept minimal by design), so we print the fields directly.
    std::cout << "player   = (" << player.x << ", " << player.y << ")\n";
    std::cout << "velocity = (" << velocity.x << ", " << velocity.y << ")\n";
    std::cout << "next     = (" << next.x << ", " << next.y << ")\n";

    // operator- and operator==. The arithmetic here is exact in float (whole-number results),
    // so == is safe; in general, comparing floats for exact equality is risky.
    Vec2f back = next - velocity * dt;
    std::cout << "back == player? " << (back == player ? "yes" : "no") << "\n\n";

    // The SAME clamp template, deduced for three different types — one definition, three uses:
    std::cout << "clamp(15, 0, 10)        = " << gmath::clamp(15, 0, 10) << "\n";          // int
    std::cout << "clamp(-3.5f, 0.f, 1.f)  = " << gmath::clamp(-3.5f, 0.0f, 1.0f) << "\n";  // float
    std::cout << "clamp(0.42, 0.0, 1.0)   = " << gmath::clamp(0.42, 0.0, 1.0) << "\n";     // double

    return 0;
}
