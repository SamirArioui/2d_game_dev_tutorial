// Exercise 05 — reflect() + overloaded clamp() (medium-hard — combines the stage)
//
// This drill previews two pieces of the mini-project and combines references +
// overloading in one file.
//
// (A) An in-place function using REFERENCE out-parameters:
//        void reflect(float& vx, float& vy);
//     A ball flying with velocity (vx, vy) bounces straight back off a corner: flip the
//     sign of BOTH components, in place, through references. No value is returned — the
//     caller's own variables are updated.
//
// (B) OVERLOADED clamp — same name, two types:
//        int   clamp(int v, int lo, int hi);     // keep v within [lo, hi]
//        float clamp(float v, float lo, float hi);
//     "Clamp" = if v is below lo return lo; if above hi return hi; else return v. Games
//     use it constantly (keep HP in [0, max], keep the player on screen, ...).
//
// TODO:
//   1. Define  void reflect(float& vx, float& vy)  that negates both components.
//   2. Define both clamp overloads (int and float) with the same clamp logic.
//   3. In main:
//        - make float vx = 3.0f, vy = -2.0f; call reflect(vx, vy); print them (expect -3, 2).
//        - print clamp(150, 0, 100)   -> 100   (int overload)
//        - print clamp(-4.5f, 0.0f, 10.0f) -> 0 (float overload)
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_reflect_and_clamp.cpp -o ex && ./ex

#include <iostream>

// TODO: define reflect(float&, float&).
// TODO: define int clamp(int, int, int) and float clamp(float, float, float).

int main() {
    float vx = 3.0f;
    float vy = -2.0f;

    // TODO: reflect(vx, vy); then print vx and vy.
    // TODO: print clamp(150, 0, 100) and clamp(-4.5f, 0.0f, 10.0f).
    (void)vx;  // remove once you use them
    (void)vy;
    return 0;
}
