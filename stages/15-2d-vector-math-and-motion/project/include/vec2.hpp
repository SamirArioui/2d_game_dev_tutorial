#pragma once

// ===========================================================================
// gmath — the course's small game-math library (self-contained copy).
//
// Stage 08 built the Vec2<T> CLASS and the scalar clamp. THIS stage extends the
// library with the free functions every physics/graphics routine needs:
//   dot · length · length_squared · normalize · distance · lerp
//
// They are FREE functions (not methods) on purpose — they read like the maths
// they implement (`dot(a, b)`, `length(v)`) and they don't need access to Vec2's
// internals, so they don't belong inside the class. This is the C++ equivalent
// of module-level helper functions in Python.
//
// Note what is NOT here: anything SFML. This header stays pure and headless so
// the unit tests compile without a window (see project/tests/). The tiny bridge
// to sf::Vector2f lives in its own header, vec2_sfml.hpp.
// ===========================================================================

#include <cmath>  // std::sqrt

namespace gmath {

// --- the Vec2 class (unchanged from stage 08) ------------------------------
template <class T>
struct Vec2 {
    T x;
    T y;

    Vec2 operator+(const Vec2& other) const { return Vec2{x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2& other) const { return Vec2{x - other.x, y - other.y}; }
    Vec2 operator*(T scalar) const { return Vec2{x * scalar, y * scalar}; }
    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
};

using Vec2f = Vec2<float>;

// --- scalar utilities ------------------------------------------------------

// clamp — confine v to [lo, hi]. General scalar helper (from stage 08).
template <class T>
T clamp(T v, T lo, T hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

// lerp (scalar) — linear interpolation: at t=0 returns a, at t=1 returns b, and
// blends straight-line in between. `t` is normally in [0, 1]. This is the single
// most useful function in game feel: smooth movement, fades, easing all build on
// it. Python has no built-in, but it's the classic `a + (b - a) * t`.
template <class T>
T lerp(T a, T b, T t) {
    return a + (b - a) * t;
}

// --- vector free functions (NEW this stage) --------------------------------

// dot — the dot product a·b = ax*bx + ay*by. It measures how much two vectors
// point the same way: positive if roughly aligned, zero if perpendicular,
// negative if opposed. The workhorse behind projection, reflection (stage 16),
// and "is the enemy in front of me?".
template <class T>
T dot(const Vec2<T>& a, const Vec2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

// length_squared — the squared magnitude. Prefer this to length() when you only
// need to COMPARE distances (e.g. "which enemy is nearest"): it skips the sqrt,
// and a < b iff a² < b² for non-negative lengths.
template <class T>
T length_squared(const Vec2<T>& v) {
    return dot(v, v);
}

// length — the magnitude |v| = sqrt(x² + y²), i.e. Pythagoras.
template <class T>
T length(const Vec2<T>& v) {
    return static_cast<T>(std::sqrt(length_squared(v)));
}

// normalize — return a unit vector (length 1) pointing the same way as v. This
// is how you turn "toward the target" into a pure DIRECTION you can then scale by
// a speed. GUARD the zero vector: it has no direction, so dividing by its length
// would be 0/0 = NaN. We return {0,0} instead — a safe, explicit choice.
template <class T>
Vec2<T> normalize(const Vec2<T>& v) {
    const T len = length(v);
    if (len <= static_cast<T>(1e-8)) {
        return Vec2<T>{static_cast<T>(0), static_cast<T>(0)};
    }
    return Vec2<T>{v.x / len, v.y / len};
}

// distance — the straight-line distance between two points, |a - b|.
template <class T>
T distance(const Vec2<T>& a, const Vec2<T>& b) {
    return length(a - b);
}

// lerp (vector) — component-wise linear interpolation between two points. At t=0
// you get a, at t=1 you get b. Used for moving platforms, camera easing, tweens.
template <class T>
Vec2<T> lerp(const Vec2<T>& a, const Vec2<T>& b, T t) {
    return Vec2<T>{lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}

}  // namespace gmath
