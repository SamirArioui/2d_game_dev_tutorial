#include "eng/font.hpp"

#include <array>

namespace eng {

bool load_system_font(sf::Font& out, const std::string& preferred) {
    if (!preferred.empty() && out.loadFromFile(preferred)) {
        return true;
    }

    // A short list of fonts that ship with each major OS. The first that loads
    // wins. Extend this if your platform isn't covered.
    static const std::array<const char*, 6> candidates = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",       // macOS
        "/System/Library/Fonts/SFNS.ttf",                     // macOS (San Francisco)
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",    // Debian/Ubuntu
        "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",  // Fedora
        "/usr/share/fonts/TTF/DejaVuSans.ttf",                // Arch
        "C:/Windows/Fonts/arial.ttf",                         // Windows
    };

    for (const char* path : candidates) {
        if (out.loadFromFile(path)) {
            return true;
        }
    }
    return false;  // no font found — caller should skip text rendering
}

}  // namespace eng
