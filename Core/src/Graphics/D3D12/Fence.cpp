#include "pch.h"
#include "Fence.h"

#include "GraphicsDevice.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/CommandQueue.h"

Engine::Graphics::Fence::Fence(GraphicsContext& gfx) {
    DEVICE().CreateFence(0u, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.ReleaseAndGetAddressOf()));
    _fenceHandle = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

Engine::Graphics::Fence::~Fence() {
    ::CloseHandle(_fenceHandle);
}

void Engine::Graphics::Fence::SetSignal(GraphicsContext& gfx) {
    CMD_QUEUE().Signal(_fence.Get(), _fenceValue);
}

void Engine::Graphics::Fence::SetSignalNext(GraphicsContext& gfx) {
    ++_fenceValue;
    SetSignal(gfx);
}

void Engine::Graphics::Fence::Wait() {
    if (_fence->GetCompletedValue() < _fenceValue) {
        _fence->SetEventOnCompletion(_fenceValue, _fenceHandle);
        ::WaitForSingleObject(_fenceHandle, INFINITE);
    }
}
