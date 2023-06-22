#pragma once

namespace Engine::Graphics {
    class GraphicsContext;

    class CommandQueue {
    public:
        CommandQueue(GraphicsContext& gfx, D3D12_COMMAND_LIST_TYPE type);
        ~CommandQueue() = default;

        [[nodiscard]] ID3D12CommandQueue& GetQueue() const { return *_cmdQueue.Get(); }
    private:
        D3D12_COMMAND_LIST_TYPE _type;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue;
    };
}

