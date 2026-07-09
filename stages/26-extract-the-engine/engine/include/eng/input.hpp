#pragma once

// ============================================================================
// eng::InputMap — map ACTION NAMES to physical keys.
//
// Game code should never say `if (Keyboard::isKeyPressed(W))`. It should ask
// "is the 'move_up' action down?". That indirection means keys are rebindable,
// the same action can have several keys, and the gameplay code reads in terms
// of intent, not hardware. This is the "input action mapping" idea.
//
// Header-only, but it does touch SFML (it queries the real keyboard), so it is
// only pulled in by code that already links SFML.
// ============================================================================

#include <string>
#include <unordered_map>

#include <SFML/Window/Keyboard.hpp>

namespace eng {

class InputMap {
public:
    // Bind (or rebind) an action to a key. Last binding for an action wins.
    void bind(const std::string& action, sf::Keyboard::Key key) { bindings_[action] = key; }

    // Is the key bound to `action` currently held down? Unknown actions are
    // simply "not down" rather than an error.
    bool is_down(const std::string& action) const {
        auto it = bindings_.find(action);
        if (it == bindings_.end()) {
            return false;
        }
        return sf::Keyboard::isKeyPressed(it->second);
    }

    // Convenience: +1 if `positive` is down, -1 if `negative` is down, 0 if both
    // or neither. Perfect for an axis like left/right or up/down movement.
    float axis(const std::string& negative, const std::string& positive) const {
        const float value = (is_down(positive) ? 1.0f : 0.0f) - (is_down(negative) ? 1.0f : 0.0f);
        return value;
    }

private:
    std::unordered_map<std::string, sf::Keyboard::Key> bindings_;
};

}  // namespace eng
