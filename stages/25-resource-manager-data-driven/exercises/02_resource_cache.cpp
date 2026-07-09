// Exercise 02 — A caching ResourceManager (medium)
//
// Generalize the memoization into a reusable templated cache: load a resource of type T on first
// use, then hand back the SAME instance every time. This is the shared ResourceManager contract.
//
// We store unique_ptr<T> (not T) so the object lives at a stable heap address — the reference we
// return stays valid even if the map rehashes, and we never copy a big resource.
//
// TODO: implement get():
//   - on a cache HIT, return the stored instance;
//   - on a MISS, make_unique<T>, call loadFromFile(path), store it, and return it.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_resource_cache.cpp -o ex && ./ex

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// A fake resource that satisfies the loadFromFile contract and counts loads.
struct FakeTexture {
    static int loads;
    bool loadFromFile(const std::string&) {
        ++loads;
        return true;
    }
};
int FakeTexture::loads = 0;

template <class T>
class ResourceManager {
public:
    T& get(const std::string& path) {
        // TODO: implement load-once caching as described above.
        (void)path;
        static T dummy;  // replace this
        return dummy;
    }

    std::size_t size() const { return cache_.size(); }

private:
    std::unordered_map<std::string, std::unique_ptr<T>> cache_;
};

int main() {
    ResourceManager<FakeTexture> textures;

    FakeTexture& a = textures.get("grass.png");
    FakeTexture& b = textures.get("grass.png");
    assert(&a == &b);                 // same instance returned
    assert(FakeTexture::loads == 1);  // loaded exactly once

    textures.get("wall.png");
    assert(FakeTexture::loads == 2);
    assert(textures.size() == 2);

    std::cout << "resource cache works\n";
    return 0;
}
