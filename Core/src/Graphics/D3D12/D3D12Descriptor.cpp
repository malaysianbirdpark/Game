#include "pch.h"
#include "D3D12Descriptor.h"

Engine::Graphics::D3D12Descriptor::D3D12Descriptor(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t capacity)
    : _type{type}, _capacity{capacity}
{
    WRITE_LOCK;

    if ((type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV) | (type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV))
        _shaderVisibility = false;

    D3D12_DESCRIPTOR_HEAP_DESC desc {};
    desc.Flags = _shaderVisibility ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NumDescriptors = capacity;
    desc.Type = type;
    desc.NodeMask = 0u;

    device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(_heap.ReleaseAndGetAddressOf()));
    _descriptorSize = device->GetDescriptorHandleIncrementSize(_type);
    _cpuHandleStart = _heap->GetCPUDescriptorHandleForHeapStart();
    _gpuHandleStart = _shaderVisibility ? _heap->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE {0};

    _emptyIndices.resize(_capacity);
    for (auto i {0}; i != capacity; ++i)
        _emptyIndices[i] = i;
}

Engine::Graphics::D3D12DescriptorHandle Engine::Graphics::D3D12Descriptor::Allocate() {
    WRITE_LOCK;

    auto const idx {_emptyIndices[_size++]};
    auto const offset {idx * _descriptorSize};

    auto handle {D3D12DescriptorHandle{}};
    handle._cpuHandle.ptr = _cpuHandleStart.ptr + offset;
    if (_shaderVisibility)
        handle._gpuHandle.ptr = _gpuHandleStart.ptr + offset;
    handle._idx = idx;
    handle._valid = true;

    return handle;
}

void Engine::Graphics::D3D12Descriptor::Free(D3D12DescriptorHandle& handle) {
    WRITE_LOCK;

    if (!handle._valid)
        return;

    --_size;

    handle = D3D12DescriptorHandle{};
}

//D3D12_CPU_DESCRIPTOR_HANDLE Engine::Graphics::D3D12Descriptor::CPUHandle(size_t idx) {
//}
//
//D3D12_GPU_DESCRIPTOR_HANDLE Engine::Graphics::D3D12Descriptor::GPUHandle(size_t idx) {
//}
