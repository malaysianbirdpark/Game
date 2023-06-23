#pragma once

namespace Engine::Graphics {
    class GraphicsContext;

    class Fence {
    public:
        Fence(GraphicsContext& gfx);
        ~Fence();

        void SetSignal(GraphicsContext& gfx);
        void SetSignalNext(GraphicsContext& gfx);
        void Wait();
    public:
        Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
        uint32_t                            _fenceValue  {0};
        HANDLE                              _fenceHandle {INVALID_HANDLE_VALUE};
    };
}

