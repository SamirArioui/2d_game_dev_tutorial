// ============================================================================
// Stage 02 mini-project — Game-math function set  (YOUR TASK)
// ============================================================================
//
// This is the STARTER. The function SIGNATURES and the `main()` that exercises
// them are given as scaffolding — your job is to fill in the six function bodies
// marked TODO, combining every concept from this stage: pass by VALUE (clamp /
// lerp / distance), pass by CONST REFERENCE (magnitude), pass by REFERENCE
// (reflect), and function OVERLOADING (clamp for int and for float).
//
// It already compiles and runs, but each body is a stub returning a placeholder,
// so the numbers are wrong until you implement them. The compiler's "unused
// parameter" warnings point at exactly which functions still need work — treat
// them as your checklist. A complete reference is in ../solution/src/main.cpp.
//
// GOAL — make the program print exactly this:
//
//   == Game-math toolbox ==
//
//   clamp(150, 0, 100)        = 100  (int overload)
//   clamp(-0.3f, 0.0f, 1.0f)  = 0  (float overload)
//
//   lerp(20, 100, t):
//     t=0 -> 20
//     t=0.25 -> 40
//     t=0.5 -> 60
//     t=0.75 -> 80
//     t=1 -> 100
//
//   distance((0,0),(3,4))     = 5
//   magnitude({3, 4})         = 5
//
//   Before reflect: velocity = (5, -2.5)
//   After  reflect: velocity = (-5, 2.5)
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o gamemath
//   ./gamemath
//   echo $?        # -> 0
// ============================================================================

#include <array>
#include <cmath>     // std::sqrt
#include <iostream>

// --- clamp: keep a value within [lo, hi]. Overloaded for int and float. -----------------
// Small numbers, so we pass BY VALUE (a copy of an int/float is free).

int clamp(int v, int lo, int hi) {
    // TODO(stage 02): return v clamped to [lo, hi] — lo if v < lo, hi if v > hi, else v.
    return 0;
}

float clamp(float v, float lo, float hi) {
    // TODO(stage 02): same logic as the int overload, but for floats. The compiler
    // picks this overload when the arguments are floats.
    return 0.0f;
}

// --- lerp: LINEAR intERPolation. Returns the point t of the way from a to b. -------------
// t = 0 gives a, t = 1 gives b, t = 0.5 gives the midpoint. The backbone of smooth motion,
// fades, and easing (stage 15). Passed by value — three cheap floats.
float lerp(float a, float b, float t) {
    // TODO(stage 02): return a + (b - a) * t.
    return 0.0f;
}

// --- distance: straight-line distance between two points (x1,y1) and (x2,y2). ------------
// Pythagoras. Floats by value again.
float distance(float x1, float y1, float x2, float y2) {
    // TODO(stage 02): compute dx = x2 - x1 and dy = y2 - y1, then return
    // std::sqrt(dx * dx + dy * dy).
    return 0.0f;
}

// --- magnitude: length of a 2D vector, passed BY CONST REFERENCE. ------------------------
// A 2-float array is small enough to copy, but we pass it as `const std::array<float, 2>&`
// to practise the pattern you'll rely on for bigger types (Vec2, containers) in later
// stages: bind to the caller's object (no copy) and promise not to change it.
float magnitude(const std::array<float, 2>& v) {
    // TODO(stage 02): return the vector's length: std::sqrt(v[0]*v[0] + v[1]*v[1]).
    return 0.0f;
}

// --- reflect: bounce a velocity straight back, IN PLACE via REFERENCE out-parameters. ----
// No return value — the caller's own vx and vy are flipped. (A real bounce off an angled
// surface uses the surface normal; that lands in the collision stage, 16.)
void reflect(float& vx, float& vy) {
    // TODO(stage 02): negate BOTH components in place: vx = -vx; vy = -vy;
}

int main() {
    std::cout << "== Game-math toolbox ==\n\n";

    // clamp — the overload is chosen by argument type, same name for both.
    int hp = clamp(150, 0, 100);              // int overload  -> 100
    float volume = clamp(-0.3f, 0.0f, 1.0f);  // float overload -> 0
    std::cout << "clamp(150, 0, 100)        = " << hp << "  (int overload)\n";
    std::cout << "clamp(-0.3f, 0.0f, 1.0f)  = " << volume << "  (float overload)\n\n";

    // lerp — walk a health bar from 20 to 100 in quarter steps.
    std::cout << "lerp(20, 100, t):\n";
    std::array<float, 5> steps = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    for (float t : steps) {
        std::cout << "  t=" << t << " -> " << lerp(20.0f, 100.0f, t) << "\n";
    }
    std::cout << "\n";

    // distance — how far a projectile travelled from muzzle to target.
    float d = distance(0.0f, 0.0f, 3.0f, 4.0f);   // classic 3-4-5 triangle -> 5
    std::cout << "distance((0,0),(3,4))     = " << d << "\n";

    // magnitude — length of a velocity vector, passed by const&.
    std::array<float, 2> velocity = {3.0f, 4.0f};
    std::cout << "magnitude({3, 4})         = " << magnitude(velocity) << "\n\n";

    // reflect — the ball hits a corner and bounces back; vx/vy change in place.
    float vx = 5.0f;
    float vy = -2.5f;
    std::cout << "Before reflect: velocity = (" << vx << ", " << vy << ")\n";
    reflect(vx, vy);
    std::cout << "After  reflect: velocity = (" << vx << ", " << vy << ")\n";

    return 0;
}
