// Exercise 02 — A caching ResourceManager (solution)
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

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
            return *it->second;  // hit
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

int main() {
    ResourceManager<FakeTexture> textures;

    FakeTexture& a = textures.get("grass.png");
    FakeTexture& b = textures.get("grass.png");
    assert(&a == &b);
    assert(FakeTexture::loads == 1);

    textures.get("wall.png");
    assert(FakeTexture::loads == 2);
    assert(textures.size() == 2);

    std::cout << "resource cache works\n";
    return 0;
}
