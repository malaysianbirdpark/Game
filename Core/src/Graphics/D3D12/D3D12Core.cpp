#include "pch.h"
#include "D3D12Core.h"

Engine::Graphics::D3D12Core::D3D12Core(int width, int height, HWND native_wnd, bool windowed)
    : _windowInfo{width, height, native_wnd, windowed}
{
#if DEBUG
        TIF(::D3D12GetDebugInterface(IID_PPV_ARGS(_debugLayer.ReleaseAndGetAddressOf())));
        _debugLayer->EnableDebugLayer();
        _debugLayer->SetEnableGPUBasedValidation(true);
        _debugLayer->SetEnableSynchronizedCommandQueueValidation(true);
#endif

        TIF(::CreateDXGIFactory(IID_PPV_ARGS(_dxgi.ReleaseAndGetAddressOf())));
        TIF(::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(_device.ReleaseAndGetAddressOf())));

        {
            D3D12_COMMAND_QUEUE_DESC qd {};
            qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            qd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

            TIF(_device->CreateCommandQueue(&qd, IID_PPV_ARGS(_commandQueue.ReleaseAndGetAddressOf())));
        }

        {
            DXGI_SWAP_CHAIN_DESC sd {};
            sd.BufferDesc.Width = static_cast<uint16_t>(_windowInfo.width); 
            sd.BufferDesc.Height = static_cast<uint16_t>(_windowInfo.height); 
            sd.BufferDesc.RefreshRate.Numerator = 60; 
            sd.BufferDesc.RefreshRate.Denominator = 1; 
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
            sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            sd.SampleDesc.Count = 1; 
            sd.SampleDesc.Quality = 0;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
            sd.BufferCount = NUM_BACK_BUFFERS;
            sd.OutputWindow = _windowInfo.nativeWnd;
            sd.Windowed = _windowInfo.windowed;
            sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; 
            sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

            TIF(_dxgi->CreateSwapChain(
                _commandQueue.Get(),
                &sd,
                _swapChain.ReleaseAndGetAddressOf()
            ));

            for (auto i {0}; i != NUM_BACK_BUFFERS; ++i)
                _swapChain->GetBuffer(i, IID_PPV_ARGS(_backBuffer[i].ReleaseAndGetAddressOf()));
        }

}

void Engine::Graphics::D3D12Core::Render() {
}

void Engine::Graphics::D3D12Core::BeginFrame() {
}

void Engine::Graphics::D3D12Core::EndFrame() {
}

void Engine::Graphics::D3D12Core::ExecuteCommand() {
}

void Engine::Graphics::D3D12Core::ClearScreen() {
}
