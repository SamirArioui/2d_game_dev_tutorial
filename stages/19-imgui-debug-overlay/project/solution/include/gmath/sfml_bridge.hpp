#pragma once

// ============================================================================
// The ONE place gmath meets SFML.
//
// `vec2.hpp` is intentionally SFML-free so the tested game logic never drags a
// window library into the unit tests. But the rendering code obviously needs
// to hand our Vec2f to SFML calls that want an sf::Vector2f. These two thin
// converters are that bridge, and they live in their own header so that ONLY
// files that already include SFML (i.e. `main.cpp`) pull them in.
//
// Python note: there is no analogue — Python has no separate "compile the
// tests without this heavy dependency" concern. In C++ keeping the include
// graph small keeps build times and coupling down.
// ============================================================================

#include <SFML/System/Vector2.hpp>

#include "gmath/vec2.hpp"

namespace gmath {

template <typename T>
sf::Vector2<T> to_sfml(const Vec2<T>& v) {
    return sf::Vector2<T>(v.x, v.y);
}

template <typename T>
Vec2<T> from_sfml(const sf::Vector2<T>& v) {
    return Vec2<T>{v.x, v.y};
}

}  // namespace gmath
