#pragma once
#include "allocator.h"

namespace AEngine {

// Allocator พื้นฐานที่ใช้ malloc/free
class DefaultAllocator final : public IAllocator {
public:
    void* allocate(size_t size, size_t align) override;
    void deallocate(void* ptr) override;
    void* reallocate(void* ptr, size_t new_size, size_t old_size, size_t align) override;
};

} // namespace AEngine