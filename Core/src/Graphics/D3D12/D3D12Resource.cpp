#include "pch.h"
#include "D3D12Resource.h"

#include "D3D12Descriptor.h"

Engine::Graphics::D3D12ConstantBuffer::D3D12ConstantBuffer(ID3D12Device* device, size_t element_size) {
    _elemSize = (element_size + 255) & ~255;

    _capacity = _elemSize * 64;
    D3D12_HEAP_PROPERTIES const heap_properties {CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_UPLOAD}};
    D3D12_RESOURCE_DESC const desc {CD3DX12_RESOURCE_DESC::Buffer{_capacity}};

    device->CreateCommittedResource(
        &heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_resource)
    );
}

Engine::Graphics::D3D12ConstantBuffer::~D3D12ConstantBuffer() {
}

void Engine::Graphics::D3D12ConstantBuffer::Push_back(void* data) {
}

void Engine::Graphics::D3D12ConstantBuffer::Transition(ID3D12GraphicsCommandList& cmd_list, D3D12_RESOURCE_STATES to) {
}
