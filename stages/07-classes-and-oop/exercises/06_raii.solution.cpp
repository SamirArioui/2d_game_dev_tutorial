// Exercise 06 — RAII: destructors that clean up (solution)
#include <iostream>

class LiveCount {
public:
    explicit LiveCount(int& count) : count_(count) {
        ++count_;
        std::cout << "spawned, alive = " << count_ << "\n";
    }

    ~LiveCount() {
        --count_;
        std::cout << "died,    alive = " << count_ << "\n";
    }

private:
    int& count_;   // reference member: bound once in the init list, never reassigned
};

int main() {
    int alive = 0;

    LiveCount a{alive};                 // alive -> 1
    {
        LiveCount b{alive};             // alive -> 2
        LiveCount c{alive};             // alive -> 3
        // b and c leave scope HERE, in REVERSE order (c then b) -> alive back to 1
    }
    std::cout << "back in main, alive = " << alive << "\n";
    // a is destroyed as main returns -> alive back to 0

    return 0;
}
