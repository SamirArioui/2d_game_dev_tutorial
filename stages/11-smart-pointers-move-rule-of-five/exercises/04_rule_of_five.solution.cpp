// Exercise 04 — The rule of 0/3/5, made observable (solution)
#include <iostream>
#include <string>
#include <utility>

class Resource {
public:
    explicit Resource(int value) : id_(next_id_++), data_(new int(value)) { log("ctor"); }

    ~Resource() {
        log(data_ ? "dtor" : "dtor (moved-from)");
        delete data_;
    }

    Resource(const Resource& other) : id_(next_id_++), data_(new int(*other.data_)) {
        log("copy ctor");
    }

    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete data_;
            data_ = new int(*other.data_);
        }
        log("copy assign");
        return *this;
    }

    // Move constructor: steal the buffer, leave `other` empty. noexcept matters:
    // std::vector will only move (not copy) its elements on reallocation if the
    // move constructor promises not to throw.
    Resource(Resource&& other) noexcept : id_(next_id_++), data_(other.data_) {
        other.data_ = nullptr;  // `other`'s destructor now frees nothing
        log("move ctor");
    }

    // Move assignment: release ours, steal theirs, null theirs.
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete data_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        log("move assign");
        return *this;
    }

    int value() const { return data_ ? *data_ : -1; }

private:
    void log(const char* op) const {
        std::cout << op << " #" << id_;
        if (data_) {
            std::cout << " (value=" << *data_ << ")\n";
        } else {
            std::cout << " (empty)\n";
        }
    }

    int id_;
    int* data_;
    static int next_id_;
};

int Resource::next_id_ = 1;

int main() {
    std::cout << "-- a: construct --\n";
    Resource a(10);

    std::cout << "-- b: copy-construct from a --\n";
    Resource b = a;

    std::cout << "-- c: move-construct from a (a becomes empty) --\n";
    Resource c = std::move(a);

    std::cout << "-- d: construct(40), then copy-assign d = b --\n";
    Resource d(40);
    d = b;

    std::cout << "-- e: construct(50), then move-assign e = std::move(c) --\n";
    Resource e(50);
    e = std::move(c);

    std::cout << "-- end of scope: destructors run in reverse (e,d,c,b,a) --\n";
    return 0;
}
