#pragma once

namespace Engine::Graphics {
struct D3D12DescriptorHandle {
    D3D12_CPU_DESCRIPTOR_HANDLE   _cpuHandle {};
    D3D12_GPU_DESCRIPTOR_HANDLE   _gpuHandle {};
    size_t                        _idx {};
    bool                          _valid {false};
};

class D3D12Descriptor {
    friend class D3D12Resource;
public:
    D3D12Descriptor(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t capacity); 
    ~D3D12Descriptor() = default;

    D3D12DescriptorHandle Allocate();
    void Free(D3D12DescriptorHandle& handle);

    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle(size_t idx);
    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle(size_t idx);
private:
    USE_LOCK;

    D3D12_DESCRIPTOR_HEAP_TYPE                    _type {};
    bool                                          _shaderVisibility {true};
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  _heap;
    x_vector<uint32_t>                            _emptyIndices;
    size_t                                        _capacity;
    size_t                                        _size {};
    size_t                                        _descriptorSize {};
    D3D12_CPU_DESCRIPTOR_HANDLE                   _cpuHandleStart {};
    D3D12_GPU_DESCRIPTOR_HANDLE                   _gpuHandleStart {};
};
}

