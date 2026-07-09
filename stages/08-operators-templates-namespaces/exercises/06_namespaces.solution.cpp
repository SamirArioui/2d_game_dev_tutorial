// Exercise 06 — Namespaces (solution)
#include <iostream>

namespace audio {
int volume() { return 11; }
}  // namespace audio

namespace video {
int volume() { return 100; }   // same name, different namespace — no clash
}  // namespace video

int main() {
    // Fully qualified calls: unambiguous about which volume() you mean.
    std::cout << "audio::volume() = " << audio::volume() << "\n";
    std::cout << "video::volume() = " << video::volume() << "\n";

    // A using-declaration brings ONE name into scope; now bare `volume()` means audio::volume.
    using audio::volume;
    std::cout << "volume() (audio) = " << volume() << "\n";

    return 0;
}
