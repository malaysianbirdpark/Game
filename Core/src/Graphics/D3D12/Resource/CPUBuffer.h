#pragma once
#include "Resource.h"


namespace Engine::Graphics {
class GraphicsContext;
class Resource;

class CPUBuffer {
public:
    CPUBuffer(GraphicsContext& gfx, uint32_t size);

    Microsoft::WRL::ComPtr<ID3D12Resource> GetResource() { return _resource->GetResource(); }

    D3D12_GPU_VIRTUAL_ADDRESS GetGPUvAddr() { return GetResource()->GetGPUVirtualAddress(); }

    template <typename T>
    void Upload(x_vector<T> const& data);
private:
    std::unique_ptr<Resource> _resource;
    uint32_t                  _size;
};

template <typename T>
void CPUBuffer::Upload(x_vector<T> const& data) {
    T* ptr {nullptr};
	//CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	GetResource()->Map(0, nullptr, reinterpret_cast<void**>(&ptr));
    std::ranges::copy(data, ptr);
	GetResource()->Unmap(0, nullptr);
}
}

