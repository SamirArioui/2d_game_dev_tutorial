// Exercise 03 — the texture-lifetime bug  (SFML; needs a display to SEE it)
//
// This is the headline pitfall of the stage. An sf::Sprite does NOT copy or own
// its texture — it holds a POINTER to one (tie this back to dangling pointers
// from stage 06). So the texture must outlive every sprite that uses it.
//
// The make_player_sprite() below is WRONG: `texture` is a local that is
// destroyed the instant the function returns, leaving the returned sprite
// pointing at freed memory. It usually shows up as a white square or garbage.
//
// Build (needs SFML via the exercises CMakeLists; needs a GPU/display to run):
//   cmake -S . -B build && cmake --build build --target 03_texture_lifetime
//   ./build/03_texture_lifetime
//
// TODO:
//   Fix the dangling texture. The clean fix: make the CALLER own the texture
//   and pass it in by reference, so it lives as long as main() needs it.
//     - change the signature to: sf::Sprite make_player_sprite(sf::Texture& texture)
//     - load into that reference instead of a local
//     - in main(), declare `sf::Texture texture;` BEFORE the sprite and pass it

#include <SFML/Graphics.hpp>

// BUG: returns a sprite bound to a texture that dies at the end of this scope.
sf::Sprite make_player_sprite() {
    sf::Texture texture;  // <-- local: destroyed when the function returns
    sf::Image img;
    img.create(16, 16, sf::Color::Green);
    (void)texture.loadFromImage(img);
    return sf::Sprite(texture);  // the returned sprite now dangles
}

int main() {
    sf::RenderWindow window(sf::VideoMode(320, 240), "Ex03 - texture lifetime");

    // TODO: fix the call so the texture outlives the sprite.
    sf::Sprite sprite = make_player_sprite();
    sprite.setScale(8.0f, 8.0f);
    sprite.setPosition(80.0f, 40.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(sprite);  // white square if the texture dangled
        window.display();
    }
    return 0;
}
