// Exercise 03 — std::shared_ptr and reference counting (solution)
#include <iostream>
#include <memory>
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

    auto b = a;  // copying a shared_ptr bumps the reference count
    std::cout << "owners after copy: " << a.use_count() << "\n";

    {
        auto c = a;  // third owner, scoped to this block
        std::cout << "owners inside block: " << a.use_count() << "\n";
    }  // c destroyed here -> count drops

    std::cout << "owners after block: " << a.use_count() << "\n";

    return 0;  // a and b gone -> count 0 -> Texture freed
}
