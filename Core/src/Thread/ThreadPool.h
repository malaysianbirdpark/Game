#pragma once

#include "LockQueue.h"
#include "ThreadJoiner.h"

namespace Engine {
    class ThreadPool {
    public:
        ThreadPool();
        ~ThreadPool();

        template <typename FunctionType>
        void Submit(FunctionType f) {
            _tasks.emplace(f);
        }
    private:
        void work();
    private:
        std::atomic_bool                 _done;
        LockQueue<std::function<void()>> _tasks;
        std::vector<std::thread>         _threads;
        ThreadJoiner                     _joiner;
    };
}

