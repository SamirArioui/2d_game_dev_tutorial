#pragma once

// ============================================================================
// eng font loading — find *a* usable TrueType font, wherever we're running.
//
// The course ships no font files, so text rendering relies on a font already on
// the machine. Paths differ per OS, and a headless CI box may have none at all.
// This helper tries a list of common system fonts (and an optional override) and
// reports whether it succeeded, so callers can DEGRADE GRACEFULLY — draw the
// game without a score readout rather than crash — instead of assuming a font.
// ============================================================================

#include <string>

#include <SFML/Graphics/Font.hpp>

namespace eng {

// Try to load a font into `out`. If `preferred` is non-empty it is tried first;
// then a built-in list of common macOS/Linux/Windows system fonts. Returns true
// if any loaded. On false, `out` is unusable and text should be skipped.
bool load_system_font(sf::Font& out, const std::string& preferred = "");

}  // namespace eng
