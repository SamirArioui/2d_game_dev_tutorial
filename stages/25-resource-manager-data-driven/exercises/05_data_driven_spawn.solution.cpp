// Exercise 05 — Data + cache, together (solution)
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
    const std::vector<EntitySpec> specs = {
        {"grass.png", 0, 0},   {"grass.png", 32, 0},  {"grass.png", 64, 0},
        {"wall.png", 0, 32},   {"grass.png", 96, 0},
    };

    ResourceManager<FakeTexture> textures;
    std::vector<FakeTexture*> used;

    for (const auto& spec : specs) {
        FakeTexture& tex = textures.get(spec.texture);  // cached: duplicates don't reload
        used.push_back(&tex);
    }

    assert(used.size() == 5);
    assert(FakeTexture::loads == 2);
    assert(textures.size() == 2);
    assert(used[0] == used[1]);

    std::cout << "spawned " << used.size() << " entities from " << textures.size()
              << " textures\n";
    return 0;
}
