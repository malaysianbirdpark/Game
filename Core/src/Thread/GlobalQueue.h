#pragma once

#include "LockQueue.h"

namespace Engine {
class GlobalQueue {
public:
    ~GlobalQueue();

    static void Init();

    void Push(std::shared_ptr<JobQueue> const job_queue);
    std::shared_ptr<JobQueue> Pop();
private:
    LockQueue<std::shared_ptr<JobQueue>> _jobQueues;
public:
    inline static std::unique_ptr<GlobalQueue> GGlobalQueue {};
};
}

