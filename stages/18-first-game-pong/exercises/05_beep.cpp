// Exercise 05 — a beep from raw samples, and the buffer-lifetime rule  (SFML audio)
//
// You don't need a .wav to make a sound. A sound is just numbers: amplitude
// samples over time. We fill an array with a sine wave and hand it to an
// sf::SoundBuffer, then an sf::Sound plays that buffer.
//
// THE PITFALL (mirrors stage 17's texture rule): sf::Sound stores a POINTER to
// its sf::SoundBuffer; it does not copy the samples. The buffer MUST outlive
// the sound. Declare the buffer first so it is destroyed last.
//
// Build (needs audio output to hear it):
//   cmake -S . -B build && cmake --build build --target 05_beep
//   ./build/05_beep       # press SPACE to beep; close the window to quit
//
// TODO:
//   1. Fill `samples` with a sine wave: samples[i] = sin(2*pi*freq*t) * amp,
//      scaled to Int16 range (multiply by 32767). t = i / sampleRate.
//   2. Load them: buffer.loadFromSamples(samples.data(), count, 1, sampleRate).

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>
#include <cstdint>
#include <vector>

int main() {
    const unsigned sample_rate = 44100;
    const unsigned count = sample_rate / 10;  // 0.1 seconds
    std::vector<sf::Int16> samples(count);

    const float freq = 440.0f;       // A4
    constexpr float kTwoPi = 6.28318530718f;
    for (unsigned i = 0; i < count; ++i) {
        // TODO: compute t and write samples[i] as a scaled sine value.
        (void)freq;
        (void)kTwoPi;
        samples[i] = 0;
    }

    // Buffer FIRST (outlives the sound), sound SECOND.
    sf::SoundBuffer buffer;
    // TODO: buffer.loadFromSamples(samples.data(), count, 1, sample_rate);
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
