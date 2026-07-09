// Exercise 04 — SceneStack (solution)
#include <catch2/catch_test_macros.hpp>

#include <memory>

#include "eng/scene.hpp"

namespace {

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

    REQUIRE(stack.size() == 2);

    stack.update(0.016f);
    stack.update(0.016f);

    auto* top = static_cast<CountingScene*>(stack.current());
    REQUIRE(top->updates == 2);  // only the top scene runs

    stack.pop();
    REQUIRE(stack.size() == 1);
}
