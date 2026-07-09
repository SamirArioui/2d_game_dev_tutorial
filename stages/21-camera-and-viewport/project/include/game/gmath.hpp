#pragma once

// ============================================================================
// gmath — the course's tiny 2D vector library (self-contained copy).
//
// This is the same `gmath::Vec2<T>` first built in stage 08 and given its free
// functions (length / normalize / lerp ...) in stage 15. Every SFML-era stage
// carries its own copy so the folder builds on its own.
//
// WHY a header-only template: a vector's operations are identical for every
// element type, so `Vec2<T>` is a class *template*. Templates must be visible
// wherever they're used, so the whole thing lives in a header (there is no
// gmath.cpp). Python has no direct analogue — a Python class is generic for
// free because everything is dynamically typed; in C++ we ask the compiler to
// stamp out a concrete `Vec2<float>` / `Vec2<int>` for us, checked at compile
// time.
// ============================================================================

#include <cmath>  // std::sqrt

// The SFML interop (`to_sfml` / `from_sfml`) is only useful when SFML's headers
// are actually on the include path. `__has_include` lets us switch it on ONLY
// then, so the pure-logic unit tests (which link neither SFML nor a window) can
// include this header with zero SFML dependency. Decoupling the math from the
// renderer is the whole reason the tilemap parser is testable.
#if __has_include(<SFML/System/Vector2.hpp>)
#include <SFML/System/Vector2.hpp>
#define GMATH_HAS_SFML 1
#endif

namespace gmath {

// A 2D vector / point. Aggregate with public data, because a vector *is* just
// its two numbers — there is nothing to hide, so no `x_`/`y_` privacy here.
template <typename T>
struct Vec2 {
    T x{};  // value-initialised to 0 for the numeric types we use
    T y{};

    // Compound assignment mutates in place and returns *this so calls chain.
    Vec2& operator+=(const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    Vec2& operator-=(const Vec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    Vec2& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
};

// The two aliases the whole course uses. `Vec2f` is by far the common one.
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;

// --- Free binary operators --------------------------------------------------
// Written in terms of the compound versions above so there's one source of
// truth. Taking the left operand by value gives us a copy to mutate + return.
template <typename T>
Vec2<T> operator+(Vec2<T> a, const Vec2<T>& b) {
    a += b;
    return a;
}
template <typename T>
Vec2<T> operator-(Vec2<T> a, const Vec2<T>& b) {
    a -= b;
    return a;
}
template <typename T>
Vec2<T> operator*(Vec2<T> v, T scalar) {
    v *= scalar;
    return v;
}
template <typename T>
Vec2<T> operator*(T scalar, Vec2<T> v) {
    v *= scalar;
    return v;
}
template <typename T>
bool operator==(const Vec2<T>& a, const Vec2<T>& b) {
    return a.x == b.x && a.y == b.y;
}
template <typename T>
bool operator!=(const Vec2<T>& a, const Vec2<T>& b) {
    return !(a == b);
}

// --- Free functions (the stage-15 additions) --------------------------------

// Dot product: a.x*b.x + a.y*b.y. Building block for length and projections.
template <typename T>
T dot(const Vec2<T>& a, const Vec2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

// Euclidean length (magnitude) of the vector.
template <typename T>
T length(const Vec2<T>& v) {
    return std::sqrt(dot(v, v));
}

// A unit-length vector pointing the same way. Returns the zero vector unchanged
// (dividing by zero length would be undefined) — a common safety guard.
template <typename T>
Vec2<T> normalize(const Vec2<T>& v) {
    const T len = length(v);
    if (len > T(0)) {
        return v * (T(1) / len);
    }
    return v;
}

// Linear interpolation between a and b: a at t==0, b at t==1.
template <typename T>
Vec2<T> lerp(const Vec2<T>& a, const Vec2<T>& b, T t) {
    return a + (b - a) * t;
}

// Straight-line distance between two points.
template <typename T>
T distance(const Vec2<T>& a, const Vec2<T>& b) {
    return length(b - a);
}

#ifdef GMATH_HAS_SFML
// Adapters to/from SFML's own vector type. Kept out of the pure library so the
// tests never need SFML on their include path (see the __has_include note up
// top). `inline` because these are function definitions living in a header.
inline sf::Vector2f to_sfml(const Vec2f& v) {
    return sf::Vector2f(v.x, v.y);
}
inline Vec2f from_sfml(const sf::Vector2f& v) {
    return Vec2f{v.x, v.y};
}
#endif

}  // namespace gmath
