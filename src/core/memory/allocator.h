#pragma once
#include "core/memory/core.h"

namespace AEngine {

struct IAllocator {
    virtual ~IAllocator() {}
    virtual void* allocate(size_t size, size_t align) = 0;
    virtual void deallocate(void* ptr) = 0;
    virtual void* reallocate(void* ptr, size_t new_size, size_t old_size, size_t align) = 0;

    template <typename T> void deleteObject(T* ptr) {
        if (ptr) {
            ptr->~T();
            deallocate(ptr);
        }
    }
};

// เราจะสร้าง Global Allocator เพื่อให้ง่ายต่อการเริ่มต้น
IAllocator& getGlobalAllocator();

} // namespace AEngine