#pragma once

namespace Engine::Graphics {
    class GraphicsContext;

    class Resource {
    public:
        Resource(GraphicsContext& gfx, D3D12_HEAP_PROPERTIES const& heap_properties, D3D12_RESOURCE_DESC const& desc, D3D12_RESOURCE_STATES = D3D12_RESOURCE_STATE_COMMON);
        virtual ~Resource() = default;

        Microsoft::WRL::ComPtr<ID3D12Resource> GetResource() const { return p_resource; }
        D3D12_RESOURCE_STATES  GetState() const { return _state; }

        void Transition(ID3D12GraphicsCommandList& cmd_list, D3D12_RESOURCE_STATES to);
    private:
        D3D12_RESOURCE_STATES                    _state;
        Microsoft::WRL::ComPtr<ID3D12Resource>   p_resource;
    };
}
