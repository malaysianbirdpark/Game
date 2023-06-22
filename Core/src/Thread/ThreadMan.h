#pragma once

#include <thread>
#include <functional>

namespace Engine {
    class ThreadMan {
    public:
        ThreadMan();
        ~ThreadMan();

        void Launch(std::function<void()> const& callback);
        void Join();

        static void InitTLS();
        static void DestroyTLS();

        //static void DoGlobalQueueWork();
        //static void DistributeReservedJobs();
    private:
        std::mutex                    _lock;
        x_vector<std::thread>         _threads;
    };
};

inline static thread_local uint32_t LThreadID;
inline static thread_local class JobQueue* LCurrentJobQueue;
inline static thread_local Engine::x_stack<int32_t> LLockStack;

//inline static thread_local uint64_t LEndTickCount;
//inline static thread_local SendBufferChunkRef LSendBufferChunk;
