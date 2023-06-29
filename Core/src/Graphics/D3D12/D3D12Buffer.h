#pragma once

namespace Engine::Graphics {
    class D3D12Buffer {
    public:
        D3D12Buffer();
        ~D3D12Buffer();
    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> _resource;
        D3D12_GPU_VIRTUAL_ADDRESS              _gpuAddr {0};
        size_t                                 _size {};
    };
}

