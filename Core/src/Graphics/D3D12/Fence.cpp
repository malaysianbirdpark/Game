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

void Engine::Graphics::Fence::SetSignal(std::unique_ptr<CommandQueue> const& p_cmdQueue) {
    p_cmdQueue->GetQueue().Signal(_fence.Get(), _fenceValue);
}

void Engine::Graphics::Fence::SetSignalNext(std::unique_ptr<CommandQueue> const& p_cmdQueue) {
    ++_fenceValue;
    SetSignal(p_cmdQueue);
}

void Engine::Graphics::Fence::Wait() {
    if (_fence->GetCompletedValue() < _fenceValue) {
        _fence->SetEventOnCompletion(_fenceValue, _fenceHandle);
        ::WaitForSingleObject(_fenceHandle, INFINITE);
    }
}
