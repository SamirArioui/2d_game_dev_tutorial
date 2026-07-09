#pragma once

// ============================================================================
// eng::Scene + eng::SceneStack — the unit of game state, and a stack of them.
//
// A Scene is one self-contained screen/mode: a menu, the gameplay, a pause
// overlay, a game-over screen. It exposes exactly the three hooks the main loop
// needs — update / render / handle_input — plus enter/exit notifications.
//
// A SceneStack lets scenes LAYER: pushing a pause menu on top of gameplay keeps
// the gameplay scene alive underneath (so it can resume), while the top scene
// is the one that runs. This is the classic game "state stack".
//
// Note this header is SFML-FREE: it only FORWARD-DECLARES sf::RenderTarget and
// sf::Event. A forward declaration is enough because they appear solely as
// reference parameters (`sf::RenderTarget&`) — the compiler never needs their
// full definition here, only where a scene actually draws. That keeps the scene
// machinery testable (you can push/pop/step a dummy scene with no SFML linked).
// ============================================================================

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace sf {
class RenderTarget;
class Event;
}  // namespace sf

namespace eng {

// Abstract base. A concrete scene overrides the three pure-virtual hooks; the
// engine's Application calls them for you (see application.hpp). This is the
// KEY inversion of control: scenes don't run a loop, they're driven by one.
class Scene {
public:
    virtual ~Scene() = default;

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
    virtual void handle_input(const sf::Event& event) = 0;

    // Optional lifecycle hooks — default to doing nothing, override if useful
    // (start music on enter, save on exit, ...).
    virtual void on_enter() {}
    virtual void on_exit() {}
};

// A stack of scenes. The TOP scene is the "current" one that the loop drives.
class SceneStack {
public:
    void push(std::unique_ptr<Scene> scene) {
        if (scene) {
            scene->on_enter();
            stack_.push_back(std::move(scene));
        }
    }

    // Remove and return the top scene (or nullptr if empty), calling on_exit.
    std::unique_ptr<Scene> pop() {
        if (stack_.empty()) {
            return nullptr;
        }
        std::unique_ptr<Scene> top = std::move(stack_.back());
        stack_.pop_back();
        top->on_exit();
        return top;
    }

    // Swap the top scene for a new one (pop + push in a single call).
    void replace(std::unique_ptr<Scene> scene) {
        pop();
        push(std::move(scene));
    }

    Scene* current() const { return stack_.empty() ? nullptr : stack_.back().get(); }
    bool empty() const { return stack_.empty(); }
    std::size_t size() const { return stack_.size(); }

    // The loop calls these; each simply forwards to the current (top) scene.
    void update(float dt) {
        if (Scene* s = current()) {
            s->update(dt);
        }
    }
    void render(sf::RenderTarget& target) {
        if (Scene* s = current()) {
            s->render(target);
        }
    }
    void handle_input(const sf::Event& event) {
        if (Scene* s = current()) {
            s->handle_input(event);
        }
    }

private:
    std::vector<std::unique_ptr<Scene>> stack_;
};

}  // namespace eng
