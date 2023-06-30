#pragma once

namespace Engine::Graphics {
    class D3D12Core {
    //public:
    //    D3D12Core(int width, int height, HWND native_wnd, bool windowed);
    //    ~D3D12Core();

    //    void Render();
    //private:
    //    void BeginFrame();
    //    void EndFrame();
    //    void ExecuteCommand();
    //    void ClearScreen();
    //private:
    //    // Fence methods
    //    void SetSignal();
    //    void SetNextSignal();
    //    void Wait();
    //private:
    //    WindowInfo _windowInfo;
    //private:
    //    Microsoft::WRL::ComPtr<ID3D12Device>                _device;
    //    Microsoft::WRL::ComPtr<ID3D12Debug1>                _debugLayer;
    //    Microsoft::WRL::ComPtr<IDXGIFactory>                _dxgi;

    //    Microsoft::WRL::ComPtr<ID3D12CommandQueue>          _commandQueue;
    //    // Helper Command Allocator-List
    //    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      _commandAllocator;
    //    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   _commandList;

    //    static constexpr uint16_t                           NUM_BACK_BUFFERS {3};
    //    Microsoft::WRL::ComPtr<IDXGISwapChain>              _swapChain;
    //    Microsoft::WRL::ComPtr<ID3D12Resource>              _backBuffer[NUM_BACK_BUFFERS];
    //    uint16_t                                            _bufferIdx {};

    //    // BackBufferView
    //    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        _backBufferView;
    //    x_vector<D3D12_CPU_DESCRIPTOR_HANDLE>               _backBufferViewHandle;

    //    // TODO: Good Synchronization method?
    //    Microsoft::WRL::ComPtr<ID3D12Fence>                 _fence;
    //    uint32_t                                            _fenceValue  {0};
    //    HANDLE                                              _fenceHandle {INVALID_HANDLE_VALUE};

    //    // TODO: Viewport and ScissorRect
    //    D3D12_VIEWPORT _vp {};
    //    CD3DX12_RECT _rect {};
    };
}

inline void TIF(HRESULT hr)
{
    if (FAILED(hr))
    {
        std::cout << GetLastError() << std::endl;
        throw 1;
    }
}

