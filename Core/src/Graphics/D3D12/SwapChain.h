#pragma once

namespace Engine::Graphics {
    class GraphicsContext;
    class RenderTarget;

    class SwapChain {
    public:
        SwapChain(GraphicsContext& gfx);
        ~SwapChain();

        void Present();
        void Swap();

        [[nodiscard]] IDXGISwapChain& GetSwapChain() const { return *_swapChain.Get(); }
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> GetBackBuffers() const;
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> GetBackBufferAt(size_t i) const;
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentBackBuffer() const;
        
        [[nodiscard]] uint16_t GetCurrentBufferIndex() const { return _bufferIdx; }
        [[nodiscard]] uint16_t GetNumOfBuffers() const { return NUM_BACK_BUFFERS; }

        void BeginFrame(ID3D12GraphicsCommandList& cmd_list);
        void EndFrame(ID3D12GraphicsCommandList& cmd_list);
    public:
        static constexpr uint16_t                          NUM_BACK_BUFFERS {3};
        Microsoft::WRL::ComPtr<IDXGISwapChain>             _swapChain;
        x_vector<std::unique_ptr<RenderTarget>>            _renderTarget;
        uint16_t                                           _bufferIdx {};
        //Microsoft::WRL::ComPtr<ID3D12Resource>   _renderTarget[NUM_BACK_BUFFERS];
    };
}

