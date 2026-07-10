#include "game/game_state.hpp"

namespace game {

// The legal transition table, written as a switch. Read it as: "from state X, which target
// states are allowed?" Anything not listed is rejected.
bool StateMachine::is_legal(GameState from, GameState to) {
    switch (from) {
        case GameState::Menu:
            return to == GameState::Playing;  // start the game
        case GameState::Playing:
            return to == GameState::GameOver   // you lost / the round ended
                   || to == GameState::Menu;   // quit back to the menu
        case GameState::GameOver:
            return to == GameState::Menu       // back to the menu
                   || to == GameState::Playing; // play again
    }
    return false;
}

bool StateMachine::transition_to(GameState to) {
    if (!is_legal(state_, to)) {
        return false;  // reject: state stays put
    }
    state_ = to;
    return true;
}

}  // namespace game
