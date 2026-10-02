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

class SceneStack;

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

protected:
    // The stack this scene lives on (set when it is pushed), so a scene can
    // change scenes itself: `stack()->push(...)`, or `stack()->pop()` to close
    // itself. Safe from inside update/render/handle_input — see SceneStack.
    SceneStack* stack() const { return owner_; }

private:
    friend class SceneStack;
    SceneStack* owner_{nullptr};
};

// A stack of scenes. The TOP scene is the "current" one that the loop drives.
//
// DEFERRED CHANGES: push/pop/replace are usually called by a scene from INSIDE
// its own update/handle_input (a menu that pops itself, a game that pushes a
// pause overlay). Destroying that scene right away would delete `this` while
// its method is still running — a use-after-free. So while the stack is busy
// calling into a scene, a change is only QUEUED, and it is applied once that
// call has returned (a safe point). Outside such a call — e.g. main() pushing
// the first scene — changes apply immediately.
class SceneStack {
public:
    void push(std::unique_ptr<Scene> scene) {
        if (scene) {
            request({Op::Push, std::move(scene)});
        }
    }

    // Remove the top scene (calling on_exit) — a no-op on an empty stack.
    void pop() { request({Op::Pop, nullptr}); }

    // Swap the top scene for a new one (pop + push as one change).
    void replace(std::unique_ptr<Scene> scene) { request({Op::Replace, std::move(scene)}); }

    Scene* current() const { return stack_.empty() ? nullptr : stack_.back().get(); }
    bool empty() const { return stack_.empty(); }
    std::size_t size() const { return stack_.size(); }

    // The loop calls these; each forwards to the current (top) scene, then
    // applies whatever changes the scene requested meanwhile.
    void update(float dt) {
        dispatch([dt](Scene& s) { s.update(dt); });
    }
    void render(sf::RenderTarget& target) {
        dispatch([&target](Scene& s) { s.render(target); });
    }
    void handle_input(const sf::Event& event) {
        dispatch([&event](Scene& s) { s.handle_input(event); });
    }

private:
    enum class Op { Push, Pop, Replace };
    struct Change {
        Op op;
        std::unique_ptr<Scene> scene;  // the scene to push (empty for Pop)
    };

    void request(Change change) {
        pending_.push_back(std::move(change));
        if (!busy_) {
            apply_pending();
        }
    }

    template <class F>
    void dispatch(F&& call) {
        busy_ = true;
        if (Scene* s = current()) {
            call(*s);
        }
        apply_pending();
    }

    void apply_pending() {
        busy_ = true;  // on_enter/on_exit may request more changes: queue them too
        // Index loop: applying a change can append to pending_ (and reallocate it).
        for (std::size_t i = 0; i < pending_.size(); ++i) {
            Change change = std::move(pending_[i]);
            if (change.op != Op::Push && !stack_.empty()) {
                std::unique_ptr<Scene> top = std::move(stack_.back());
                stack_.pop_back();
                top->on_exit();
            }  // `top` is destroyed here — no scene method is running any more
            if (change.op != Op::Pop && change.scene) {
                change.scene->owner_ = this;
                change.scene->on_enter();
                stack_.push_back(std::move(change.scene));
            }
        }
        pending_.clear();
        busy_ = false;
    }

    std::vector<std::unique_ptr<Scene>> stack_;
    std::vector<Change> pending_;
    bool busy_{false};  // true while calling into a scene or applying changes
};

}  // namespace eng
