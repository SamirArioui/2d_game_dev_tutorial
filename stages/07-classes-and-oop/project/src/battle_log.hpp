#pragma once

#include <string>

// ---------------------------------------------------------------------------
// BattleLog — a demonstration of RAII.
//
// RAII = "Resource Acquisition Is Initialization". The idea: tie a resource's lifetime to an
// OBJECT's lifetime. The constructor ACQUIRES the resource (here: opens the log, prints a
// banner); the destructor RELEASES it (here: prints a closing summary). Because C++ destructors
// run deterministically the instant the object leaves scope — even on an early return or an
// exception — the cleanup is guaranteed. You can't forget it.
//
// PYTHON NOTE: the closest analogue is a `with` block / context manager (__enter__/__exit__).
// But Python's __del__ runs "eventually", whenever the garbage collector gets to it; a C++
// destructor runs at a precise, predictable moment (scope exit, reverse construction order).
// ---------------------------------------------------------------------------
class BattleLog {
public:
    explicit BattleLog(std::string title);
    ~BattleLog();   // does REAL work: prints the closing line and how many entries were logged

    // Record one line of the battle and count it. `const std::string&`: we only read the text,
    // so take it by const reference (no copy).
    void record(const std::string& line);

    int entries() const { return entries_; }

private:
    std::string title_;
    int entries_ = 0;   // in-class initialiser: entries_ starts at 0 even before the ctor body
};
