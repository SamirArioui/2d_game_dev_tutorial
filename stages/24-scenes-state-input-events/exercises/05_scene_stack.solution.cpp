// Exercise 05 — The scene stack (solution)
#include <cassert>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void update(float dt) = 0;
};

class CountingScene : public Scene {
public:
    void update(float /*dt*/) override { ++updates; }
    int updates{0};
};

class SceneStack {
public:
    void push(std::unique_ptr<Scene> scene) { scenes_.push_back(std::move(scene)); }
    void pop() {
        if (!scenes_.empty()) {
            scenes_.pop_back();
        }
    }
    Scene* top() const { return scenes_.empty() ? nullptr : scenes_.back().get(); }
    bool empty() const { return scenes_.empty(); }

    void update(float dt) {
        if (Scene* active = top()) {
            active->update(dt);
        }
    }

private:
    std::vector<std::unique_ptr<Scene>> scenes_;
};

int main() {
    SceneStack stack;
    assert(stack.top() == nullptr);
    stack.update(0.016f);

    auto a = std::make_unique<CountingScene>();
    auto b = std::make_unique<CountingScene>();
    CountingScene* a_ptr = a.get();
    CountingScene* b_ptr = b.get();

    stack.push(std::move(a));
    stack.push(std::move(b));
    assert(stack.top() == b_ptr);

    stack.update(0.016f);
    assert(a_ptr->updates == 0);
    assert(b_ptr->updates == 1);

    stack.pop();  // b is destroyed here — do not touch b_ptr afterwards
    assert(stack.top() == a_ptr);
    stack.update(0.016f);
    assert(a_ptr->updates == 1);

    std::cout << "scene stack works\n";
    return 0;
}
