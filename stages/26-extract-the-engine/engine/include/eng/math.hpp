#pragma once

// ============================================================================
// eng math — a tiny 2D vector type and scalar helpers.
//
// This lives in namespace `gmath` (the shared course contract), NOT `eng`,
// because the math is deliberately kept SEPARATE from the rest of the engine:
// it has ZERO dependency on SFML, so it compiles and unit-tests without a
// window, a GPU, or any of SFML's headers. Everything else in `eng` is allowed
// to lean on SFML; the math is the pure, portable core they all build on.
//
// Python note: think of a `vec2.py` with a small dataclass and free functions —
// but here `Vec2<T>` is a TEMPLATE, so `Vec2<float>`, `Vec2<int>`, ... are all
// generated from one definition, each fully type-checked by the compiler.
// ============================================================================

#include <cmath>

namespace gmath {

// A 2D vector / point. `T` is usually `float` (see Vec2f below). It is a plain
// value type: copyable, comparable, and cheap to pass around.
template <class T>
struct Vec2 {
    T x{};
    T y{};

    constexpr Vec2() = default;
    constexpr Vec2(T x_in, T y_in) : x(x_in), y(y_in) {}

    // Compound assignment mutates *this and returns it, so `a += b += c` chains.
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

// Binary operators as free functions (idiomatic C++: keeps them symmetric and
// lets the left operand be passed by value so we can reuse the compound forms).
template <class T>
constexpr Vec2<T> operator+(Vec2<T> a, const Vec2<T>& b) {
    a += b;
    return a;
}
template <class T>
constexpr Vec2<T> operator-(Vec2<T> a, const Vec2<T>& b) {
    a -= b;
    return a;
}
template <class T>
constexpr Vec2<T> operator*(Vec2<T> v, T scalar) {
    v *= scalar;
    return v;
}
template <class T>
constexpr Vec2<T> operator*(T scalar, Vec2<T> v) {
    v *= scalar;
    return v;
}
template <class T>
constexpr bool operator==(const Vec2<T>& a, const Vec2<T>& b) {
    return a.x == b.x && a.y == b.y;
}
template <class T>
constexpr bool operator!=(const Vec2<T>& a, const Vec2<T>& b) {
    return !(a == b);
}

// The overwhelmingly common instantiation — one alias so game code reads clean.
using Vec2f = Vec2<float>;

// --- Scalar / vector helpers (free functions, so they read like math) -------

inline float length(const Vec2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// Unit-length version of v. Guard against the zero vector: normalising (0,0)
// would divide by zero, so we return (0,0) instead of a NaN.
inline Vec2f normalized(const Vec2f& v) {
    const float len = length(v);
    if (len <= 0.0f) {
        return Vec2f{};
    }
    return Vec2f{v.x / len, v.y / len};
}

inline float dot(const Vec2f& a, const Vec2f& b) {
    return a.x * b.x + a.y * b.y;
}

inline float distance(const Vec2f& a, const Vec2f& b) {
    return length(b - a);
}

// Constrain a scalar to [lo, hi]. Used all over: paddle limits, camera clamp, ...
inline float clamp(float value, float lo, float hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

// Linear interpolation: a at t==0, b at t==1.
inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

}  // namespace gmath
