#include "scene/scene_stack.hpp"

#include <utility>

// NOTE: no SFML include here either. We only ever PASS the sf::RenderTarget&/sf::Event& references
// through to the scenes; we never touch their members, so the forward declarations suffice and
// this whole file compiles and links without SFML — which is what makes it unit-testable.

namespace scene {

void SceneStack::push(std::unique_ptr<Scene> scene) {
    request({true, std::move(scene)});
}

void SceneStack::pop() {
    request({false, nullptr});
}

void SceneStack::request(PendingChange change) {
    pending_.push_back(std::move(change));
    if (!busy_) {
        apply_pending();  // nobody is mid-call into a scene: safe to apply right now
    }
}

void SceneStack::apply_pending() {
    busy_ = true;  // a scene destructor that pushes/pops only queues more work for this loop
    // Index loop, not range-for: applying a change can append to pending_ (and reallocate it).
    for (std::size_t i = 0; i < pending_.size(); ++i) {
        PendingChange change = std::move(pending_[i]);
        if (change.is_push) {
            if (change.scene) {
                scenes_.push_back(std::move(change.scene));
            }
        } else if (!scenes_.empty()) {
            scenes_.pop_back();  // unique_ptr destructor deletes the scene for us (RAII)
        }
    }
    pending_.clear();
    busy_ = false;
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

// Each driver marks the stack busy while it calls into scenes, so any push/pop a scene makes is
// queued instead of destroying or reallocating something we're still using. Afterwards — the safe
// point — we apply the queue.
void SceneStack::update(float dt) {
    busy_ = true;
    if (Scene* active = top()) {
        active->update(dt);
    }
    apply_pending();
}

void SceneStack::handle_input(const sf::Event& event) {
    busy_ = true;
    if (Scene* active = top()) {
        active->handle_input(event);
    }
    apply_pending();
}

void SceneStack::render(sf::RenderTarget& target) {
    busy_ = true;
    for (const auto& scene : scenes_) {
        scene->render(target);
    }
    apply_pending();
}

}  // namespace scene
