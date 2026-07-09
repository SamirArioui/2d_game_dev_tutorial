#include "scene/scene_stack.hpp"

#include <utility>

// NOTE: no SFML include here either. We only ever PASS the sf::RenderTarget&/sf::Event& references
// through to the scenes; we never touch their members, so the forward declarations suffice and
// this whole file compiles and links without SFML — which is what makes it unit-testable.

namespace scene {

void SceneStack::push(std::unique_ptr<Scene> scene) {
    scenes_.push_back(std::move(scene));
}

void SceneStack::pop() {
    if (!scenes_.empty()) {
        scenes_.pop_back();  // unique_ptr destructor deletes the scene for us (RAII)
    }
}

Scene* SceneStack::top() const {
    if (scenes_.empty()) {
        return nullptr;
    }
    return scenes_.back().get();
}

bool SceneStack::empty() const {
    return scenes_.empty();
}

std::size_t SceneStack::size() const {
    return scenes_.size();
}

void SceneStack::update(float dt) {
    if (Scene* active = top()) {
        active->update(dt);
    }
}

void SceneStack::handle_input(const sf::Event& event) {
    if (Scene* active = top()) {
        active->handle_input(event);
    }
}

void SceneStack::render(sf::RenderTarget& target) {
    for (const auto& scene : scenes_) {
        scene->render(target);
    }
}

}  // namespace scene
