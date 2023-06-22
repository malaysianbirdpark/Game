#pragma once

namespace Engine::Graphics {
    class GraphicsContext;

    class SwapChain {
    public:
        SwapChain(GraphicsContext& gfx);
        ~SwapChain() = default;

        void Present();
        void Swap();

        [[nodiscard]] IDXGISwapChain* GetSwapChain() const { return _swapChain.Get(); }
        //[[nodiscard]] ID3D12Resource* GetRenderTargets() const { return _renderTarget[0].Get(); }
        //[[nodiscard]] ID3D12Resource* GetRenderTargetAt(uint16_t i) const { return _renderTarget[i].Get(); }
        //[[nodiscard]] ID3D12Resource* GetCurrentRenderTarget() const { return _renderTarget[_bufferIdx].Get(); }
        
        [[nodiscard]] uint16_t GetCurrentBufferIndex() const { return _bufferIdx; }
        [[nodiscard]] uint16_t GetNumOfBuffers() const { return NUM_BACK_BUFFERS; }

        void BeginFrame(ID3D12GraphicsCommandList& cmd_list);
        void EndFrame(ID3D12GraphicsCommandList& cmd_list);
    public:
        static constexpr uint16_t                          NUM_BACK_BUFFERS {3};
        Microsoft::WRL::ComPtr<IDXGISwapChain>             _swapChain;
        std::vector<std::unique_ptr<class RenderTarget>>   _renderTarget;
        uint16_t                                           _bufferIdx {};

        //Microsoft::WRL::ComPtr<ID3D12Resource>   _renderTarget[NUM_BACK_BUFFERS];
    };
}

