#pragma once

// ============================================================================
// gmath::Vec2<T> — a tiny 2D vector (self-contained copy).
//
// This is the SAME shape introduced in stage 15 and reused everywhere after.
// It is DELIBERATELY free of any SFML include so the game LOGIC that uses it
// stays testable without a window (the whole point of "keep logic pure").
// The bridge to sf::Vector2f lives in `gmath/sfml_bridge.hpp`, included ONLY
// by rendering code — never by the tested core.
//
// Python note: think of a small immutable-ish value type like a namedtuple
// with math on it. The big difference is that T is a compile-time TYPE: a
// Vec2<float> and a Vec2<int> are different types the compiler tracks for you,
// so you cannot accidentally mix pixel-integers and physics-floats.
// ============================================================================

#include <cmath>

namespace gmath {

template <typename T>
struct Vec2 {
    T x{};  // brace-init to the type's zero, so a default Vec2f is (0, 0).
    T y{};

    // Small, obvious operators. We pass Vec2 by value here because it is two
    // scalars — copying it is as cheap as copying the members individually.
    Vec2 operator+(const Vec2& r) const { return {x + r.x, y + r.y}; }
    Vec2 operator-(const Vec2& r) const { return {x - r.x, y - r.y}; }
    Vec2 operator*(T s) const { return {x * s, y * s}; }
    Vec2 operator/(T s) const { return {x / s, y / s}; }

    Vec2& operator+=(const Vec2& r) {
        x += r.x;
        y += r.y;
        return *this;
    }
    Vec2& operator-=(const Vec2& r) {
        x -= r.x;
        y -= r.y;
        return *this;
    }
    Vec2& operator*=(T s) {
        x *= s;
        y *= s;
        return *this;
    }

    // Exact equality only makes sense for integer vectors; for floats prefer a
    // tolerance (Catch2's Approx in tests). Provided for completeness/tests.
    bool operator==(const Vec2& r) const { return x == r.x && y == r.y; }
    bool operator!=(const Vec2& r) const { return !(*this == r); }
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;

// --- free functions (kept out of the struct so they read like math) ---------

inline float dot(const Vec2f& a, const Vec2f& b) { return a.x * b.x + a.y * b.y; }

// Length via Pythagoras. sqrt is not free, so `length_squared` exists for the
// common "compare distances" case where you never need the actual sqrt.
inline float length_squared(const Vec2f& v) { return dot(v, v); }
inline float length(const Vec2f& v) { return std::sqrt(length_squared(v)); }

// Return v scaled to length 1. A zero vector has no direction, so we return it
// unchanged rather than dividing by zero (which would give NaNs).
inline Vec2f normalize(const Vec2f& v) {
    const float len = length(v);
    if (len == 0.0f) {
        return v;
    }
    return v / len;
}

// Linear interpolation between a and b. t==0 -> a, t==1 -> b.
inline Vec2f lerp(const Vec2f& a, const Vec2f& b, float t) { return a + (b - a) * t; }

}  // namespace gmath
