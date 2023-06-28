#pragma once

namespace Engine::Graphics {
class GraphicsContext;

class DescriptorHeap {
public:
    DescriptorHeap(GraphicsContext& gfx, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT num_of_descriptors);
    virtual ~DescriptorHeap() = default;

    D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(size_t idx) const { return _handle[idx]; }
protected:
    D3D12_DESCRIPTOR_HEAP_TYPE                    _type {};
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  _descHeap {};
    size_t                                        _heapSize {};
    x_vector<D3D12_CPU_DESCRIPTOR_HANDLE>         _handle   {};
};
};
