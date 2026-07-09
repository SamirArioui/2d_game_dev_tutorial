// Exercise 06 — A pause overlay on the scene stack (solution)
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

class GameScene : public Scene {
public:
    void update(float /*dt*/) override { ++ticks; }
    int ticks{0};
};

class PauseScene : public Scene {
public:
    void update(float /*dt*/) override { ++ticks; }
    int ticks{0};
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

    auto game = std::make_unique<GameScene>();
    GameScene* game_ptr = game.get();
    stack.push(std::move(game));

    stack.update(0.016f);
    assert(game_ptr->ticks == 1);

    // (A) push the overlay WITHOUT removing the game beneath it.
    auto pause = std::make_unique<PauseScene>();
    PauseScene* pause_ptr = pause.get();
    stack.push(std::move(pause));

    stack.update(0.016f);
    assert(pause_ptr != nullptr && pause_ptr->ticks == 1);
    assert(game_ptr->ticks == 1);  // frozen while paused

    // (B) pop the overlay to resume; the preserved game scene is back on top.
    stack.pop();

    stack.update(0.016f);
    assert(game_ptr->ticks == 2);

    std::cout << "pause overlay works\n";
    return 0;
}
