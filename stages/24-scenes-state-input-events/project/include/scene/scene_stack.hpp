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
    std::vector<std::unique_ptr<Scene>> scenes_;
};

}  // namespace scene
