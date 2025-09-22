#pragma once
#include "allocator.h"

namespace AEngine {

template <typename T>
struct UniquePtr {
    UniquePtr() : m_ptr(nullptr), m_allocator(nullptr) {}
    UniquePtr(T* obj, IAllocator* allocator) : m_ptr(obj), m_allocator(allocator) {}
    
    template <typename T2>
    UniquePtr(UniquePtr<T2>&& rhs) {
        *this = static_cast<UniquePtr<T2>&&>(rhs);
    }

    ~UniquePtr() {
        if (m_ptr) {
            AENGINE_DELETE(*m_allocator, m_ptr);
        }
    }

    UniquePtr(const UniquePtr& rhs) = delete;
    void operator=(const UniquePtr& rhs) = delete;

    template <typename T2>
    void operator=(UniquePtr<T2>&& rhs) {
        if (m_ptr) {
            AENGINE_DELETE(*m_allocator, m_ptr);
        }
        m_allocator = rhs.getAllocator();
        m_ptr = static_cast<T*>(rhs.detach());
    }

    template <typename... Args> static UniquePtr<T> create(IAllocator& allocator, Args&&... args) {
        return UniquePtr<T>(AENGINE_NEW(allocator, T)(static_cast<Args&&>(args)...), &allocator);
    }

    T* detach() {
        T* res = m_ptr;
        m_ptr = nullptr;
        m_allocator = nullptr;
        return res;
    }

    void reset() {
        if (m_ptr) {
            AENGINE_DELETE(*m_allocator, m_ptr);
        }
        m_ptr = nullptr;
        m_allocator = nullptr;
    }

    T* get() const { return m_ptr; }
    IAllocator* getAllocator() const { return m_allocator; }
    T& operator *() const { return *m_ptr; }
    T* operator ->() const { return m_ptr; }
    operator bool() const { return m_ptr; }

private:
    T* m_ptr = nullptr;
    IAllocator* m_allocator = nullptr;
};

} // namespace AEngine