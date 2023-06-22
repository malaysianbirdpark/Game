#include "pch.h"
#include "MemoryAllocator.h"

void* BaseAllocator::Allocate(int32_t size) {
    return malloc(size);
}

void BaseAllocator::Release(void* ptr) {
    free(ptr);
}
