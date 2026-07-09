// Exercise 03 — solution  (SFML)
//
// The fix: the CALLER owns the texture. We pass it in by reference and load
// into it, so it lives exactly as long as main() keeps it around. The sprite,
// declared after and destroyed before the texture, can never outlive it.

#include <SFML/Graphics.hpp>

// Takes the texture by reference — no local texture to die. The caller decides
// how long it lives (RAII: it dies when the owning scope ends, not here).
sf::Sprite make_player_sprite(sf::Texture& texture) {
    sf::Image img;
    img.create(16, 16, sf::Color::Green);
    (void)texture.loadFromImage(img);
    return sf::Sprite(texture);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(320, 240), "Ex03 - texture lifetime");

    // Declared BEFORE the sprite, so destroyed AFTER it. The sprite's pointer
    // stays valid for the whole program.
    sf::Texture texture;
    sf::Sprite sprite = make_player_sprite(texture);
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
        window.draw(sprite);  // now shows the solid green square, no dangling
        window.display();
    }
    return 0;
}
