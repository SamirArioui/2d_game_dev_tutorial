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
    // TODO(stage 24): take ownership of `scene` by moving it onto scenes_ (std::move).
}

void SceneStack::pop() {
    // TODO(stage 24): remove the top scene if the stack is non-empty (the unique_ptr destructor
    // deletes it for you — RAII). Popping an empty stack must be a safe no-op.
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
