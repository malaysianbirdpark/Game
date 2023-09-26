#pragma once

namespace Engine {
    class ThreadJoiner {
    public:
        explicit ThreadJoiner(std::vector<std::thread>& threads) : _threads{threads}
        {
        }

        ~ThreadJoiner() {
            for (auto& t : _threads)
                if (t.joinable())
                    t.join();
        }
    private:
        std::vector<std::thread>& _threads;
    };
}

