#include "eng/audio.hpp"

#include <cmath>
#include <cstdint>
#include <vector>

namespace eng {

sf::SoundBuffer make_tone(float frequency_hz, float seconds, unsigned sample_rate, double volume) {
    const std::size_t sample_count =
        static_cast<std::size_t>(static_cast<double>(sample_rate) * seconds);

    std::vector<sf::Int16> samples(sample_count);
    const double two_pi_f = 2.0 * 3.14159265358979323846 * frequency_hz;
    const double amplitude = volume * 32767.0;  // sf::Int16 range is [-32768, 32767]

    for (std::size_t i = 0; i < sample_count; ++i) {
        const double t = static_cast<double>(i) / static_cast<double>(sample_rate);

        // A short linear fade-out avoids the "click" you get when a waveform is
        // cut off mid-cycle (the speaker jumping instantly to zero).
        const double fade = 1.0 - (static_cast<double>(i) / static_cast<double>(sample_count));

        samples[i] = static_cast<sf::Int16>(amplitude * fade * std::sin(two_pi_f * t));
    }

    sf::SoundBuffer buffer;
    // 1 channel = mono. loadFromSamples copies the samples INTO the buffer, so
    // our local vector can safely go away when this function returns.
    buffer.loadFromSamples(samples.data(), sample_count, 1, sample_rate);
    return buffer;
}

}  // namespace eng
