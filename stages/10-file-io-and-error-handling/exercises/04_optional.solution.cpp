// Exercise 04 — std::optional and the three error-handling styles (solution)
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

std::optional<int> parse_int(const std::string& s) {
    std::stringstream ss(s);
    int value = 0;
    ss >> value;
    if (!ss) {
        return std::nullopt;  // no number at all -> empty
    }

    char leftover = 0;
    if (ss >> leftover) {
        return std::nullopt;  // extra junk after the number -> not a clean int
    }

    return value;  // implicitly wrapped into std::optional<int>
}

int main() {
    std::optional<int> ok = parse_int("42");
    if (ok.has_value()) {
        std::cout << "parse '42'  -> " << *ok << "\n";
    }

    std::optional<int> bad = parse_int("oops");
    std::cout << "parse 'oops' -> " << (bad.has_value() ? "has value" : "(none)")
              << ", using fallback " << bad.value_or(0) << "\n";

    std::optional<int> spaced = parse_int("  7 ");
    std::cout << "parse '  7 ' -> " << spaced.value_or(-1) << "\n";

    return 0;
}
