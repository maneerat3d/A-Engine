#include "default_allocator.h"
#include <cstdlib> // for malloc, free, realloc

namespace AEngine {

// getGlobalAllocator implementation
IAllocator& getGlobalAllocator() {
    static DefaultAllocator alloc;
    return alloc;
}

void* DefaultAllocator::allocate(size_t size, size_t align) {
    // _aligned_malloc/aligned_alloc จะดีกว่า แต่เพื่อความง่าย เราใช้ malloc ไปก่อน
    return malloc(size);
}

void DefaultAllocator::deallocate(void* ptr) {
    free(ptr);
}

void* DefaultAllocator::reallocate(void* ptr, size_t new_size, size_t old_size, size_t align) {
    return realloc(ptr, new_size);
}

} // namespace AEngine