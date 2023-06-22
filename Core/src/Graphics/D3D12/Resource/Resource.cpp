#include "pch.h"
#include "Resource.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/GraphicsDevice.h"

Engine::Graphics::Resource::Resource(GraphicsContext& gfx, D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES state)
    : _state{state}
{
    auto const heap_properties {CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_DEFAULT}};

    static constexpr float clear_color[] {0.0f, 0.75f, 1.0f};
    auto const clear_value {CD3DX12_CLEAR_VALUE{desc.Format, clear_color}};

    DEVICE().CreateCommittedResource(
        &heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        state,
        &clear_value,
        IID_PPV_ARGS(p_resource.ReleaseAndGetAddressOf())
    );
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
