// ============================================================================
// Stage 11 mini-project — ScopedResource (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the five special members (plus the constructor)
// so ScopedResource correctly owns its raw handle and every operation LOGS — that
// logging is the whole point, making the copy/move/destroy machinery visible.
//
// The declarations are in scoped_resource.hpp; the log() helper below is provided
// for you — call it from each member. The starter already compiles and links (the
// bodies are placeholders), but the rule-of-5 log is empty until you fill it in.
//
// A complete reference is in ../solution/src/scoped_resource.cpp — try it first.
// ============================================================================
#include "scoped_resource.hpp"

#include <iostream>
#include <utility>  // std::move

ScopedResource::ScopedResource(std::string label, int handle_id)
    : label_(std::move(label)), handle_(nullptr) {
    // TODO(stage 11): ACQUIRE the resource — allocate handle_ = new int(handle_id) —
    // then call log("ctor") so the construction is visible.
}

ScopedResource::~ScopedResource() {
    // TODO(stage 11): log "dtor" (or "dtor (moved-from)" when handle_ is null), then
    // RELEASE the resource with `delete handle_` (delete on nullptr is safe).
}

ScopedResource::ScopedResource(const ScopedResource& other) : handle_(nullptr) {
    // TODO(stage 11): DEEP copy — copy other.label_ and allocate a fresh
    // handle_ = new int(*other.handle_); then log("copy ctor").
}

ScopedResource& ScopedResource::operator=(const ScopedResource& other) {
    // TODO(stage 11): guard self-assignment, delete the old handle_, deep-copy
    // other's handle_ and label_, then log("copy assign").
    return *this;
}

ScopedResource::ScopedResource(ScopedResource&& other) noexcept : handle_(nullptr) {
    // TODO(stage 11): STEAL other's buffer — take other.handle_ and other.label_,
    // set other.handle_ = nullptr so its dtor frees nothing; then log("move ctor").
}

ScopedResource& ScopedResource::operator=(ScopedResource&& other) noexcept {
    // TODO(stage 11): guard self-assignment, delete the old handle_, steal other's
    // handle_ (and null it out) and label_, then log("move assign").
    return *this;
}

void ScopedResource::log(const char* op) const {
    std::cout << "  [ScopedResource] " << op << " '" << label_ << "'";
    if (handle_) {
        std::cout << " (handle " << *handle_ << ")\n";
    } else {
        std::cout << " (empty)\n";
    }
}
