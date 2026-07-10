#pragma once

// ===========================================================================
// Input ACTION MAPPING — the difference between a hobby project and something maintainable.
//
// The naive approach scatters `if (key == W)` all over gameplay code. Then a player wants to
// rebind W, or you add a gamepad, and you're hunting through every file. Instead we introduce a
// layer of indirection: raw keys map to SEMANTIC ACTIONS (MoveUp, Confirm, ...), and gameplay
// only ever talks about actions. Rebinding is now a one-line change to the map.
//
// Python comparison: a plain dict `{key_code: action}` you look up per key press.
//
// We key the map on plain `int` key CODES (not sf::Keyboard::Key) so this class is SFML-free and
// unit-testable. The app feeds it `static_cast<int>(event.key.code)`.
// ===========================================================================

#include <cstddef>
#include <optional>
#include <unordered_map>

namespace input {

enum class Action { MoveUp, MoveDown, Confirm, Cancel, Quit };

class InputMap {
public:
    // Bind (or rebind) a key code to an action. Binding a key that's already bound replaces it.
    void bind(int key_code, Action action) { bindings_[key_code] = action; }
    void unbind(int key_code) { bindings_.erase(key_code); }

    // The action a key triggers, or std::nullopt if the key is unbound. std::optional is C++'s
    // "maybe a value" (Python's `None`-or-value), introduced back in stage 10.
    std::optional<Action> action_for(int key_code) const {
        auto it = bindings_.find(key_code);
        if (it == bindings_.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    bool is_bound(int key_code) const { return bindings_.find(key_code) != bindings_.end(); }
    std::size_t size() const { return bindings_.size(); }

private:
    std::unordered_map<int, Action> bindings_;
};

}  // namespace input
