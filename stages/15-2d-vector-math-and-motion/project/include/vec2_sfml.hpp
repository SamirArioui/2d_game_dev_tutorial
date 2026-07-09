#pragma once

// ===========================================================================
// The Vec2 <-> sf::Vector2f BRIDGE.
//
// WHY does this live in its own tiny header, separate from vec2.hpp?
//
//   1. vec2.hpp must stay PURE — no SFML, no windowing. That's what lets the
//      physics/math unit tests compile and run headless (no display, no GPU).
//      The moment vec2.hpp includes <SFML/...>, every test needs SFML linked.
//
//   2. We want ONE vector type flowing through gameplay/physics code, not two.
//      SFML has its own sf::Vector2f, but it lacks dot/length/normalize/lerp and
//      it drags in the whole rendering stack. So we do our maths in gmath::Vec2f
//      and convert ONLY at the boundary where we hand positions to SFML to draw.
//
// The conversions are trivial and inline — the "adapter" is one line each. This
// is the classic pattern of keeping your domain types independent of a library
// and translating at the edges.
// ===========================================================================

#include <SFML/System/Vector2.hpp>

#include "vec2.hpp"

namespace gmath {

// gmath::Vec2f -> sf::Vector2f, used right before window.draw(...).
inline sf::Vector2f to_sfml(const Vec2f& v) {
    return sf::Vector2f(v.x, v.y);
}

// sf::Vector2f -> gmath::Vec2f, used when reading e.g. mouse position from SFML.
inline Vec2f from_sfml(const sf::Vector2f& v) {
    return Vec2f{v.x, v.y};
}

}  // namespace gmath
