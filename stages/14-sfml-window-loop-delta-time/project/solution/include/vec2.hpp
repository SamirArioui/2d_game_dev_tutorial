#pragma once

// ===========================================================================
// gmath — the course's small game-math library (self-contained copy).
//
// This is the CANONICAL Vec2 built in stage 08. Each SFML stage carries its own
// copy of vec2.hpp so the stage builds on its own, without reaching across
// folders. Stage 15 will EXTEND this file with the free functions dot / length
// / normalize / distance / lerp; stage 14 only needs the vector itself.
//
// A namespace is C++'s answer to a Python MODULE: writing `gmath::Vec2` reads
// like `gmath.Vec2` after `import gmath`.
// ===========================================================================

namespace gmath {

// A 2D vector as a CLASS TEMPLATE. `T` is filled in when you use it, so one
// definition serves Vec2<float>, Vec2<int>, ... — C++'s statically-typed answer
// to Python duck typing: a separate, fully type-checked class per T.
template <class T>
struct Vec2 {
    T x;
    T y;

    // operator+ : vector addition (Python's __add__). const (reads, doesn't
    // mutate), takes the other vector by const& (no copy), returns a new Vec2.
    Vec2 operator+(const Vec2& other) const { return Vec2{x + other.x, y + other.y}; }

    // operator- : vector subtraction (Python's __sub__).
    Vec2 operator-(const Vec2& other) const { return Vec2{x - other.x, y - other.y}; }

    // operator* : scale by a SCALAR of type T (Python's __mul__). This is v * dt,
    // not vector-times-vector.
    Vec2 operator*(T scalar) const { return Vec2{x * scalar, y * scalar}; }

    // operator== : component-wise equality (Python's __eq__). Handy in tests.
    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
};

// A convenience ALIAS for the common float case. `Vec2f` and `Vec2<float>` name
// the same type. The graphics/physics stages use Vec2f throughout.
using Vec2f = Vec2<float>;

// clamp — a FUNCTION TEMPLATE confining `v` to [lo, hi]. Works for any type with
// operator<; the compiler deduces T, so `clamp(5, 0, 10)` needs no explicit <int>.
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

}  // namespace gmath
