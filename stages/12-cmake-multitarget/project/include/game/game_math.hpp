#pragma once

// ---------------------------------------------------------------------------
// Pure game-math helpers.
//
// "Pure" means the result depends only on the arguments, and calling the
// function changes nothing else (no globals, no printing, no files). That is
// what makes these trivial to unit-test later (stage 13): the same inputs
// always give the same answer, so a test just compares against a known value.
//
// Python note: think of a small `game_math.py` module of free functions — but
// here every parameter and the return value have a fixed TYPE (`float`) that
// the compiler checks. No `def clamp(value, lo, hi):` accepting anything.
// ---------------------------------------------------------------------------
namespace game {

// Constrain `value` to the inclusive range [lo, hi].
float clamp(float value, float lo, float hi);

// Linear interpolation: a at t==0, b at t==1, straight-line blend in between.
// Used constantly in games: smooth movement, fades, easing.
float lerp(float a, float b, float t);

// Straight-line (Euclidean) distance between (x1,y1) and (x2,y2).
float distance(float x1, float y1, float x2, float y2);

}  // namespace game
