#pragma once

namespace Engine::Graphics {
    struct WindowInfo {
        int width;
        int height;
        HWND nativeWnd;
        bool windowed;
    };

    class D3D12Core {
    public:
        D3D12Core(int width, int height, HWND native_wnd, bool windowed);

        void Render();
    private:
        void BeginFrame();
        void EndFrame();
        void ExecuteCommand();
        void ClearScreen();
    private:
        WindowInfo _windowInfo;
    private:
        Microsoft::WRL::ComPtr<ID3D12Device>        _device;
        Microsoft::WRL::ComPtr<ID3D12Debug1>        _debugLayer;
        Microsoft::WRL::ComPtr<IDXGIFactory>        _dxgi;

        Microsoft::WRL::ComPtr<ID3D12CommandQueue>  _commandQueue;

        static constexpr uint16_t                   NUM_BACK_BUFFERS {3};
        Microsoft::WRL::ComPtr<IDXGISwapChain>      _swapChain;
        Microsoft::WRL::ComPtr<ID3D12Resource>      _backBuffer[NUM_BACK_BUFFERS];
        uint16_t                                    _bufferIdx {};
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

