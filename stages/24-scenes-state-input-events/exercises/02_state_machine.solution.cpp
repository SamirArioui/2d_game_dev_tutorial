// Exercise 02 — A guarded state machine (solution)
#include <cassert>
#include <iostream>

enum class GameState { Menu, Playing, GameOver };

class StateMachine {
public:
    GameState state() const { return state_; }

    bool transition_to(GameState to) {
        if (!is_legal(state_, to)) {
            return false;
        }
        state_ = to;
        return true;
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

    assert(sm.transition_to(GameState::Playing));
    assert(sm.state() == GameState::Playing);

    assert(!sm.transition_to(GameState::Playing));
    assert(sm.state() == GameState::Playing);

    assert(sm.transition_to(GameState::GameOver));
    assert(!sm.transition_to(GameState::GameOver));

    std::cout << "state machine guards transitions\n";
    return 0;
}
