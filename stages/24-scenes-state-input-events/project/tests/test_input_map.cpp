// Tests for input action mapping: keys -> semantic actions, and rebinding.
#include <catch2/catch_test_macros.hpp>

#include "input/action.hpp"

using input::Action;
using input::InputMap;

// Stand-in key codes. In the app these are static_cast<int>(sf::Keyboard::W) etc.; the point of
// the InputMap being SFML-free is that the tests don't care what the numbers really mean.
namespace {
constexpr int KeyW = 22;
constexpr int KeyS = 18;
constexpr int KeyEnter = 58;
constexpr int KeyUnbound = 999;
}  // namespace

TEST_CASE("bound keys resolve to their action; unbound keys resolve to nothing", "[input]") {
    InputMap map;
    map.bind(KeyW, Action::MoveUp);
    map.bind(KeyS, Action::MoveDown);
    map.bind(KeyEnter, Action::Confirm);

    REQUIRE(map.action_for(KeyW) == Action::MoveUp);
    REQUIRE(map.action_for(KeyS) == Action::MoveDown);
    REQUIRE(map.action_for(KeyEnter) == Action::Confirm);

    // An unbound key returns an empty optional (Python's `None`).
    REQUIRE_FALSE(map.action_for(KeyUnbound).has_value());
    REQUIRE(map.is_bound(KeyW));
    REQUIRE_FALSE(map.is_bound(KeyUnbound));
}

TEST_CASE("rebinding a key changes which action it triggers", "[input]") {
    InputMap map;
    map.bind(KeyW, Action::MoveUp);
    REQUIRE(map.action_for(KeyW) == Action::MoveUp);

    // The whole payoff: change the binding in ONE place, gameplay code is untouched.
    map.bind(KeyW, Action::Confirm);
    REQUIRE(map.action_for(KeyW) == Action::Confirm);
    REQUIRE(map.size() == 1);  // rebinding replaced, it did not add a second entry
}

TEST_CASE("unbinding removes a key", "[input]") {
    InputMap map;
    map.bind(KeyEnter, Action::Confirm);
    map.unbind(KeyEnter);
    REQUIRE_FALSE(map.is_bound(KeyEnter));
    REQUIRE(map.size() == 0);
}
