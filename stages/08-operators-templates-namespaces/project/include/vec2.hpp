#pragma once

// ===========================================================================
// gmath — the course's small game-math library.                    (YOUR TASK)
//
// This header is the STARTER: the declarations are given (the namespace, the Vec2<T> struct with
// members `T x, y;`, every operator/method SIGNATURE, the Vec2f alias, and the clamp<T>
// signature) but the BODIES are stubbed with TODOs and placeholder returns. Implementing them IS
// this stage's mini-project. Do NOT change the signatures or add extra operations.
//
// A complete reference is in ../../solution/include/vec2.hpp — try it yourself first.
//
// Everything math-y lives in the `gmath` namespace so names like Vec2 or clamp never collide
// with the standard library or with your own code. A namespace is C++'s answer to a Python
// MODULE: writing `gmath::Vec2` reads like `gmath.Vec2` after `import gmath`.
//
// This header is the CANONICAL Vec2 the rest of the course depends on. Keep it minimal: later
// stages extend it (stage 15 adds dot/length/normalize/lerp), tests pin it down (stage 13), and
// the SFML stages bridge it to sf::Vector2f (stage 15). Do not add operations it doesn't need.
// ===========================================================================

namespace gmath {

// A 2D vector as a CLASS TEMPLATE. `T` is a type parameter filled in when you use it, so one
// definition serves Vec2<float>, Vec2<int>, Vec2<double>, ... This is C++'s statically-typed
// answer to Python "duck typing": instead of one class that accepts anything at run time, the
// compiler stamps out a separate, fully type-checked class for each T you actually use.
template <class T>
struct Vec2 {
    T x;
    T y;

    // operator+ : vector addition. Overloading '+' is the C++ version of Python's
    // __add__(self, other). It's a const method (it reads, doesn't mutate) and takes the other
    // vector by const& (no copy). It returns a brand-new Vec2 rather than modifying either input.
    Vec2 operator+(const Vec2& other) const {
        // TODO(stage 08): return a new Vec2 whose x/y are the component-wise sums.
        return Vec2{};
    }

    // operator- : vector subtraction (Python's __sub__).
    Vec2 operator-(const Vec2& other) const {
        // TODO(stage 08): return a new Vec2 whose x/y are the component-wise differences.
        return Vec2{};
    }

    // operator* : scale by a SCALAR of the same type T (Python's __mul__). This is v * 2.0f,
    // not vector-times-vector — there is no single "correct" vector*vector, so we don't define it.
    Vec2 operator*(T scalar) const {
        // TODO(stage 08): return a new Vec2 with each component multiplied by `scalar`.
        return Vec2{};
    }

    // operator== : component-wise equality (Python's __eq__). Handy for the unit tests in stage 13.
    bool operator==(const Vec2& other) const {
        // TODO(stage 08): return true only when BOTH x and y match.
        return false;
    }
};

// A convenience ALIAS for the common float case. `using` here works like a typedef: `Vec2f` and
// `Vec2<float>` name the exact same type. The graphics/physics stages use Vec2f throughout.
using Vec2f = Vec2<float>;

// clamp — a FUNCTION TEMPLATE (as opposed to the class template above). It confines `v` to the
// range [lo, hi]. It works for ANY type that supports '<': int, float, double, ... The compiler
// DEDUCES T from the arguments, so you write clamp(5, 0, 10) with no explicit <int> needed.
//
// It lives in gmath alongside Vec2 as a general-purpose scalar utility (it is NOT a Vec2 method).
template <class T>
T clamp(T v, T lo, T hi) {
    // TODO(stage 08): return lo if v < lo, hi if v > hi, otherwise v (using only '<' / '>').
    return v;
}

}  // namespace gmath
