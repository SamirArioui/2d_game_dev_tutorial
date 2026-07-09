// Exercise 05 — The scene stack (medium-hard)
//
// A scene stack drives whichever screen is on top: only the TOP scene updates and gets input.
// (The real Scene in the mini-project also has render()/handle_input() taking SFML types; here we
// strip it down to update() so the drill stays window-free and about the STACK mechanics.)
//
// TODO: implement push, pop, top, and update on SceneStack.
//   - top() returns the last-pushed scene, or nullptr when empty.
//   - update(dt) forwards to the TOP scene only (do nothing if empty).
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_scene_stack.cpp -o ex && ./ex

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

// Minimal scene interface (compare scene/scene.hpp in the mini-project, which adds render/input).
class Scene {
public:
    virtual ~Scene() = default;
    virtual void update(float dt) = 0;
};

// A test scene that counts its updates.
class CountingScene : public Scene {
public:
    void update(float /*dt*/) override { ++updates; }
    int updates{0};
};

class SceneStack {
public:
    void push(std::unique_ptr<Scene> scene) {
        // TODO: add `scene` to the top of the stack.
        (void)scene;
    }
    void pop() {
        // TODO: remove the top scene if the stack isn't empty.
    }
    Scene* top() const {
        // TODO: return the top scene, or nullptr if empty.
        return nullptr;
    }
    bool empty() const { return scenes_.empty(); }

    void update(float dt) {
        // TODO: forward to top() only, if there is one.
        (void)dt;
    }

private:
    std::vector<std::unique_ptr<Scene>> scenes_;
};

int main() {
    SceneStack stack;
    assert(stack.top() == nullptr);
    stack.update(0.016f);  // must be a safe no-op when empty

    auto a = std::make_unique<CountingScene>();
    auto b = std::make_unique<CountingScene>();
    CountingScene* a_ptr = a.get();
    CountingScene* b_ptr = b.get();

    stack.push(std::move(a));
    stack.push(std::move(b));
    assert(stack.top() == b_ptr);

    stack.update(0.016f);      // only b (the top) updates
    assert(a_ptr->updates == 0);
    assert(b_ptr->updates == 1);

    stack.pop();               // b is DESTROYED here (unique_ptr) — do not touch b_ptr afterwards
    assert(stack.top() == a_ptr);
    stack.update(0.016f);      // a is back on top and now updates
    assert(a_ptr->updates == 1);

    std::cout << "scene stack works\n";
    return 0;
}
