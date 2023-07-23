#pragma once

#include <chrono>

namespace Engine {
    class Clk {
    public:
        static void Init();

        [[nodiscard]] static float Mark();
        [[nodiscard]] static float Peek();
    private:
        inline static std::chrono::high_resolution_clock::time_point _checkPoint {};
    };
}

