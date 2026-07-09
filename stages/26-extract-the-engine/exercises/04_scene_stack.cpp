// Exercise 04 — SceneStack (medium)
//
// The scene stack drives whichever scene is on TOP. Push a couple of dummy
// scenes and check that current()/size()/pop() behave. This links no SFML: the
// dummy scene's render/handle_input just ignore their (forward-declared) SFML
// parameters.

#include <catch2/catch_test_macros.hpp>

#include <memory>

#include "eng/scene.hpp"

namespace {

// A minimal scene that counts how many times it was updated. Because Scene's
// render/handle_input parameters are only forward-declared SFML types, we can
// override them without including SFML — as long as we don't touch the objects.
struct CountingScene : eng::Scene {
    int updates = 0;
    void update(float) override { ++updates; }
    void render(sf::RenderTarget&) override {}
    void handle_input(const sf::Event&) override {}
};

}  // namespace

TEST_CASE("stack drives the top scene", "[ex04]") {
    eng::SceneStack stack;
    REQUIRE(stack.empty());

    stack.push(std::make_unique<CountingScene>());
    stack.push(std::make_unique<CountingScene>());

    // TODO: how many scenes are on the stack now?
    REQUIRE(stack.size() == 0);  // <-- fix

    // update() forwards to the TOP scene only. Step it a few times.
    stack.update(0.016f);
    stack.update(0.016f);

    auto* top = static_cast<CountingScene*>(stack.current());
    // TODO: how many updates did the TOP scene receive?
    REQUIRE(top->updates == 99);  // <-- fix

    // TODO: after one pop, what is the size?
    stack.pop();
    REQUIRE(stack.size() == 99);  // <-- fix
}
