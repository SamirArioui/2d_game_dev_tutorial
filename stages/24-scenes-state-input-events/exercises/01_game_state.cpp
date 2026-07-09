// Exercise 01 — A game-state enum and its cycle (easy)
//
// Games are almost always in exactly one high-level STATE. Model the states with an `enum class`
// and write the function that advances to the next one: Menu -> Playing -> GameOver -> Menu ...
//
// Python comparison: an Enum plus a tiny function; you might even use a dict {state: next}.
//
// TODO:
//   1. Complete `advance` so it returns the next state in the cycle.
//   2. Complete `name` so it returns the state's name as text.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_game_state.cpp -o ex && ./ex

#include <cassert>
#include <iostream>

enum class GameState { Menu, Playing, GameOver };

GameState advance(GameState s) {
    // TODO: return Playing after Menu, GameOver after Playing, Menu after GameOver.
    return s;
}

const char* name(GameState s) {
    // TODO: return "Menu" / "Playing" / "GameOver".
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
