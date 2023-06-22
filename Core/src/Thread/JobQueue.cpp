#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

void JobQueue::Push(JobRef job, bool push_only) {
    // 항상 이렇게 count를 증가 시킨 뒤에 PUSH를 해야 한다
    auto const prev_count {_jobCount.fetch_add(1)};
    _jobs.Push(job); // WRITE_LOCK

    if (prev_count == 0) {
        if (LCurrentJobQueue == nullptr && !push_only) 
            Execute();
        else // 여유 잇는 다른 thread가 처리
            GGlobalQueue->Push(shared_from_this());
    }
}

void JobQueue::Execute() {
    while (true) {
        bvector<JobRef> jobs;
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
            GGlobalQueue->Push(shared_from_this());
            break;
        }
    }
}
