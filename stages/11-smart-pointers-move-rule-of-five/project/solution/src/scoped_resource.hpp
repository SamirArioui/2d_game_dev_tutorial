// ============================================================================
// Stage 11 mini-project — ScopedResource (header) — REFERENCE SOLUTION
// ============================================================================
//
// A textbook RULE-OF-5 class: it owns a raw resource (a heap-allocated handle id)
// and therefore must define all five special members. Every one of them LOGS, so
// running the demo makes the otherwise-invisible copy/move/destroy machinery
// visible — which is the whole teaching point.
//
// In real code you'd rarely write this by hand; you'd hold a std::unique_ptr and
// get the rule of 0 for free. ScopedResource exists so you can watch what
// unique_ptr does for you under the hood.
// ============================================================================
#pragma once

#include <string>

class ScopedResource {
public:
    // Acquire: allocate the underlying handle.
    ScopedResource(std::string label, int handle_id);

    // (1) Destructor — release the handle.
    ~ScopedResource();

    // (2) Copy constructor / (3) copy assignment — DEEP copy (own buffer).
    ScopedResource(const ScopedResource& other);
    ScopedResource& operator=(const ScopedResource& other);

    // (4) Move constructor / (5) move assignment — steal the buffer, cheap.
    ScopedResource(ScopedResource&& other) noexcept;
    ScopedResource& operator=(ScopedResource&& other) noexcept;

    int handle() const { return handle_ ? *handle_ : -1; }
    bool empty() const { return handle_ == nullptr; }

private:
    void log(const char* op) const;

    std::string label_;
    int* handle_;  // the raw owned resource — the reason we need the rule of 5
};
