// Exercise 04 — The rule of 0/3/5, made observable (medium–hard)
//
// A class that OWNS a raw resource (here a heap `int*`) must control how it is
// copied, moved, and destroyed — otherwise the compiler's defaults will copy the
// POINTER (two objects, one buffer) and double-free it. The rules:
//
//   Rule of 0: own nothing raw. Hold members that manage themselves (std::string,
//              std::vector, std::unique_ptr) and write NONE of the special members.
//              (This is what you should do 99% of the time — see the mini-project.)
//   Rule of 3: if you write ONE of { destructor, copy ctor, copy assignment },
//              write all THREE (pre-C++11).
//   Rule of 5: C++11 adds two more — the move ctor and move assignment — for cheap
//              ownership transfer. If you write any special member, consider all 5.
//
// This class logs every special member so you can SEE which one fires for each
// line of main(). The destructor + the two COPY operations are already written for
// you (that's the rule of 3). YOUR JOB: add the two MOVE operations to reach the
// rule of 5, and watch "copy" turn into "move" for the std::move lines.
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 04_rule_of_five.cpp -o ex && ./ex

#include <iostream>
#include <string>
#include <utility>  // std::move

class Resource {
public:
    // Normal constructor: acquire the resource (allocate the heap int).
    explicit Resource(int value) : id_(next_id_++), data_(new int(value)) { log("ctor"); }

    // (1) Destructor — release the resource. delete on nullptr is safe.
    ~Resource() {
        log(data_ ? "dtor" : "dtor (moved-from)");
        delete data_;
    }

    // (2) Copy constructor — DEEP copy: allocate our own buffer.
    Resource(const Resource& other) : id_(next_id_++), data_(new int(*other.data_)) {
        log("copy ctor");
    }

    // (3) Copy assignment — release what we hold, then deep-copy the other.
    Resource& operator=(const Resource& other) {
        if (this != &other) {  // guard against  x = x;
            delete data_;
            data_ = new int(*other.data_);
        }
        log("copy assign");
        return *this;
    }

    // TODO (4) Move constructor:  Resource(Resource&& other) noexcept
    //   - steal other.data_ into this->data_
    //   - set other.data_ = nullptr so its destructor frees nothing
    //   - init id_(next_id_++) and call log("move ctor")
    //   Mark it `noexcept` — containers rely on that to move instead of copy.

    // TODO (5) Move assignment:  Resource& operator=(Resource&& other) noexcept
    //   - guard against self-assignment (this != &other)
    //   - delete our current data_, steal other.data_, null out other.data_
    //   - log("move assign"); return *this;

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
