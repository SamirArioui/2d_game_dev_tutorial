// Tests for the game state machine: only LEGAL transitions apply.
#include <catch2/catch_test_macros.hpp>

#include "game/game_state.hpp"

using game::GameState;
using game::StateMachine;

TEST_CASE("a fresh machine starts in the Menu", "[state]") {
    StateMachine sm;
    REQUIRE(sm.state() == GameState::Menu);
}

TEST_CASE("the happy path Menu -> Playing -> GameOver -> Menu is legal", "[state]") {
    StateMachine sm;
    REQUIRE(sm.transition_to(GameState::Playing));
    REQUIRE(sm.state() == GameState::Playing);
    REQUIRE(sm.transition_to(GameState::GameOver));
    REQUIRE(sm.state() == GameState::GameOver);
    REQUIRE(sm.transition_to(GameState::Menu));
    REQUIRE(sm.state() == GameState::Menu);
}

TEST_CASE("illegal transitions are rejected and leave the state unchanged", "[state]") {
    StateMachine sm;  // in Menu

    SECTION("Menu cannot jump straight to GameOver") {
        REQUIRE_FALSE(sm.transition_to(GameState::GameOver));
        REQUIRE(sm.state() == GameState::Menu);  // unchanged
    }
    SECTION("transitioning to the same state is not allowed") {
        REQUIRE_FALSE(sm.transition_to(GameState::Menu));
        REQUIRE(sm.state() == GameState::Menu);
    }
}

TEST_CASE("from GameOver you can play again or return to the menu", "[state]") {
    StateMachine sm;
    sm.transition_to(GameState::Playing);
    sm.transition_to(GameState::GameOver);

    SECTION("play again") {
        REQUIRE(sm.transition_to(GameState::Playing));
    }
    SECTION("back to menu") {
        REQUIRE(sm.transition_to(GameState::Menu));
    }
}
