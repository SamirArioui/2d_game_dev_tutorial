#pragma once

// ===========================================================================
// The game's high-level STATE, plus a tiny state machine that guards the legal moves between
// states. Pure logic, no SFML — so it is unit-tested directly.
//
// Python comparison: a state machine is usually a dict-of-dicts transition table,
//     TRANSITIONS = {Menu: {Playing}, Playing: {GameOver, Menu}, GameOver: {Menu, Playing}}
// and you refuse any move that isn't in the table. That's exactly what StateMachine does.
// ===========================================================================

namespace game {

// `enum class` (scoped enum, stage 03): the states our game can be in. Scoped means you must
// write `GameState::Menu`, and there's no accidental conversion to int.
enum class GameState { Menu, Playing, GameOver };

class StateMachine {
public:
    GameState state() const { return state_; }

    // Attempt to move to `to`. Applies and returns true only if the move is LEGAL; otherwise the
    // state is unchanged and it returns false. Guarding transitions here means gameplay code can
    // never shove the game into a nonsensical state (e.g. Menu straight to GameOver).
    bool transition_to(GameState to);

private:
    static bool is_legal(GameState from, GameState to);

    GameState state_{GameState::Menu};  // every game boots into the menu
};

}  // namespace game
