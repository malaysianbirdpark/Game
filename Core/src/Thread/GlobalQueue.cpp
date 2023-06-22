#include "pch.h"
#include "GlobalQueue.h"

GlobalQueue::GlobalQueue() {
}

GlobalQueue::~GlobalQueue() {
}

void GlobalQueue::Push(JobQueueRef const job_queue) {
    _jobQueues.Push(job_queue);
}

JobQueueRef GlobalQueue::Pop() {
    return _jobQueues.Pop();
}
