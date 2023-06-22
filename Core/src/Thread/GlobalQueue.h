#pragma once

class GlobalQueue {
public:
    GlobalQueue();
    ~GlobalQueue();

    void Push(JobQueueRef const job_queue);
    JobQueueRef Pop();
private:
    LockQueue<JobQueueRef> _jobQueues;
};

