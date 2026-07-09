// Exercise 05 — solution  (SFML audio)
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    const unsigned sample_rate = 44100;
    const unsigned count = sample_rate / 10;  // 0.1 s
    std::vector<sf::Int16> samples(count);

    const float freq = 440.0f;
    constexpr float kTwoPi = 6.28318530718f;
    for (unsigned i = 0; i < count; ++i) {
        const float t = static_cast<float>(i) / sample_rate;
        const float fade = 1.0f - static_cast<float>(i) / count;  // avoid an end click
        const float value = std::sin(kTwoPi * freq * t) * 0.3f * fade;
        samples[i] = static_cast<sf::Int16>(value * 32767.0f);
    }

    sf::SoundBuffer buffer;  // declared FIRST -> destroyed LAST (outlives sound)
    if (!buffer.loadFromSamples(samples.data(), count, 1, sample_rate)) {
        std::cerr << "Failed to load samples\n";
        return 1;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);

    sf::RenderWindow window(sf::VideoMode(320, 120), "Ex05 - beep (press SPACE)");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                sound.play();
            }
        }
        window.clear(sf::Color(20, 20, 30));
        window.display();
    }
    return 0;
}
