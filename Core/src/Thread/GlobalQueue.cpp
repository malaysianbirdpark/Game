#include "pch.h"
#include "GlobalQueue.h"

Engine::GlobalQueue::~GlobalQueue() {
}

void Engine::GlobalQueue::Init() {
    GGlobalQueue = MakeUnique<GlobalQueue>();
}

void Engine::GlobalQueue::Push(std::shared_ptr<JobQueue> const job_queue) {
    _jobQueues.Push(job_queue);
}

std::shared_ptr<JobQueue> Engine::GlobalQueue::Pop() {
    return _jobQueues.Pop();
}
