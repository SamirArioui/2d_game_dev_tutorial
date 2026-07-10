// ============================================================================
// Stage 02 mini-project — Game-math function set  (REFERENCE SOLUTION)
// ============================================================================
// This is the reference implementation. The starter you fill in is one level
// up at ../src/main.cpp. Build this the same way:
//   clang++ -std=c++17 -Wall -Wextra project/solution/src/main.cpp -o gamemath
// ============================================================================
//
// A tiny toolbox of the math every 2D game leans on, written as free functions so we
// can practise the different ways to pass arguments. In later stages these grow into a
// real math library (stage 04 splits them across files; stage 08 turns points into a
// Vec2<T> template).
//
// This ONE program exercises every concept from stage 02:
//   - pass by VALUE            (clamp / lerp / distance take cheap copies of floats)
//   - pass by REFERENCE        (reflect updates the caller's vx, vy in place)
//   - pass by CONST REFERENCE  (magnitude reads a point without copying it)
//   - function OVERLOADING     (clamp(int) and clamp(float) share one name)
//   - stack vs heap            (explained in the README; everything here lives on the
//                               stack — real heap allocation arrives in stage 06)
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
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

float clamp(float v, float lo, float hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

// --- lerp: LINEAR intERPolation. Returns the point t of the way from a to b. -------------
// t = 0 gives a, t = 1 gives b, t = 0.5 gives the midpoint. The backbone of smooth motion,
// fades, and easing (stage 15). Passed by value — three cheap floats.
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// --- distance: straight-line distance between two points (x1,y1) and (x2,y2). ------------
// Pythagoras. Floats by value again.
float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// --- magnitude: length of a 2D vector, passed BY CONST REFERENCE. ------------------------
// A 2-float array is small enough to copy, but we pass it as `const std::array<float, 2>&`
// to practise the pattern you'll rely on for bigger types (Vec2, containers) in later
// stages: bind to the caller's object (no copy) and promise not to change it.
float magnitude(const std::array<float, 2>& v) {
    return std::sqrt(v[0] * v[0] + v[1] * v[1]);
}

// --- reflect: bounce a velocity straight back, IN PLACE via REFERENCE out-parameters. ----
// No return value — the caller's own vx and vy are flipped. (A real bounce off an angled
// surface uses the surface normal; that lands in the collision stage, 16.)
void reflect(float& vx, float& vy) {
    vx = -vx;
    vy = -vy;
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
