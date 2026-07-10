// mathutils.cpp — DEFINITIONS for the declarations in mathutils.hpp.  (REFERENCE SOLUTION)
//
// A .cpp includes its OWN header first, so the compiler checks these bodies against the
// promises in the header (a signature typo becomes a compile error here, not a confusing
// linker error later). This file is one TRANSLATION UNIT; it compiles to mathutils.o.
#include "mathutils.hpp"

#include <cmath>   // std::sqrt

float clamp(float value, float lo, float hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

float lerp(float a, float b, float t) {
    // At t = 0 this is a; at t = 1 it is b; values in between slide along the line.
    return a + (b - a) * t;
}

float distance(float x1, float y1, float x2, float y2) {
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}
