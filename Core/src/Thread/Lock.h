#pragma once

// Reader-Writer SpinLock

// policy
// W lock: exclusive lock owner
// R lock: shared lock count
// W -> W (0)
// W -> R (0)
// R -> W (X)
// R -> R (0)
namespace Engine {
class Lock {
    enum : uint32_t {
        ACQUIRE_TIMEOUT      = 5'000'000'000,
        MAX_SPIN_COUNT       = 5'000,
        WRITE_THREAD_MASK    = 0xFFFF'0000,
        READ_COUNT_MASK      = 0x0000'FFFF,
        EMPTY_FLAG           = 0x0000'0000,
    };
public:
    void WriteLock(char const* name);
    void WriteUnlock(char const* name);
    void ReadLock(char const* name);
    void ReadUnlock(char const* name);
private:
    std::atomic<uint32_t> _lockFlag;
    uint16_t _writeCount {};
};

class ReadLockGuard {
public:
    ReadLockGuard(Lock& lock, char const* name) : _lock{lock}, _name{name} { _lock.ReadLock(_name.c_str()); }
    ~ReadLockGuard() { _lock.ReadUnlock(_name.c_str()); }
private:
    Lock& _lock;
    std::string _name;
};

class WriteLockGuard {
public:
    WriteLockGuard(Lock& lock, char const* name) : _lock{lock}, _name{name} { _lock.WriteLock(_name.c_str()); }
    ~WriteLockGuard() { _lock.WriteUnlock(_name.c_str()); }
private:
    Lock& _lock;
    std::string _name;
};
}

#define USE_MULTIPLE_LOCK(count) Lock _locks[count]
#define USE_LOCK                 USE_MULTIPLE_LOCK(1)
#define READ_LOCK_IDX(idx)       ReadLockGuard readLockGuard_##idx {_locks[idx], typeid(this).name()}
#define READ_LOCK                READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)      WriteLockGuard writeLockGuard_##idx {_locks[idx], typeid(this).name()}
#define WRITE_LOCK               WRITE_LOCK_IDX(0)
