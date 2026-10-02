// ============================================================================
// Stage 11 mini-project — ScopedResource (implementation) — REFERENCE SOLUTION
// ============================================================================
#include "scoped_resource.hpp"

#include <iostream>
#include <utility>  // std::move

ScopedResource::ScopedResource(std::string label, int handle_id)
    : label_(std::move(label)), handle_(new int(handle_id)) {
    log("ctor");
}

ScopedResource::~ScopedResource() {
    log(handle_ ? "dtor" : "dtor (moved-from)");
    delete handle_;  // delete on nullptr is safe
}

// Deep copy. `other` may be a moved-from (empty) object, so copy "empty" as empty
// instead of dereferencing a null handle.
ScopedResource::ScopedResource(const ScopedResource& other)
    : label_(other.label_), handle_(other.handle_ ? new int(*other.handle_) : nullptr) {
    log("copy ctor");
}

// Allocate FIRST, then release the old handle. If `new` throws, *this is untouched
// (no dangling handle_ for the destructor to double-free). The same order also
// makes self-assignment (x = x) safe without a special check.
ScopedResource& ScopedResource::operator=(const ScopedResource& other) {
    int* fresh = other.handle_ ? new int(*other.handle_) : nullptr;
    label_ = other.label_;  // std::string assignment is self-assignment safe
    delete handle_;
    handle_ = fresh;
    log("copy assign");
    return *this;
}

ScopedResource::ScopedResource(ScopedResource&& other) noexcept
    : label_(std::move(other.label_)), handle_(other.handle_) {  // steal
    other.handle_ = nullptr;  // leave `other` empty; its dtor frees nothing
    log("move ctor");
}

ScopedResource& ScopedResource::operator=(ScopedResource&& other) noexcept {
    if (this != &other) {
        delete handle_;
        handle_ = other.handle_;
        other.handle_ = nullptr;
        label_ = std::move(other.label_);
    }
    log("move assign");
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
