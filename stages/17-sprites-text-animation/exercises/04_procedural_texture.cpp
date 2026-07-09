// Exercise 04 — build a texture in code with sf::Image  (SFML)
//
// You don't need a .png on disk to get a texture. sf::Image is a CPU-side pixel
// buffer you can write pixel-by-pixel, then upload to an sf::Texture. This is
// how this whole course avoids shipping binary art assets.
//
// Build (needs a display to run):
//   cmake -S . -B build && cmake --build build --target 04_procedural_texture
//   ./build/04_procedural_texture
//
// TODO:
//   In make_checkerboard():
//     1. image.create(size, size, sf::Color::Black)
//     2. loop over x,y; when ((x/tile + y/tile) % 2 == 0) setPixel to `light`
//     3. return the image
//   Then load it into `texture` in main() with texture.loadFromImage(...).

#include <SFML/Graphics.hpp>

sf::Image make_checkerboard(unsigned size, unsigned tile, sf::Color light) {
    sf::Image image;
    // TODO: create the image and paint a checkerboard, then return it.
    (void)size;
    (void)tile;
    (void)light;
    return image;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(256, 256), "Ex04 - procedural texture");

    sf::Texture texture;
    // TODO: load the checkerboard image into the texture (check the bool result).
    sf::Image board = make_checkerboard(64, 8, sf::Color(200, 200, 60));
    (void)board;  // TODO: texture.loadFromImage(board);

    sf::Sprite sprite(texture);
    sprite.setScale(4.0f, 4.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}
