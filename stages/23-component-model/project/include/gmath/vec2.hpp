#pragma once

// ===========================================================================
// gmath — the course's small game-math library (the Vec2 half of it).
//
// This is the CANONICAL Vec2 introduced at stage 08 and grown at stage 15. It stays SFML-free on
// purpose: pure math with no rendering dependency is trivially unit-testable and links into both
// the game logic and the tests. The bridge to sf::Vector2f lives in the separate `vec2_sfml.hpp`
// so that code which never touches a window never has to include (or link) SFML.
//
// A namespace is C++'s answer to a Python MODULE: `gmath::Vec2` reads like `gmath.Vec2`.
// ===========================================================================

#include <cmath>  // std::sqrt for length()/normalize()

namespace gmath {

// A 2D vector as a CLASS TEMPLATE. `T` is filled in when you use it, so one definition serves
// Vec2<float>, Vec2<int>, ... — C++'s statically-typed answer to Python duck typing.
template <class T>
struct Vec2 {
    T x{};
    T y{};

    // Vector addition / subtraction — the C++ version of Python's __add__ / __sub__. const
    // (they read, don't mutate) and take the other vector by const& (no copy).
    Vec2 operator+(const Vec2& other) const { return Vec2{x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2& other) const { return Vec2{x - other.x, y - other.y}; }

    // Scale by a SCALAR of type T (Python's __mul__). This is `v * 2.0f`, not vector*vector —
    // there is no single "correct" vector*vector, so we don't define it.
    Vec2 operator*(T scalar) const { return Vec2{x * scalar, y * scalar}; }

    // Compound assignment — mutate in place (Python's __iadd__ / __isub__). Returns *this so
    // they chain. These are what the movement system uses: `transform.pos += velocity.vel * dt`.
    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Component-wise equality (Python's __eq__). Handy for the unit tests.
    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vec2& other) const { return !(*this == other); }
};

// Convenience ALIAS for the common float case. `Vec2f` and `Vec2<float>` name the same type.
using Vec2f = Vec2<float>;

// --- free-function geometry (added at stage 15) ----------------------------------------------
template <class T>
T dot(const Vec2<T>& a, const Vec2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

template <class T>
T length(const Vec2<T>& v) {
    return static_cast<T>(std::sqrt(dot(v, v)));
}

// Return v scaled to length 1. A zero vector has no direction, so we return it unchanged rather
// than dividing by zero.
template <class T>
Vec2<T> normalize(const Vec2<T>& v) {
    const T len = length(v);
    if (len == T{}) {
        return v;
    }
    return Vec2<T>{v.x / len, v.y / len};
}

}  // namespace gmath
