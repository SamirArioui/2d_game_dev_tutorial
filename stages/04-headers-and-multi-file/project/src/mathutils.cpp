// mathutils.cpp — DEFINITIONS for the declarations in mathutils.hpp.
//
// A .cpp includes its OWN header first, so the compiler checks these bodies against the
// promises in the header (a signature typo becomes a compile error here, not a confusing
// linker error later). This file is one TRANSLATION UNIT; it compiles to mathutils.o.
#include "mathutils.hpp"

#include <cmath>   // std::sqrt

float clamp(float value, float lo, float hi) {
    // TODO(stage 04): return `value` forced into the inclusive range [lo, hi]
    // (return lo if it's below, hi if it's above, otherwise value unchanged).
    (void)value;
    (void)lo;
    (void)hi;
    return 0.0f;  // placeholder so the starter links; replace with the real result
}

float lerp(float a, float b, float t) {
    // TODO(stage 04): linear interpolation — a at t = 0, b at t = 1, straight line between.
    (void)a;
    (void)b;
    (void)t;
    return 0.0f;  // placeholder
}

float distance(float x1, float y1, float x2, float y2) {
    // TODO(stage 04): Euclidean distance between (x1, y1) and (x2, y2) — use std::sqrt.
    (void)x1;
    (void)y1;
    (void)x2;
    (void)y2;
    return 0.0f;  // placeholder
}
