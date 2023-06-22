#pragma once

namespace Engine::Graphics {
    class GraphicsContext;
    class CommandQueue;

    class Fence {
    public:
        Fence(GraphicsContext& gfx);
        ~Fence();

        void SetSignal(std::unique_ptr<CommandQueue> const& p_cmdQueue);
        void SetSignalNext(std::unique_ptr<CommandQueue> const& p_cmdQueue);
        void Wait();
    public:
        Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
        uint32_t                            _fenceValue  {0};
        HANDLE                              _fenceHandle {INVALID_HANDLE_VALUE};
    };
}

