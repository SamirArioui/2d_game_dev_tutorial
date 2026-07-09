#pragma once

// ===========================================================================
// motion — pure 2D motion & tweening, built on gmath::Vec2f.
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
    body.pos = body.pos + body.vel * dt;   // uses velocity from the START of the step
    body.vel = body.vel + accel * dt;
}

// SEMI-IMPLICIT (symplectic) Euler updates velocity FIRST, then moves with the
// NEW velocity. One line reordered — but it's far more stable for games and
// conserves energy well, so it's the default for gravity/springs. Prefer this.
inline void integrate_semi_implicit(Body& body, const gmath::Vec2f& accel, float dt) {
    body.vel = body.vel + accel * dt;      // update velocity first ...
    body.pos = body.pos + body.vel * dt;   // ... then move with the NEW velocity
}

// aim_velocity — the "shoot toward a target" helper the mini-project is built on.
// The direction is normalize(target - origin); scaling it by `speed` gives a
// velocity of exactly `speed` units/second pointed straight at the target. This
// is why normalize matters: it separates DIRECTION from MAGNITUDE.
inline gmath::Vec2f aim_velocity(const gmath::Vec2f& origin, const gmath::Vec2f& target,
                                 float speed) {
    return gmath::normalize(target - origin) * speed;
}

// --- easing (tweening curves) ----------------------------------------------
//
// An easing function reshapes a time fraction t in [0,1] into another value in
// [0,1], but along a curve. Feeding the eased value into lerp(a, b, eased) makes
// motion start/stop gently instead of at constant speed. All of these satisfy
// f(0)=0 and f(1)=1; only the path between differs.

// ease_in_quad — starts slow, speeds up (t squared).
inline float ease_in_quad(float t) {
    return t * t;
}

// ease_out_quad — starts fast, slows to a stop.
inline float ease_out_quad(float t) {
    return 1.0f - (1.0f - t) * (1.0f - t);
}

// smoothstep — the classic S-curve: eases in AND out, flat slope at both ends.
// smoothstep(t) = 3t² - 2t³. The go-to curve for "smooth but simple".
inline float smoothstep(float t) {
    return t * t * (3.0f - 2.0f * t);
}

// ping_pong — turn an ever-increasing time value into a 0→1→0→1… triangle wave
// of period 2. Feed it (elapsed_seconds * rate) to drive a platform back and
// forth forever: the output sweeps up to 1, back down to 0, and repeats.
inline float ping_pong(float t) {
    float cycle = t - 2.0f * std::floor(t / 2.0f);  // wrap t into [0, 2)
    return cycle <= 1.0f ? cycle : 2.0f - cycle;    // fold [1,2) back down to [1,0]
}

}  // namespace motion
