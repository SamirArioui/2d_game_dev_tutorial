// mathutils.hpp — DECLARATIONS for the game-math library.
//
// A header is the PUBLIC FACE of a library: it lists what exists (names, parameter types,
// return types) so any .cpp can #include it and call these functions. The bodies live in
// mathutils.cpp and get compiled exactly once, into one object file.
//
// Python note: `#include "mathutils.hpp"` is NOT `import`. It literally pastes this file's
// text into whoever includes it (see the README). There is no module object created, no
// new namespace, and nothing is executed — it is a copy/paste the preprocessor performs.
#pragma once   // include guard: paste this header at most once per translation unit

// Clamp `value` into the inclusive range [lo, hi].
float clamp(float value, float lo, float hi);

// Linear interpolation: a at t = 0, b at t = 1, a straight line in between.
float lerp(float a, float b, float t);

// Euclidean distance between the points (x1, y1) and (x2, y2).
float distance(float x1, float y1, float x2, float y2);
