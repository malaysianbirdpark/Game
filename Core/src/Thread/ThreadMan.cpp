#include "pch.h"
#include "ThreadMan.h"
#include "GlobalQueue.h"
#include "JobQueue.h"

Engine::ThreadMan::ThreadMan() {
    // Main Thread Initialization
    InitTLS();
}

Engine::ThreadMan::~ThreadMan() {
    Join();
}

void Engine::ThreadMan::Launch(std::function<void()> const& callback) {
    std::lock_guard<std::mutex> guard {_lock};

    _threads.emplace_back(
        [=]() {
            InitTLS();
            callback();
            DestroyTLS();
        }
    );
}

void Engine::ThreadMan::Join() {
    for (auto& t : _threads)
        if (t.joinable())
            t.join();
    _threads.clear();
}

void Engine::ThreadMan::InitTLS() {
    static std::atomic<uint32_t> SThreadID {1};
    LThreadID = SThreadID.fetch_add(1);
}

void Engine::ThreadMan::DestroyTLS() {
}

//void ThreadMan::DoGlobalQueueWork() {
//    while (true) {
//        auto const now {GetTickCount64()};
//        if (now > LEndTickCount)
//            break;
//
//        auto job_queue {GGlobalQueue->Pop()};
//        if (job_queue == nullptr)
//            break;
//
//        job_queue->Execute();
//    }
//}
//
//void ThreadMan::DistributeReservedJobs() {
//    auto const now {GetTickCount64()};
//    GJobTimer->Distribute(now);
//}
