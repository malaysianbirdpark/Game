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

        // Command Queue, Allocator, List
        {
            D3D12_COMMAND_QUEUE_DESC qd {};
            qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            qd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

            TIF(_device->CreateCommandQueue(&qd, IID_PPV_ARGS(_commandQueue.ReleaseAndGetAddressOf())));
            TIF(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_commandAllocator.ReleaseAndGetAddressOf())));
            TIF(_device->CreateCommandList(0u, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(_commandList.ReleaseAndGetAddressOf())));
            _commandList->Close();
        }

        // SwapChain and BackBuffers
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

        // BackBufferView
        {

            D3D12_DESCRIPTOR_HEAP_DESC desc {};
            desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            desc.NumDescriptors = NUM_BACK_BUFFERS;

            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            desc.NodeMask = 0;

            _backBufferViewHandle.resize(NUM_BACK_BUFFERS);

            TIF(_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(_backBufferView.ReleaseAndGetAddressOf())));

            auto const begin = _backBufferView->GetCPUDescriptorHandleForHeapStart();
            auto const size = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
            for (int i = 0; i != NUM_BACK_BUFFERS; i++)
            {
                _backBufferViewHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(begin, i * size);
                _device->CreateRenderTargetView(_backBuffer[i].Get(), nullptr, _backBufferViewHandle[i]);
            }
        }

        // Fence
        {
            _device->CreateFence(0u, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.ReleaseAndGetAddressOf()));
            _fenceHandle = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
        }

        // For Simplicity, Viewport and the ScissorRect will be always the full screen for now
        {
            _vp.Width = static_cast<float>(width);
            _vp.Height = static_cast<float>(height);
            _vp.TopLeftX = 0.0f;
            _vp.TopLeftY = 0.0f;
            _vp.MinDepth = 0.0f;
            _vp.MaxDepth = 1.0f;

            _rect = CD3DX12_RECT{0, 0, width, height};
        }
}

Engine::Graphics::D3D12Core::~D3D12Core() {
    Wait();
}

void Engine::Graphics::D3D12Core::Render() {
    BeginFrame();
    EndFrame();
}

void Engine::Graphics::D3D12Core::BeginFrame() {
    Wait();

    _commandAllocator->Reset();
    _commandList->Reset(_commandAllocator.Get(), nullptr);

    auto const barrier {
        CD3DX12_RESOURCE_BARRIER::Transition(
            _backBuffer[_bufferIdx].Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET
        )
    };
    _commandList->ResourceBarrier(1u, &barrier);

    ClearScreen();

    // TODO: Binds RootSignatures
    // TODO: Binds PipeliesStateObjects

    // TODO: Viewport and ScissorRect
    _commandList->RSSetViewports(1u, &_vp);
    _commandList->RSSetScissorRects(1u, &_rect);
}

void Engine::Graphics::D3D12Core::EndFrame() {
    auto const barrier {
        CD3DX12_RESOURCE_BARRIER::Transition(
            _backBuffer[_bufferIdx].Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT
        )
    };
    _commandList->ResourceBarrier(1u, &barrier);

    _commandList->Close();

    ExecuteCommand();

    // Present and Swap
    _swapChain->Present(0u, 0u);

    SetNextSignal();

    _bufferIdx = (_bufferIdx + 1) % NUM_BACK_BUFFERS;
}

void Engine::Graphics::D3D12Core::ExecuteCommand() {
    // TODO: Multi-Thread
    ID3D12CommandList* submitted_lists[] {_commandList.Get()};
    _commandQueue->ExecuteCommandLists(std::size(submitted_lists), submitted_lists);
}

void Engine::Graphics::D3D12Core::ClearScreen() {
    static float color[] {0.24f, 0.12f, 0.64f, 1.0f};
    auto const handle {_backBufferViewHandle[_bufferIdx]};
    _commandList->OMSetRenderTargets(1u, &handle, FALSE, nullptr);
    _commandList->ClearRenderTargetView(handle, color, 0u, nullptr);
}

void Engine::Graphics::D3D12Core::SetSignal() {
    _commandQueue->Signal(_fence.Get(), _fenceValue);
}

void Engine::Graphics::D3D12Core::SetNextSignal() {
    ++_fenceValue;
    SetSignal();
}

void Engine::Graphics::D3D12Core::Wait() {
    if (_fence->GetCompletedValue() < _fenceValue) {
        _fence->SetEventOnCompletion(_fenceValue, _fenceHandle);
        ::WaitForSingleObject(_fenceHandle, INFINITE);
    }
}
