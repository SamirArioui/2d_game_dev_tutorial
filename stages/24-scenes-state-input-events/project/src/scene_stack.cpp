// Stage 24 mini-project — the scene stack (implementation) — YOUR TASK.
//
// This is the STARTER: every method is stubbed with a TODO and a placeholder return so the project
// compiles and links, but tests/test_scene_stack.cpp FAILS until you implement them. The
// declarations (and the scenes_ storage) live in scene_stack.hpp.
//
// A complete reference is in ../solution/src/scene_stack.cpp — try it yourself first.
#include "scene/scene_stack.hpp"

#include <utility>

// NOTE: no SFML include here either. You only ever PASS the sf::RenderTarget&/sf::Event& references
// through to the scenes; never touch their members, so the forward declarations suffice and this
// whole file compiles and links without SFML — which is what makes it unit-testable.

namespace scene {

void SceneStack::push(std::unique_ptr<Scene> scene) {
    // TODO(stage 24): hand the change to request() as a push: request({true, std::move(scene)}).
}

void SceneStack::pop() {
    // TODO(stage 24): hand the change to request() as a pop: request({false, nullptr}).
}

void SceneStack::request(PendingChange change) {
    // TODO(stage 24): append `change` to pending_ (std::move). If we are NOT busy_ (no scene is
    // mid-call), call apply_pending() so the change takes effect immediately.
}

void SceneStack::apply_pending() {
    // TODO(stage 24): set busy_ = true, then walk pending_ IN ORDER with an index loop (applying a
    // change may append to pending_, which can reallocate it, so no range-for):
    //   - a push moves its scene onto scenes_ (skip a null scene);
    //   - a pop removes the top scene if there is one (the unique_ptr destructor deletes it —
    //     RAII). Popping an empty stack must be a safe no-op.
    // Finally clear pending_ and set busy_ = false.
}

Scene* SceneStack::top() const {
    // TODO(stage 24): return a raw (non-owning) pointer to the top scene, or nullptr if empty.
    return nullptr;  // placeholder
}

bool SceneStack::empty() const {
    // TODO(stage 24): return whether the stack has no scenes.
    return true;  // placeholder
}

std::size_t SceneStack::size() const {
    // TODO(stage 24): return how many scenes are on the stack.
    return 0;  // placeholder
}

// For the three drivers below: set busy_ = true BEFORE calling into any scene and call
// apply_pending() afterwards. That way a scene that pushes or pops (even popping itself) only
// queues the change, and it is applied once we are no longer inside the scene's method.

void SceneStack::update(float dt) {
    // TODO(stage 24): forward update(dt) to the TOP scene only (if there is one).
}

void SceneStack::handle_input(const sf::Event& event) {
    // TODO(stage 24): forward the event to the TOP scene only (if there is one).
}

void SceneStack::render(sf::RenderTarget& target) {
    // TODO(stage 24): draw EVERY scene bottom-to-top so overlays compose over the scenes beneath.
}

}  // namespace scene
