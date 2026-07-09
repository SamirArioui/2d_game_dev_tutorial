// Exercise 06 — A pause overlay on the scene stack (hard)
//
// The stack shines when you PUSH a scene ON TOP without removing the one below. A pause menu
// pushed over the playing scene freezes the game (only the top updates) yet the game is preserved
// underneath, ready to resume when you pop the overlay. This is why a stack beats a single
// "current scene" variable.
//
// The SceneStack below is complete (you built it in exercise 05). Your job is to use it:
//
// TODO:
//   1. At marker (A): push a PauseScene on top of the running GameScene.
//   2. At marker (B): pop the PauseScene to resume the game.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 06_pause_overlay.cpp -o ex && ./ex

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

    stack.update(0.016f);  // game running
    assert(game_ptr->ticks == 1);

    // (A) TODO: push a PauseScene overlay. Keep a raw pointer to it so we can check its ticks.
    PauseScene* pause_ptr = nullptr;  // replace: point this at the pushed PauseScene
    // ... push here ...

    stack.update(0.016f);  // paused: the overlay updates, the game is FROZEN underneath
    assert(pause_ptr != nullptr && pause_ptr->ticks == 1);
    assert(game_ptr->ticks == 1);  // game did NOT advance while paused

    // (B) TODO: pop the overlay to resume the game.
    // ... pop here ...

    stack.update(0.016f);  // resumed
    assert(game_ptr->ticks == 2);

    std::cout << "pause overlay works\n";
    return 0;
}
