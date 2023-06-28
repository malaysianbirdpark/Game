#include "pch.h"
#include "GraphicsDevice.h"

#include "Graphics/GraphicsContext.h"

namespace Engine::Graphics {
    GraphicsDevice::GraphicsDevice() {
#if DEBUG
        ThrowIfFailed(::D3D12GetDebugInterface(IID_PPV_ARGS(_debug.ReleaseAndGetAddressOf())));
        _debug->EnableDebugLayer();
        _debug->SetEnableGPUBasedValidation(true);
        _debug->SetEnableSynchronizedCommandQueueValidation(true);
#endif

        ThrowIfFailed(::CreateDXGIFactory(IID_PPV_ARGS(_dxgi.ReleaseAndGetAddressOf())));
        ThrowIfFailed(::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(_device.ReleaseAndGetAddressOf())));
    }
}
