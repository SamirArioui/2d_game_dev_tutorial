// Exercise 03 — solution
#include <catch2/catch_test_macros.hpp>

#include <string>
#include <unordered_map>

class Bag {
public:
    void add(const std::string& name, int n) { counts_[name] += n; }
    int count(const std::string& name) const {
        auto it = counts_.find(name);
        return it == counts_.end() ? 0 : it->second;
    }
    int distinct() const { return static_cast<int>(counts_.size()); }

private:
    std::unordered_map<std::string, int> counts_;
};

struct BagFixture {
    Bag bag;
    BagFixture() {
        bag.add("apple", 3);
        bag.add("key", 1);
    }
};

// TEST_CASE_METHOD runs inside a fresh BagFixture, so `bag` is a member here.
TEST_CASE_METHOD(BagFixture, "starts with three apples", "[ex03]") {
    REQUIRE(bag.count("apple") == 3);
    REQUIRE(bag.distinct() == 2);
}

TEST_CASE_METHOD(BagFixture, "adding stacks the count", "[ex03]") {
    bag.add("apple", 2);
    REQUIRE(bag.count("apple") == 5);
    // Still two DISTINCT items — we stacked, we didn't add a new kind. And this
    // test's fixture is independent of the one above.
    REQUIRE(bag.distinct() == 2);
}
