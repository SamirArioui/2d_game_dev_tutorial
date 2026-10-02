// Tests for the scene stack's push/pop/top mechanics and "top scene only" updating.
//
// We can test all of this WITHOUT SFML because scene.hpp forward-declares the SFML types: a mock
// scene can override render()/handle_input() with empty bodies (we never call them here) and we
// drive update(), which takes only a float. This is the payoff of keeping the stack SFML-free.
#include <memory>

#include <catch2/catch_test_macros.hpp>

#include "scene/scene.hpp"
#include "scene/scene_stack.hpp"

namespace {

// A test double that counts how many times it was update()d.
struct MockScene : scene::Scene {
    int* update_count;
    explicit MockScene(int* counter) : update_count(counter) {}

    void update(float /*dt*/) override { ++(*update_count); }
    void render(sf::RenderTarget& /*target*/) override {}          // never called in these tests
    void handle_input(const sf::Event& /*event*/) override {}      // never called in these tests
};

}  // namespace

TEST_CASE("an empty stack has no top and updating it is a no-op", "[scene]") {
    scene::SceneStack stack;
    REQUIRE(stack.empty());
    REQUIRE(stack.size() == 0);
    REQUIRE(stack.top() == nullptr);
    REQUIRE_NOTHROW(stack.update(0.016f));  // must not crash on an empty stack
}

TEST_CASE("push/pop/top behave like a stack", "[scene]") {
    scene::SceneStack stack;
    int a_updates = 0;
    int b_updates = 0;

    stack.push(std::make_unique<MockScene>(&a_updates));
    REQUIRE(stack.size() == 1);
    REQUIRE(stack.top() != nullptr);

    stack.push(std::make_unique<MockScene>(&b_updates));
    REQUIRE(stack.size() == 2);

    // Only the TOP scene (b) updates.
    stack.update(0.016f);
    REQUIRE(a_updates == 0);
    REQUIRE(b_updates == 1);

    // Pop b; a is resumed and now receives updates.
    stack.pop();
    REQUIRE(stack.size() == 1);
    stack.update(0.016f);
    REQUIRE(a_updates == 1);
    REQUIRE(b_updates == 1);  // b, popped, no longer updates

    stack.pop();
    REQUIRE(stack.empty());
}

namespace {

// A scene that pops ITSELF from inside its own update(). If the stack destroyed it immediately,
// the rest of update() would run on a deleted object (use-after-free — ASan would report it).
struct SelfPoppingScene : scene::Scene {
    scene::SceneStack& stack;
    int updates = 0;
    explicit SelfPoppingScene(scene::SceneStack& s) : stack(s) {}

    void update(float /*dt*/) override {
        stack.pop();  // request: only QUEUED, because the stack is busy calling us
        ++updates;    // still safe: `this` is alive until update() has returned
    }
    void render(sf::RenderTarget& /*target*/) override {}
    void handle_input(const sf::Event& /*event*/) override {}
};

}  // namespace

TEST_CASE("a scene can safely pop itself during update", "[scene]") {
    scene::SceneStack stack;
    int base_updates = 0;
    stack.push(std::make_unique<MockScene>(&base_updates));
    stack.push(std::make_unique<SelfPoppingScene>(stack));
    REQUIRE(stack.size() == 2);

    stack.update(0.016f);  // the self-pop is applied AFTER update() returns
    REQUIRE(stack.size() == 1);
    REQUIRE(base_updates == 0);

    stack.update(0.016f);  // the scene underneath is active again
    REQUIRE(base_updates == 1);
}

TEST_CASE("push and pop outside a scene call take effect immediately", "[scene]") {
    scene::SceneStack stack;
    int updates = 0;
    stack.push(std::make_unique<MockScene>(&updates));
    REQUIRE(stack.size() == 1);  // not queued: nobody was mid-call
    stack.pop();
    REQUIRE(stack.empty());
    REQUIRE_NOTHROW(stack.pop());  // popping an empty stack is a no-op
}
