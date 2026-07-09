// Exercise 03 — Input action mapping (medium)
//
// Decouple "which key" from "what it does". Gameplay asks for ACTIONS (MoveUp, Confirm); a map
// translates raw key codes into those actions, so rebinding is a one-line change.
//
// Python comparison: a dict {key_code: action} and a .get(key) that returns None when unbound.
//
// TODO: implement bind() and action_for() on the InputMap below.
//   - action_for returns the bound Action, or std::nullopt if the key isn't bound.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_input_mapping.cpp -o ex && ./ex

#include <cassert>
#include <iostream>
#include <optional>
#include <unordered_map>

enum class Action { MoveUp, MoveDown, Confirm, Cancel };

// Stand-in key codes (the real app would use static_cast<int>(sf::Keyboard::W), etc.).
constexpr int KeyW = 1;
constexpr int KeyEnter = 2;
constexpr int KeyEsc = 3;

class InputMap {
public:
    void bind(int key_code, Action action) {
        // TODO: store the mapping.
        (void)key_code;
        (void)action;
    }

    std::optional<Action> action_for(int key_code) const {
        // TODO: look up key_code; return the Action, or std::nullopt if it's unbound.
        (void)key_code;
        return std::nullopt;
    }

private:
    std::unordered_map<int, Action> bindings_;
};

int main() {
    InputMap map;
    map.bind(KeyW, Action::MoveUp);
    map.bind(KeyEnter, Action::Confirm);

    assert(map.action_for(KeyW) == Action::MoveUp);
    assert(map.action_for(KeyEnter) == Action::Confirm);
    assert(!map.action_for(KeyEsc).has_value());  // unbound -> nullopt

    // Rebind W to Cancel: gameplay code that asks "what does W do?" now gets Cancel, unchanged.
    map.bind(KeyW, Action::Cancel);
    assert(map.action_for(KeyW) == Action::Cancel);

    std::cout << "input mapping works\n";
    return 0;
}
