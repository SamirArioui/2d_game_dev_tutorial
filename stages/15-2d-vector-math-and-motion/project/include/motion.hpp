#pragma once

// ===========================================================================
// motion — pure 2D motion & tweening, built on gmath::Vec2f.  (YOUR TASK)
//
// This is the STARTER. Every function below has a `// TODO(stage 15): ...` body
// with a placeholder return, so the project compiles and the window app links —
// but ../tests/test_motion.cpp starts RED. Implement each (the comments give the
// formula) to drive the suite GREEN. NOTE: this builds on the vector functions
// you implement in vec2.hpp, so finish those first (aim_velocity uses normalize).
// A reference is in ../solution/include/motion.hpp — try it yourself first.
//
// Everything here is window-free and unit-tested (project/tests/). It covers the
// two pillars of "things that move" in a 2D game:
//
//   1. INTEGRATION — turning acceleration + velocity into new positions each
//      step (Euler, explicit vs semi-implicit). This is how gravity works.
//   2. TWEENING — moving something smoothly from A to B over time using lerp
//      plus an EASING curve (ease-in/out) so it accelerates and decelerates
//      instead of snapping at constant speed. This is how a platform glides.
// ===========================================================================

#include <cmath>  // std::floor

#include "vec2.hpp"

namespace motion {

// A moving body is just position + velocity. Acceleration is passed in per step
// (it can change frame to frame — thrust, gravity, wind), so it isn't stored.
struct Body {
    gmath::Vec2f pos;
    gmath::Vec2f vel;
};

// --- integration -----------------------------------------------------------
//
// "Integration" here means numerically stepping the equations of motion: given
// where we are and how fast we're going, where are we one dt later?
//
// EXPLICIT (forward) Euler updates position with the OLD velocity, THEN updates
// velocity. It's the "obvious" order and what a naive implementation writes —
// but under constant acceleration it slightly lags and gains energy over time.
inline void integrate_explicit(Body& body, const gmath::Vec2f& accel, float dt) {
    // TODO(stage 15): explicit (forward) Euler — move with the OLD velocity
    // FIRST, then update velocity:  pos += vel * dt;  vel += accel * dt;
    (void)body;
    (void)accel;
    (void)dt;
}

// SEMI-IMPLICIT (symplectic) Euler updates velocity FIRST, then moves with the
// NEW velocity. One line reordered — but it's far more stable for games and
// conserves energy well, so it's the default for gravity/springs. Prefer this.
inline void integrate_semi_implicit(Body& body, const gmath::Vec2f& accel, float dt) {
    // TODO(stage 15): semi-implicit (symplectic) Euler — update velocity FIRST,
    // then move with the NEW velocity:  vel += accel * dt;  pos += vel * dt;
    // (Just the two lines above, swapped. This is the one to prefer for gravity.)
    (void)body;
    (void)accel;
    (void)dt;
}

// aim_velocity — the "shoot toward a target" helper the mini-project is built on.
// The direction is normalize(target - origin); scaling it by `speed` gives a
// velocity of exactly `speed` units/second pointed straight at the target. This
// is why normalize matters: it separates DIRECTION from MAGNITUDE.
inline gmath::Vec2f aim_velocity(const gmath::Vec2f& origin, const gmath::Vec2f& target,
                                 float speed) {
    // TODO(stage 15): return a velocity of magnitude `speed` aimed from origin
    // toward target — normalize(target - origin) * speed. (Uses your normalize.)
    (void)origin;
    (void)target;
    (void)speed;
    return gmath::Vec2f{0.0f, 0.0f};  // placeholder
}

// --- easing (tweening curves) ----------------------------------------------
//
// An easing function reshapes a time fraction t in [0,1] into another value in
// [0,1], but along a curve. Feeding the eased value into lerp(a, b, eased) makes
// motion start/stop gently instead of at constant speed. All of these satisfy
// f(0)=0 and f(1)=1; only the path between differs.

// ease_in_quad — starts slow, speeds up (t squared).
inline float ease_in_quad(float t) {
    // TODO(stage 15): starts slow, speeds up — return t squared (t * t).
    return t;  // placeholder: linear (not yet eased)
}

// ease_out_quad — starts fast, slows to a stop.
inline float ease_out_quad(float t) {
    // TODO(stage 15): starts fast, slows to a stop —
    // return 1 - (1 - t) * (1 - t).
    return t;  // placeholder: linear (not yet eased)
}

// smoothstep — the classic S-curve: eases in AND out, flat slope at both ends.
// smoothstep(t) = 3t² - 2t³. The go-to curve for "smooth but simple".
inline float smoothstep(float t) {
    // TODO(stage 15): the classic S-curve — return 3t² - 2t³, i.e.
    // t * t * (3 - 2 * t). Eases in AND out with flat slopes at 0 and 1.
    return t;  // placeholder: linear (not yet an S-curve)
}

// ping_pong — turn an ever-increasing time value into a 0→1→0→1… triangle wave
// of period 2. Feed it (elapsed_seconds * rate) to drive a platform back and
// forth forever: the output sweeps up to 1, back down to 0, and repeats.
inline float ping_pong(float t) {
    // TODO(stage 15): fold an ever-increasing t into a 0->1->0 triangle wave of
    // period 2. First wrap t into [0, 2): cycle = t - 2 * floor(t / 2). Then fold
    // the top half back down: return cycle <= 1 ? cycle : 2 - cycle.
    return t;  // placeholder: no wrapping/folding yet
}

}  // namespace motion
