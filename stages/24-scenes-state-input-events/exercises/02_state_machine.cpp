// Exercise 02 — A guarded state machine (medium)
//
// A raw enum lets any code assign any state — including nonsense like Menu straight to GameOver.
// A state machine GUARDS transitions: it only applies legal moves and rejects the rest.
//
// Python comparison: check the move against a transition table before committing to it.
//
// TODO: implement transition_to so that it:
//   - applies the move and returns true when is_legal(current, target) is true;
//   - otherwise leaves the state unchanged and returns false.
// (is_legal is already written for you below.)
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_state_machine.cpp -o ex && ./ex

#include <cassert>
#include <iostream>

enum class GameState { Menu, Playing, GameOver };

class StateMachine {
public:
    GameState state() const { return state_; }

    bool transition_to(GameState to) {
        // TODO: use is_legal(state_, to); on success set state_ = to and return true; else false.
        (void)to;
        return false;
    }

private:
    static bool is_legal(GameState from, GameState to) {
        switch (from) {
            case GameState::Menu:
                return to == GameState::Playing;
            case GameState::Playing:
                return to == GameState::GameOver || to == GameState::Menu;
            case GameState::GameOver:
                return to == GameState::Menu || to == GameState::Playing;
        }
        return false;
    }

    GameState state_{GameState::Menu};
};

int main() {
    StateMachine sm;
    assert(sm.state() == GameState::Menu);

    assert(sm.transition_to(GameState::Playing));       // legal
    assert(sm.state() == GameState::Playing);

    assert(!sm.transition_to(GameState::Playing));      // illegal: no self-loop
    assert(sm.state() == GameState::Playing);           // unchanged

    assert(sm.transition_to(GameState::GameOver));      // legal
    assert(!sm.transition_to(GameState::GameOver));     // illegal now

    std::cout << "state machine guards transitions\n";
    return 0;
}
