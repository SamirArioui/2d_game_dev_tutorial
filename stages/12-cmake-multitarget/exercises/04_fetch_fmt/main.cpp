#include <fmt/format.h>  // comes from the fetched fmt library

int main() {
    // If this links and runs, FetchContent successfully downloaded and built fmt.
    fmt::print("{:*^32}\n", " fetched fmt works ");
    fmt::print("fmt::format returns a string: {}\n", fmt::format("{} + {} = {}", 2, 3, 2 + 3));
    return 0;
}
