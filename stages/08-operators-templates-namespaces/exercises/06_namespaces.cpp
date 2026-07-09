// Exercise 06 — Namespaces (medium)
//
// A namespace groups related names and prevents clashes. Two libraries can each define a
// function called `volume()` as long as they live in different namespaces: audio::volume() vs
// video::volume(). This is C++'s system for "who owns this name" — the same role a Python module
// plays (it does NOT control compilation the way a module controls import, but it namespaces
// names the same way).
//
// PYTHON NOTE:
//   import audio; audio.volume()      # module-qualified
//   from audio import volume          # bring one name in
//   ~ C++:  audio::volume();          // qualified
//           using audio::volume;      // bring one name in
//           using namespace audio;    // bring ALL of audio in (avoid this in headers!)
//
// TODO:
//   1. Put a volume() returning 11 in namespace `audio`, and a volume() returning 100 in
//      namespace `video`.
//   2. In main, call both by their qualified names.
//   3. Add a `using audio::volume;` declaration, then call volume() unqualified.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 06_namespaces.cpp -o ex && ./ex

#include <iostream>

namespace audio {
// TODO: int volume() { return 11; }
}  // namespace audio

namespace video {
// TODO: int volume() { return 100; }
}  // namespace video

int main() {
    // TODO: print audio::volume() and video::volume().
    // TODO: `using audio::volume;` then call volume() unqualified.
    return 0;
}
