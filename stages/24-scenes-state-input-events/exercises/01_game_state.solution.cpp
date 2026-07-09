// Exercise 01 — A game-state enum and its cycle (solution)
#include <cassert>
#include <iostream>

enum class GameState { Menu, Playing, GameOver };

GameState advance(GameState s) {
    switch (s) {
        case GameState::Menu:
            return GameState::Playing;
        case GameState::Playing:
            return GameState::GameOver;
        case GameState::GameOver:
            return GameState::Menu;
    }
    return GameState::Menu;
}

const char* name(GameState s) {
    switch (s) {
        case GameState::Menu:
            return "Menu";
        case GameState::Playing:
            return "Playing";
        case GameState::GameOver:
            return "GameOver";
    }
    return "?";
}

int main() {
    GameState s = GameState::Menu;
    std::cout << name(s) << " -> ";
    s = advance(s);
    std::cout << name(s) << " -> ";
    s = advance(s);
    std::cout << name(s) << " -> ";
    s = advance(s);
    std::cout << name(s) << "\n";

    assert(advance(GameState::Menu) == GameState::Playing);
    assert(advance(GameState::Playing) == GameState::GameOver);
    assert(advance(GameState::GameOver) == GameState::Menu);
    return 0;
}
