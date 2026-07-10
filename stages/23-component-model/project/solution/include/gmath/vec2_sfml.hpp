#pragma once

// ===========================================================================
// The one place gmath meets SFML. Keeping the conversion here (and OUT of vec2.hpp) means the
// math + game logic stay SFML-free and testable, while the renderer includes this small bridge to
// turn a gmath::Vec2f into the sf::Vector2f that SFML's draw calls expect.
// ===========================================================================

#include <SFML/System/Vector2.hpp>

#include "gmath/vec2.hpp"

namespace gmath {

template <class T>
sf::Vector2<T> to_sfml(const Vec2<T>& v) {
    return sf::Vector2<T>{v.x, v.y};
}

template <class T>
Vec2<T> from_sfml(const sf::Vector2<T>& v) {
    return Vec2<T>{v.x, v.y};
}

}  // namespace gmath
