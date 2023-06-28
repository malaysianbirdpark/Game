#include "pch.h"
#include "Commander.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/GraphicsDevice.h"

Engine::Graphics::Commander::Commander(GraphicsContext& gfx, D3D12_COMMAND_LIST_TYPE type)
    : _type{type}
{
    ThrowIfFailed(DEVICE().CreateCommandAllocator(
        _type,
        IID_PPV_ARGS(p_allocator.ReleaseAndGetAddressOf())
    ));

    ThrowIfFailed(DEVICE().CreateCommandList(
        0u,
        _type,
        p_allocator.Get(),
        nullptr,
        IID_PPV_ARGS(p_list.ReleaseAndGetAddressOf())
    ));

    p_list->Close();
}

Engine::Graphics::Commander::~Commander() {
    Reset();
}

void Engine::Graphics::Commander::Reset() {
    p_allocator->Reset();
    p_list->Reset(p_allocator.Get(), nullptr);
}

void Engine::Graphics::Commander::Close() {
    p_list->Close();
}
