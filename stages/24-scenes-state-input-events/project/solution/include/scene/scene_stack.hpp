#pragma once

// ===========================================================================
// SceneStack — a stack of scenes that drives whichever one is on top.
//
// Only the TOP scene updates and receives input; render draws bottom-to-top so an overlay (say a
// pause menu pushed on top of the playing scene) composes over the frozen game beneath it.
// Pushing suspends the scene below; popping resumes it. This is the classic game "screen stack".
//
// It owns its scenes via unique_ptr (stage 11): the stack is the single owner, and popping a
// scene destroys it automatically (RAII). Still SFML-free — see scene.hpp for why.
//
// DEFERRED CHANGES: push()/pop() may be called from INSIDE a scene's own update/handle_input/
// render — e.g. a pause menu that pops itself. Destroying that scene right away would delete
// `this` while its method is still running (use-after-free), and pushing while render() walks
// scenes_ could reallocate the vector under the loop. So while the stack is calling into a scene
// it only QUEUES the change; apply_pending() performs the queue once that call has returned.
// Outside such a call (e.g. from main) push/pop take effect immediately.
// ===========================================================================

#include <cstddef>
#include <memory>
#include <vector>

#include "scene/scene.hpp"

namespace scene {

class SceneStack {
public:
    void push(std::unique_ptr<Scene> scene);
    void pop();

    Scene* top() const;  // the active scene, or nullptr if the stack is empty
    bool empty() const;
    std::size_t size() const;

    // Drive the active scene. update() and handle_input() go to the TOP scene only...
    void update(float dt);
    void handle_input(const sf::Event& event);
    // ...but render() draws every scene bottom-to-top so overlays stack correctly.
    void render(sf::RenderTarget& target);

private:
    // One queued change. scene is the scene to push (empty for a pop).
    struct PendingChange {
        bool is_push;
        std::unique_ptr<Scene> scene;
    };

    void request(PendingChange change);  // queue it, and apply now unless we are busy
    void apply_pending();                // perform every queued change, in order

    std::vector<std::unique_ptr<Scene>> scenes_;
    std::vector<PendingChange> pending_;
    bool busy_{false};  // true while we are calling into a scene (or applying changes)
};

}  // namespace scene
