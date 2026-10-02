// Tests for the engine's PURE core — the parts that compile without SFML.
//
// These guard the engine itself (the game's tests live in project/tests). They
// link only Catch2: scene.hpp forward-declares the sf:: types and
// resource_manager.hpp is a header-only template, so no window is needed.
#include <memory>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include "eng/resource_manager.hpp"
#include "eng/scene.hpp"

namespace {

// Counts what happens to it, through pointers that outlive the scene itself.
struct ProbeScene : eng::Scene {
    int* updates;
    bool* destroyed;
    bool pop_self_on_update = false;
    std::unique_ptr<eng::Scene> push_on_update;

    ProbeScene(int* u, bool* d) : updates(u), destroyed(d) {}
    ~ProbeScene() override { *destroyed = true; }

    void update(float /*dt*/) override {
        if (push_on_update) {
            stack()->push(std::move(push_on_update));
        }
        if (pop_self_on_update) {
            stack()->pop();  // only QUEUED: the stack is busy calling us
        }
        ++(*updates);  // `this` must still be alive here (ASan would flag a use-after-free)
    }
    void render(sf::RenderTarget& /*target*/) override {}
    void handle_input(const sf::Event& /*event*/) override {}
};

struct FakeResource {
    static int loads;
    std::string path;
    bool loadFromFile(const std::string& p) {
        ++loads;
        path = p;
        return true;
    }
};
int FakeResource::loads = 0;

}  // namespace

TEST_CASE("a scene can pop itself from inside update", "[engine][scene]") {
    int base_updates = 0, top_updates = 0;
    bool base_dead = false, top_dead = false;

    eng::SceneStack stack;
    stack.push(std::make_unique<ProbeScene>(&base_updates, &base_dead));
    auto top = std::make_unique<ProbeScene>(&top_updates, &top_dead);
    top->pop_self_on_update = true;
    stack.push(std::move(top));
    REQUIRE(stack.size() == 2);

    stack.update(0.016f);
    REQUIRE(top_updates == 1);  // update() ran to completion...
    REQUIRE(top_dead);          // ...and only then was the scene destroyed
    REQUIRE(stack.size() == 1);

    stack.update(0.016f);
    REQUIRE(base_updates == 1);  // the scene beneath is current again
    REQUIRE_FALSE(base_dead);
}

TEST_CASE("a push requested during update applies after it", "[engine][scene]") {
    int a_updates = 0, b_updates = 0;
    bool a_dead = false, b_dead = false;

    eng::SceneStack stack;
    auto a = std::make_unique<ProbeScene>(&a_updates, &a_dead);
    a->push_on_update = std::make_unique<ProbeScene>(&b_updates, &b_dead);
    stack.push(std::move(a));

    stack.update(0.016f);
    REQUIRE(stack.size() == 2);
    REQUIRE(b_updates == 0);  // the new scene did not run in the frame it was pushed

    stack.update(0.016f);
    REQUIRE(a_updates == 1);  // only the top scene updates
    REQUIRE(b_updates == 1);
}

TEST_CASE("resource manager loads each path once and keeps addresses stable", "[engine][resources]") {
    FakeResource::loads = 0;
    eng::ResourceManager<FakeResource> cache;

    FakeResource& first = cache.get("player.png");
    for (int i = 0; i < 100; ++i) {  // plenty of inserts: the map rehashes several times
        cache.get("tile" + std::to_string(i) + ".png");
    }
    FakeResource& again = cache.get("player.png");

    REQUIRE(&first == &again);  // same object, same address
    REQUIRE(first.path == "player.png");
    REQUIRE(FakeResource::loads == 101);  // 1 + 100, the second "player.png" was a cache hit
}
