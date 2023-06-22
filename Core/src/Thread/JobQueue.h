#pragma once

#include "Job.h"
#include "JobTimer.h"
#include "LockQueue.h"

class JobQueue : public std::enable_shared_from_this<JobQueue> {
    friend class ThreadMan;
public:
    void DoAsync(CallBackType&& callback) {
        Push(ObjectPool<Job>::MakeShared(std::move(callback)));
    }

    template <typename T, typename Ret, typename... Args>
    void DoAsync(Ret(T::*memFunc)(Args...), Args... args) {
        auto owner {std::static_pointer_cast<T>(shared_from_this())};
        Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
    }

    void DoTimer(uint64_t tick_after, CallBackType&& callback) {
        auto job {ObjectPool<Job>::MakeShared(std::move(callback))};
        GJobTimer->Reserve(tick_after, shared_from_this(), job);
    }

    template <typename T, typename Ret, typename... Args>
    void DoTimer(uint64_t tick_after, Ret(T::*memFunc)(Args...), Args... args) {
        auto owner {std::static_pointer_cast<T>(shared_from_this())};
        auto job {ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...)};
        GJobTimer->Reserve(tick_after, owner, job);
    }

    virtual void FlushJob() = 0;
public:
    void Push(JobRef job, bool push_only = false);
    void Execute();
protected:
    LockQueue<JobRef> _jobs;
    Atomic<int32_t>   _jobCount {};
};


