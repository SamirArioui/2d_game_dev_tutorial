// Exercise 03 — Input action mapping (solution)
#include <cassert>
#include <iostream>
#include <optional>
#include <unordered_map>

enum class Action { MoveUp, MoveDown, Confirm, Cancel };

constexpr int KeyW = 1;
constexpr int KeyEnter = 2;
constexpr int KeyEsc = 3;

class InputMap {
public:
    void bind(int key_code, Action action) { bindings_[key_code] = action; }

    std::optional<Action> action_for(int key_code) const {
        auto it = bindings_.find(key_code);
        if (it == bindings_.end()) {
            return std::nullopt;
        }
        return it->second;
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
    assert(!map.action_for(KeyEsc).has_value());

    map.bind(KeyW, Action::Cancel);
    assert(map.action_for(KeyW) == Action::Cancel);

    std::cout << "input mapping works\n";
    return 0;
}
