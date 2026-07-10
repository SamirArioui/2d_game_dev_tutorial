#pragma once

// ===========================================================================
// sim — the WINDOW-FREE simulation logic for the stage-14 ball demo.  (REFERENCE SOLUTION)
//
// This is the fully-implemented reference. The STARTER you fill in is two levels
// up at ../../include/sim.hpp, where these function bodies are TODO stubs.
//
// Why is this a separate header with no SFML in it? Because rendering needs a
// screen, but LOGIC doesn't. Keeping the ball's motion and the timestep math in
// pure functions means we can unit-test them with Catch2 (see project/tests/)
// on a headless CI machine — no window required. This "pure logic, separate
// from rendering" rule is the throughline of the whole course (see ROADMAP.md).
//
// Everything here is a free function operating on plain data — the C++ analogue
// of a small, side-effect-light Python module of helpers.
// ===========================================================================

#include "vec2.hpp"

namespace sim {

// The ball is just data: where it is, how fast it moves, how big it is.
// Position is the CENTER of the ball, in pixels. Velocity is pixels PER SECOND
// (not per frame) — that's what makes motion frame-rate independent below.
struct Ball {
    gmath::Vec2f pos;
    gmath::Vec2f vel;
    float radius;
};

// Constant-velocity integration: pos += vel * dt.
//
// This is the simplest possible "physics": no gravity, no acceleration (those
// arrive in stage 15). Because `vel` is in pixels/second and `dt` is seconds,
// `vel * dt` is the pixels to move THIS frame — so a fast 240 Hz machine and a
// slow 30 Hz machine cover the same distance per real second. That is the whole
// point of delta time. Python has no built-in real-time loop to compare to;
// this is the game-loop idiom you must build yourself.
inline void integrate(Ball& ball, float dt) {
    ball.pos = ball.pos + ball.vel * dt;
}

// Reflect the ball off the four edges of a [0, width] x [0, height] box.
//
// Remember SFML's coordinate system: origin (0,0) is the TOP-LEFT and +y points
// DOWN. So "top edge" is y == 0 and "bottom edge" is y == height. We account for
// the radius (the ball touches an edge when its center is one radius away), snap
// the center back just inside, and flip the velocity component on the axis that
// crossed. Returns true if a bounce happened (handy for playing a sound later).
inline bool reflect_in_bounds(Ball& ball, float width, float height) {
    bool bounced = false;

    if (ball.pos.x - ball.radius < 0.0f) {
        ball.pos.x = ball.radius;   // snap the left edge to the wall
        ball.vel.x = -ball.vel.x;   // reverse horizontal direction
        bounced = true;
    } else if (ball.pos.x + ball.radius > width) {
        ball.pos.x = width - ball.radius;
        ball.vel.x = -ball.vel.x;
        bounced = true;
    }

    if (ball.pos.y - ball.radius < 0.0f) {
        ball.pos.y = ball.radius;
        ball.vel.y = -ball.vel.y;
        bounced = true;
    } else if (ball.pos.y + ball.radius > height) {
        ball.pos.y = height - ball.radius;
        ball.vel.y = -ball.vel.y;
        bounced = true;
    }

    return bounced;
}

// --- fixed-timestep accumulator -------------------------------------------
//
// A variable timestep (feed each frame's real dt straight into the physics) is
// simple but non-deterministic: the same inputs produce different results on
// different hardware, and very large dt values can make objects tunnel through
// walls. A FIXED timestep updates the simulation in constant-size slices (e.g.
// 1/120 s), which is stable and reproducible. The accumulator is the bridge
// between the two: real frames arrive at irregular intervals, and we bank that
// time until a whole fixed slice has built up.
//
// Add this frame's elapsed time to `accumulator`, then pull out as many whole
// `fixed_dt` slices as fit. Return how many fixed updates to run this frame; the
// leftover stays in `accumulator` for next time. `max_steps` caps the count so
// one long stall (a breakpoint, dragging the window) can't demand hundreds of
// catch-up steps and freeze the game — the dreaded "spiral of death".
inline int steps_to_run(float& accumulator, float frame_dt, float fixed_dt, int max_steps) {
    accumulator += frame_dt;

    int steps = 0;
    while (accumulator >= fixed_dt && steps < max_steps) {
        accumulator -= fixed_dt;
        ++steps;
    }
    return steps;
}

// Clamp a single frame's dt so one huge hitch (the OS suspended us, the user
// dragged the title bar) doesn't teleport everything across the screen. Used in
// the real loop BEFORE feeding time to the accumulator.
inline float clamp_frame_dt(float dt, float max_dt) {
    return dt > max_dt ? max_dt : dt;
}

// Linear interpolation, defined locally for render smoothing. Stage 15 promotes
// this to the shared gmath library; here it lets us draw the ball at a position
// blended between its previous and current fixed states, so a 60 Hz screen shows
// smooth motion even when physics ticks at 120 Hz. `t` in [0,1].
inline gmath::Vec2f render_lerp(const gmath::Vec2f& previous, const gmath::Vec2f& current,
                                float t) {
    return previous + (current - previous) * t;
}

}  // namespace sim
