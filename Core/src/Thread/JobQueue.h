#pragma once

#include "Job.h"
#include "LockQueue.h"

namespace Engine {
class JobQueue : public std::enable_shared_from_this<JobQueue> {
    friend class ThreadMan;
public:
    void DoAsync(CallBackType&& callback) {
        //Push(MakeShared<Job>(callback));
    }

    template <typename T, typename Ret, typename... Args>
    void DoAsync(Ret(T::*memFunc)(Args...), Args... args) {
        auto owner {std::static_pointer_cast<T>(shared_from_this())};
        Push(MakeShared<Job>(owner, memFunc, std::forward<Args>(args)...));
    }

    virtual void FlushJob() = 0;
public:
    void Push(std::shared_ptr<Job> job, bool push_only = false);
    void Execute();
protected:
    LockQueue<std::shared_ptr<Job>> _jobs;
    std::atomic<int32_t>   _jobCount {};
};


}
