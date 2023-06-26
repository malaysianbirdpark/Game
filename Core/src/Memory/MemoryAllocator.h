#pragma once

class BaseAllocator {
public:
    static void* Allocate(int32_t size);
    static void Release(void* ptr);
};

template <typename T>
class STLAllocator {
public:
    using value_type = T;

    STLAllocator() = default;

    template <typename Other>
    STLAllocator(STLAllocator<Other> const&) {}

    T* allocate(size_t count) {
        auto const size {static_cast<int32_t>(count * sizeof(T))};
        return static_cast<T*>(BaseAllocator::Allocate(size));
    }

    void deallocate(T* ptr, size_t count) {
        BaseAllocator::Release(ptr); 
    }
};
