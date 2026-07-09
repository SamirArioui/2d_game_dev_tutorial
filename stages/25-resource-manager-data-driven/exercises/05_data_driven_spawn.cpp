// Exercise 05 — Data + cache, together (hard)
//
// The payoff of the whole stage: build the game world from DATA, pulling every texture THROUGH the
// caching manager so shared textures load only once. Here five entities reference just two
// distinct textures — so exactly two loads should happen.
//
// The ResourceManager and the parsed specs are provided. Your job is the spawn loop.
//
// TODO: for each spec, get its texture through `textures` (so duplicates are cached) and remember
//       which texture each spawned entity uses (push the returned pointer into `used`).
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_data_driven_spawn.cpp -o ex && ./ex

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            return *it->second;
        }
        auto resource = std::make_unique<T>();
        resource->loadFromFile(path);
        T& ref = *resource;
        cache_.emplace(path, std::move(resource));
        return ref;
    }
    std::size_t size() const { return cache_.size(); }

private:
    std::unordered_map<std::string, std::unique_ptr<T>> cache_;
};

struct EntitySpec {
    std::string texture;
    float x{0.0f};
    float y{0.0f};
};

int main() {
    // Five entities, but only two distinct textures.
    const std::vector<EntitySpec> specs = {
        {"grass.png", 0, 0},   {"grass.png", 32, 0},  {"grass.png", 64, 0},
        {"wall.png", 0, 32},   {"grass.png", 96, 0},
    };

    ResourceManager<FakeTexture> textures;
    std::vector<FakeTexture*> used;  // which texture each spawned entity ended up using

    // TODO: loop over `specs`; for each, get the texture through `textures` and push its address
    //       into `used`.

    assert(used.size() == 5);            // five entities spawned
    assert(FakeTexture::loads == 2);     // but only two actual loads (grass + wall)
    assert(textures.size() == 2);        // two unique textures cached
    assert(used[0] == used[1]);          // the grass entities SHARE one texture instance

    std::cout << "spawned " << used.size() << " entities from " << textures.size()
              << " textures\n";
    return 0;
}
