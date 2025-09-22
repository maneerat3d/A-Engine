#pragma once
#include <cstdint>

#define AENGINE_NEW(allocator, ...) new (AEngine::NewPlaceholder(), (allocator).allocate(sizeof(__VA_ARGS__), alignof(__VA_ARGS__))) __VA_ARGS__
#define AENGINE_DELETE(allocator, var) (allocator).deleteObject(var)

namespace AEngine {
    using i8 = char;
    using u8 = unsigned char;
    using i16 = short;
    using u16 = unsigned short;
    using i32 = int;
    using u32 = unsigned int;
    using i64 = long long;
    using u64 = unsigned long long;

    struct NewPlaceholder {};
}
inline void* operator new(size_t, AEngine::NewPlaceholder, void* where) { return where; }
inline void operator delete(void*, AEngine::NewPlaceholder,  void*) { }