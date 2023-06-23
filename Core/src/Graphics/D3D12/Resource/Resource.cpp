#include "pch.h"
#include "Resource.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/GraphicsDevice.h"

Engine::Graphics::Resource::Resource(GraphicsContext& gfx, D3D12_HEAP_PROPERTIES const& heap_properties, D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES state)
    : _state{state}
{
    ThrowIfFailed(DEVICE().CreateCommittedResource(
        &heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        state,
        nullptr,
        IID_PPV_ARGS(p_resource.ReleaseAndGetAddressOf())
    ));
}

void Engine::Graphics::Resource::Transition(ID3D12GraphicsCommandList& cmd_list, D3D12_RESOURCE_STATES to) {
    D3D12_RESOURCE_BARRIER const barrier {
        CD3DX12_RESOURCE_BARRIER::Transition(
            p_resource.Get(),
            _state,
            to
        )
    };

    cmd_list.ResourceBarrier(1u, &barrier);

    _state = to;
}
