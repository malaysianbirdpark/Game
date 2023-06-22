#pragma once

namespace Engine::Graphics {
    class GraphicsContext;

    class Commander {
    public:
        Commander(GraphicsContext& gfx, D3D12_COMMAND_LIST_TYPE type);
        ~Commander() = default;

        void Reset();
        void Close();
    private:
        D3D12_COMMAND_LIST_TYPE _type;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_allocator;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_list;
    };
}
