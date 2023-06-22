#include "pch.h"
#include "Lock.h"

void Lock::WriteLock(char const* name) {
#ifdef DEBUG
    GDeadLockProfiler->PushLock(name);
#endif
    
    // If the same thread has the ownership, the result is always success
    uint32_t const lockThreadId {(_lockFlag.load() & WRITE_THREAD_MASK) >> 16};
    if (LThreadID == lockThreadId) {
        ++_writeCount;
        return;
    }

    // Getting ownership by competition, when no one has it.
    /*
    if (_lockFlag == EMPTY_FLAG) {
        uint32_t const desired {(LThreadId << 16) & WRITE_THREAD_MASK)};
        _lockFlag = desired;
    }
    */
    auto const start {std::chrono::high_resolution_clock::now()};
    uint32_t const desired {(LThreadID << 16) & WRITE_THREAD_MASK};
    while (true) {
        for (auto spinCount {0}; spinCount != MAX_SPIN_COUNT; ++spinCount) {
            uint32_t expected {EMPTY_FLAG};
            if (_lockFlag.compare_exchange_strong(OUT expected, desired)) {
                // _lockFlag is empty! success!
                ++_writeCount;
                return;
            }
        }

        auto const elapsed {(std::chrono::high_resolution_clock::now() - start).count()};
        if (elapsed >= ACQUIRE_TIMEOUT)
            CRASH("LOCK_TIMEOUT");

        std::this_thread::yield();
    }
}

void Lock::WriteUnlock(char const* name) {
#ifdef DEBUG
    GDeadLockProfiler->PopLock(name);
#endif
    
    // Write unlock is impossible before all the Read locks are unlocked
    if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
        CRASH("INVALID_UNLOCK_ORDER");

    int32_t const lock_count {--_writeCount};
    if (lock_count == 0)
        _lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(char const* name) {
#ifdef DEBUG
    GDeadLockProfiler->PushLock(name);
#endif
    
    // if the same thread has W lock, always success
    uint32_t const lockThreadId {(_lockFlag.load() & READ_COUNT_MASK)};
    if (LThreadID == lockThreadId) {
        _lockFlag.fetch_add(1);
        return;
    }

    // if no one has W lock, then threads get into competition for counting up the shared read counter
    auto const start {std::chrono::high_resolution_clock::now()};
    while (true) {
        for (uint32_t spinCount {0}; spinCount != MAX_SPIN_COUNT; ++spinCount) {
            uint32_t expected {(_lockFlag.load() & READ_COUNT_MASK)};
            if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
                return; // success
        }

        auto const elapsed {(std::chrono::high_resolution_clock::now() - start).count()};
        if (elapsed >= ACQUIRE_TIMEOUT)
            CRASH("LOCK_TIMEOUT");

        std::this_thread::yield();
    }
}

void Lock::ReadUnlock(char const* name) {
#ifdef DEBUG
    GDeadLockProfiler->PopLock(name);
#endif
    
    // caution!! fetch_x's return value is previous value (before modification)
    if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0) {
        // _lockFlag was 0
        CRASH("MULTIPLE_READ_UNLOCK");
    }
}
