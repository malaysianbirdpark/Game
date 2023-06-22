#include "pch.h"
#include "GraphicsDevice.h"

namespace Engine::Graphics {
    GraphicsDevice::GraphicsDevice() {
#if DEBUG
        ::D3D12GetDebugInterface(IID_PPV_ARGS(_debug.ReleaseAndGetAddressOf()));
        _debug->EnableDebugLayer();
#endif

        ::CreateDXGIFactory(IID_PPV_ARGS(_dxgi.ReleaseAndGetAddressOf()));
        ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(_device.ReleaseAndGetAddressOf()));
    }
}
