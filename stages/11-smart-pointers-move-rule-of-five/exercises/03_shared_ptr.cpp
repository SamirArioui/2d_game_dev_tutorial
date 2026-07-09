// Exercise 03 — std::shared_ptr and reference counting (medium)
//
// Sometimes ONE object is legitimately used by several owners and should live until
// the last of them is done — e.g. a texture shared by many sprites. That's
// std::shared_ptr<T>: it keeps a reference COUNT, and deletes the object only when
// the count hits zero. This is exactly Python's model (CPython reference-counts
// every object) — the difference is that in C++ you opt IN to it explicitly, and
// pay for it only where you need shared ownership.
//
//   auto a = std::make_shared<Texture>("hero.png");   // use_count() == 1
//   auto b = a;                                        // COPY is fine -> count 2
//   // object freed when both a and b are gone
//
// .use_count() reports the current number of owners. Prefer unique_ptr by default;
// reach for shared_ptr only when ownership is genuinely shared.
//
// Fill in the TODOs. Expected output:
//   loaded texture hero.png
//   owners after make_shared: 1
//   owners after copy: 2
//   owners inside block: 3
//   owners after block: 2
//   ~Texture hero.png
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 03_shared_ptr.cpp -o ex && ./ex

#include <iostream>
#include <memory>  // std::shared_ptr, std::make_shared
#include <string>

struct Texture {
    std::string path;
    explicit Texture(std::string p) : path(std::move(p)) {
        std::cout << "loaded texture " << path << "\n";
    }
    ~Texture() { std::cout << "~Texture " << path << "\n"; }
};

int main() {
    std::shared_ptr<Texture> a = std::make_shared<Texture>("hero.png");
    std::cout << "owners after make_shared: " << a.use_count() << "\n";

    // TODO 1: make a COPY `b` of `a` (just `auto b = a;`). Print
    //         "owners after copy: " << a.use_count()   (expect 2).

    {
        // TODO 2: make another copy `c` inside this block. Print
        //         "owners inside block: " << a.use_count()  (expect 3).
    }
    // `c` was destroyed leaving the block, dropping the count back to 2.

    // TODO 3: print "owners after block: " << a.use_count()  (expect 2).

    return 0;  // a and b go away here; count -> 0; the Texture is freed
}
