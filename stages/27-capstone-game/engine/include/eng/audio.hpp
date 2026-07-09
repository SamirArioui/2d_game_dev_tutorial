#pragma once

// ============================================================================
// eng audio — generate simple tones IN CODE (no .wav/.ogg files shipped).
//
// SFML plays sound from an sf::SoundBuffer (the raw samples). Instead of loading
// a file we synthesize the samples ourselves — a sine wave is just
// amplitude * sin(2*pi*freq*t). This gives Pong its "beep" with zero assets.
//
// LIFETIME WARNING (the classic SFML footgun, met in stage 18): an sf::Sound
// keeps only a POINTER to its buffer. The buffer must outlive the sound, so you
// store the returned SoundBuffer somewhere durable (a member) — never a local
// that is destroyed before the sound finishes playing.
// ============================================================================

#include <SFML/Audio/SoundBuffer.hpp>

namespace eng {

// Synthesize a `seconds`-long sine tone at `frequency_hz`. `volume` is 0..1.
// Returns a ready-to-play SoundBuffer (copyable/movable value).
sf::SoundBuffer make_tone(float frequency_hz, float seconds, unsigned sample_rate = 44100,
                          double volume = 0.35);

}  // namespace eng
