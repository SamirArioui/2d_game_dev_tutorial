#pragma once

// ===========================================================================
// Input ACTION MAPPING — the difference between a hobby project and something maintainable.
//                                                                              (YOUR TASK)
// This header is the STARTER: the enum, the class and the bindings_ storage are given, but the
// method BODIES are stubbed with TODOs. Implementing them is part of this stage's mini-project;
// the project still compiles but tests/test_input_map.cpp FAILS until you fill them in. A complete
// reference is in ../../solution/include/input/action.hpp.
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
    void bind(int key_code, Action action) {
        // TODO(stage 24): store the mapping key_code -> action in bindings_ (operator[] replaces
        // any existing binding for that key, which is exactly the rebind behaviour we want).
    }
    void unbind(int key_code) {
        // TODO(stage 24): remove key_code from bindings_ (a no-op if it isn't bound).
    }

    // The action a key triggers, or std::nullopt if the key is unbound. std::optional is C++'s
    // "maybe a value" (Python's `None`-or-value), introduced back in stage 10.
    std::optional<Action> action_for(int key_code) const {
        // TODO(stage 24): look key_code up in bindings_; return its Action, or std::nullopt if the
        // key is unbound.
        return std::nullopt;  // placeholder
    }

    bool is_bound(int key_code) const {
        // TODO(stage 24): return whether key_code currently has a binding.
        return false;  // placeholder
    }
    std::size_t size() const {
        // TODO(stage 24): return how many bindings exist.
        return 0;  // placeholder
    }

private:
    std::unordered_map<int, Action> bindings_;
};

}  // namespace input
