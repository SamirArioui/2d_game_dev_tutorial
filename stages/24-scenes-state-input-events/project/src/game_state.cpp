// Stage 24 mini-project — the game state machine (implementation) — YOUR TASK.
//
// This is the STARTER: both methods are stubbed with a TODO and a placeholder return so the
// project compiles and links, but tests/test_game_state.cpp FAILS until you implement them. The
// declarations (the GameState enum, the class, and the state() accessor) live in game_state.hpp.
//
// A complete reference is in ../solution/src/game_state.cpp — try it yourself first.
#include "game/game_state.hpp"

namespace game {

// The legal transition table. Read it as: "from state X, which target states are allowed?"
bool StateMachine::is_legal(GameState from, GameState to) {
    // TODO(stage 24): return true only for the LEGAL moves, everything else false:
    //   Menu     -> Playing
    //   Playing  -> GameOver | Menu
    //   GameOver -> Menu | Playing
    // (A state may never transition to itself.) A switch over `from` reads cleanly here.
    return false;  // placeholder
}

bool StateMachine::transition_to(GameState to) {
    // TODO(stage 24): if is_legal(state_, to), apply the move (set state_ = to) and return true;
    // otherwise leave state_ unchanged and return false.
    return false;  // placeholder
}

}  // namespace game
