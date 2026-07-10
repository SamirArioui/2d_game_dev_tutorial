// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
#include "battle_log.hpp"

#include <iostream>
#include <utility>   // std::move

BattleLog::BattleLog(std::string title) : title_(std::move(title)) {
    // ACQUIRE: announce that the log is open. In a real engine this might open a file (stage 10)
    // or a network connection — the pattern is identical.
    std::cout << "=== " << title_ << " ===\n";
}

BattleLog::~BattleLog() {
    // RELEASE: this is the "real cleanup" that proves RAII is more than a slogan. It runs
    // automatically when the BattleLog goes out of scope — nothing in main() calls it by hand.
    std::cout << "=== " << title_ << " closed (" << entries_ << " entries logged) ===\n";
}

void BattleLog::record(const std::string& line) {
    ++entries_;
    std::cout << "  " << line << "\n";
}
