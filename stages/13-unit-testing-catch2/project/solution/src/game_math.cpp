#include "game/game_math.hpp"

#include <cmath>

namespace game {

float clamp(float value, float lo, float hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

float lerp(float a, float b, float t) {
    // a + (b - a) * t  ->  a at t=0, b at t=1.
    return a + (b - a) * t;
}

float distance(float x1, float y1, float x2, float y2) {
    const float dx = x2 - x1;
    const float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

}  // namespace game
