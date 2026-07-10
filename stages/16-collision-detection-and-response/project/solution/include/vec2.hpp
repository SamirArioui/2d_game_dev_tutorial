#pragma once

// ===========================================================================
// gmath — self-contained copy for stage 16.
//
// This is the stage-15 version of the library: the Vec2<T> class PLUS the free
// functions dot / length / length_squared / normalize / distance / lerp, which
// the collision code below leans on heavily (a contact normal is a normalized
// vector; penetration depth is a length; the bounce response is a dot product).
//
// Still no SFML here — collision math must stay pure and unit-testable.
// ===========================================================================

#include <cmath>

namespace gmath {

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

template <class T>
T lerp(T a, T b, T t) {
    return a + (b - a) * t;
}

template <class T>
T dot(const Vec2<T>& a, const Vec2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

template <class T>
T length_squared(const Vec2<T>& v) {
    return dot(v, v);
}

template <class T>
T length(const Vec2<T>& v) {
    return static_cast<T>(std::sqrt(length_squared(v)));
}

template <class T>
Vec2<T> normalize(const Vec2<T>& v) {
    const T len = length(v);
    if (len <= static_cast<T>(1e-8)) {
        return Vec2<T>{static_cast<T>(0), static_cast<T>(0)};
    }
    return Vec2<T>{v.x / len, v.y / len};
}

template <class T>
T distance(const Vec2<T>& a, const Vec2<T>& b) {
    return length(a - b);
}

template <class T>
Vec2<T> lerp(const Vec2<T>& a, const Vec2<T>& b, T t) {
    return Vec2<T>{lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}

}  // namespace gmath
