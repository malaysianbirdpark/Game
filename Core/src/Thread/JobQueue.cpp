#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

void Engine::JobQueue::Push(std::shared_ptr<Job> job, bool push_only) {
    // 항상 이렇게 count를 증가 시킨 뒤에 PUSH를 해야 한다
    auto const prev_count {_jobCount.fetch_add(1)};
    _jobs.Push(job); // WRITE_LOCK

    if (prev_count == 0) {
        if (LCurrentJobQueue == nullptr && !push_only) 
            Execute();
        else // 여유 잇는 다른 thread가 처리
            GlobalQueue::GGlobalQueue->Push(shared_from_this());
    }
}

void Engine::JobQueue::Execute() {
    while (true) {
        x_vector<std::shared_ptr<Job>> jobs;
        _jobs.PopAll(OUT jobs);

        auto const job_count {static_cast<int32_t>(jobs.size())};
        for (auto const& j : jobs)
            j->Execute();

        // queue에 있던 모든 job을 완료했으면 빠져나옴
        if (_jobCount.fetch_sub(job_count) == job_count) {
            LCurrentJobQueue = nullptr; 

        }

        auto const now {GetTickCount64()};
        if (now >= LEndTickCount) {
            LCurrentJobQueue = nullptr;
            GlobalQueue::GGlobalQueue->Push(shared_from_this());
            break;
        }
    }
}
