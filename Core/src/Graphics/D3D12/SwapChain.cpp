#include "pch.h"
#include "SwapChain.h"

#include "Graphics/GraphicsContext.h"
#include "GraphicsDevice.h"
#include "CommandQueue.h"

namespace Engine::Graphics {
    SwapChain::SwapChain(GraphicsContext& gfx) {
        DXGI_SWAP_CHAIN_DESC sd {};
        sd.BufferDesc.Width = static_cast<uint16_t>(gfx.GetWindowData().width); 
        sd.BufferDesc.Height = static_cast<uint16_t>(gfx.GetWindowData().height); 
        sd.BufferDesc.RefreshRate.Numerator = 60; 
        sd.BufferDesc.RefreshRate.Denominator = 1; 
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.SampleDesc.Count = 1; 
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
        sd.BufferCount = NUM_BACK_BUFFERS;
        sd.OutputWindow = gfx.GetWindowData().nativeWnd;
        sd.Windowed = gfx.GetWindowData().windowed;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; 
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        ThrowIfFailed(DXGI().CreateSwapChain(
            &gfx.GetCommandQueueImpl()->GetQueue(),
            &sd,
            _swapChain.ReleaseAndGetAddressOf()
        ));

        for (auto i {0}; i != NUM_BACK_BUFFERS; ++i)
            _swapChain->GetBuffer(i, IID_PPV_ARGS(_backbuffer[i].ReleaseAndGetAddressOf()));
    }

    void SwapChain::Present() {
        _swapChain->Present(0u, 0u);
    }

    void SwapChain::Swap() {
        _bufferIdx = (_bufferIdx + 1) % NUM_BACK_BUFFERS;
    }

    Microsoft::WRL::ComPtr<ID3D12Resource> SwapChain::GetBackBuffers() const {
        return _backbuffer[0];
    }

    Microsoft::WRL::ComPtr<ID3D12Resource> SwapChain::GetBackBufferAt(size_t i) const {
        return _backbuffer[i];
    }

    Microsoft::WRL::ComPtr<ID3D12Resource> SwapChain::GetCurrentBackBuffer() const {
        return _backbuffer[_bufferIdx];
    }

    void SwapChain::BeginFrame(ID3D12GraphicsCommandList& cmd_list) {
        auto const barrier {
            CD3DX12_RESOURCE_BARRIER::Transition(
                GetCurrentBackBuffer().Get(),
                D3D12_RESOURCE_STATE_PRESENT,
                D3D12_RESOURCE_STATE_RENDER_TARGET
            )
        };
        cmd_list.ResourceBarrier(1u, &barrier);
    }

    void SwapChain::EndFrame(ID3D12GraphicsCommandList& cmd_list) {
        auto const barrier {
            CD3DX12_RESOURCE_BARRIER::Transition(
                GetCurrentBackBuffer().Get(),
                D3D12_RESOURCE_STATE_RENDER_TARGET,
                D3D12_RESOURCE_STATE_PRESENT
            )
        };
        cmd_list.ResourceBarrier(1u, &barrier);
    }
}
