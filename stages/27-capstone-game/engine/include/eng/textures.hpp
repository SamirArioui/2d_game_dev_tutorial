#pragma once

// ============================================================================
// eng procedural textures — build sf::Texture objects IN CODE, no image files.
//
// The course ships no binary assets, so every sprite's pixels are generated at
// runtime from an sf::Image (a CPU-side pixel buffer) uploaded to an sf::Texture
// (GPU-side). This keeps the repo text-only and teaches how a texture is really
// just a grid of RGBA bytes.
// ============================================================================

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace eng {

// A flat rectangle of one colour — paddles, walls, HUD bars.
sf::Texture make_solid_texture(unsigned width, unsigned height, sf::Color color);

// A filled circle of `diameter` px on a transparent background — the ball, coins.
sf::Texture make_circle_texture(unsigned diameter, sf::Color color);

// A two-colour checkerboard — handy as a placeholder / floor pattern.
sf::Texture make_checker_texture(unsigned width, unsigned height, unsigned cell, sf::Color a,
                                 sf::Color b);

}  // namespace eng
