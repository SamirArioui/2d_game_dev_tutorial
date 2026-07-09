#pragma once

// ============================================================================
// The bridge between gmath and SFML.
//
// gmath::Vec2 is deliberately SFML-free (so it stays testable). But the moment
// we hand a position to a sprite or a view we need sf::Vector2. Rather than
// contaminate math.hpp with an SFML include, the conversions live HERE, in a
// header only the SFML-facing code pulls in. Pure logic keeps compiling without
// SFML; rendering code includes this one extra file.
// ============================================================================

#include <SFML/System/Vector2.hpp>

#include "eng/math.hpp"

namespace gmath {

template <class T>
inline sf::Vector2<T> to_sfml(const Vec2<T>& v) {
    return sf::Vector2<T>{v.x, v.y};
}

template <class T>
inline Vec2<T> from_sfml(const sf::Vector2<T>& v) {
    return Vec2<T>{v.x, v.y};
}

}  // namespace gmath
