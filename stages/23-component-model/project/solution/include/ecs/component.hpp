#pragma once

// ===========================================================================
// Components — plain DATA structs. No behaviour, no methods, no inheritance.
//
// This is the whole point of the component model: a component answers "what data does this thing
// have?", never "what does this thing do?". Behaviour lives in SYSTEMS (free functions that walk
// the entities). Because these are pure data with no SFML in sight, everything that reads them
// stays unit-testable.
//
// Python comparison: think of each component as a small @dataclass you attach to an object,
// rather than a base class you inherit from.
// ===========================================================================

#include <cstdint>

#include "gmath/vec2.hpp"

namespace ecs {

// Where the entity is. (Position only — velocity is its own component so that a system can ask for
// "things that MOVE" separately from "things that have a place in the world".)
struct Transform {
    gmath::Vec2f pos{0.0f, 0.0f};
};

// How fast the entity is moving, in pixels per second.
struct Velocity {
    gmath::Vec2f vel{0.0f, 0.0f};
};

// A rectangle to draw, described as pure data. Colour is stored as raw RGBA bytes so this header
// needs no SFML; the renderer turns it into an sf::Color at draw time.
struct Shape {
    gmath::Vec2f size{0.0f, 0.0f};
    std::uint8_t r{255};
    std::uint8_t g{255};
    std::uint8_t b{255};
    std::uint8_t a{255};
};

}  // namespace ecs
