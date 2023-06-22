#include "pch.h"
#include "SwapChain.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/GraphicsDevice.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Resource/RenderTarget.h"
#include "Resource/Resource.h"

namespace Engine::Graphics {
    SwapChain::SwapChain(GraphicsContext& gfx) {
        DXGI_SWAP_CHAIN_DESC sd;
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

        DXGI().CreateSwapChain(
            &gfx.GetCommandQueueImpl()->GetQueue(),
            &sd,
            _swapChain.ReleaseAndGetAddressOf()
        );

        _renderTarget.reserve(NUM_BACK_BUFFERS);
        for (auto i {0}; i != NUM_BACK_BUFFERS; ++i)
            _renderTarget[i] = MakeUnique<RenderTarget>(
                gfx, 
                gfx.GetWindowData().width,
                gfx.GetWindowData().height,
                DXGI_FORMAT_R8G8B8A8_UNORM
            );
            //_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTarget[i]));
    }

    void SwapChain::Present() {
        _swapChain->Present(0u, 0u);
        Swap();
    }

    void SwapChain::Swap() {
        _bufferIdx = (_bufferIdx + 1) % NUM_BACK_BUFFERS;
    }

    void SwapChain::BeginFrame(ID3D12GraphicsCommandList& cmd_list) {
        _renderTarget[_bufferIdx]->p_resource->Transition(cmd_list, D3D12_RESOURCE_STATE_RENDER_TARGET);
    }

    void SwapChain::EndFrame(ID3D12GraphicsCommandList& cmd_list) {
        _renderTarget[_bufferIdx]->p_resource->Transition(cmd_list, D3D12_RESOURCE_STATE_PRESENT);
    }
}
