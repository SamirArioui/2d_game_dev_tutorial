#pragma once

// ===========================================================================
// Scene — the canonical interface for one screen/mode of the game (Menu, Playing, GameOver).
//
// This is the SHARED CONTRACT the rest of the course depends on: stage 26 lifts this exact
// interface into `namespace eng`, and the capstone (27) builds its screens on it.
//
// Notice we FORWARD-DECLARE the SFML types instead of including SFML here. A forward declaration
// ("this class exists; details later") is enough to name a type in a reference parameter, and it
// keeps this header — and the SceneStack that drives it — SFML-free, so the stack's logic is
// unit-testable without a window. Only the concrete scenes (in the app) include real SFML to draw.
//
// Python comparison: an abstract base class (abc.ABC) whose three @abstractmethods every concrete
// screen must implement.
// ===========================================================================

namespace sf {
class RenderTarget;
class Event;
}  // namespace sf

namespace scene {

class Scene {
public:
    // A virtual destructor so `delete scene_ptr;` through a Scene* runs the derived destructor.
    // (Stage 07's rule: any class meant to be deleted through a base pointer needs this.)
    virtual ~Scene() = default;

    virtual void update(float dt) = 0;                      // advance the scene by dt seconds
    virtual void render(sf::RenderTarget& target) = 0;      // draw the scene into `target`
    virtual void handle_input(const sf::Event& event) = 0;  // react to one input event
};

}  // namespace scene
