#include "pch.h"
#include "DescriptorHeap.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/GraphicsDevice.h"

Engine::Graphics::DescriptorHeap::DescriptorHeap(GraphicsContext& gfx, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT num_of_descriptors) {
    _type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    _heapSize = DEVICE().GetDescriptorHandleIncrementSize(_type);

    D3D12_DESCRIPTOR_HEAP_DESC desc {};
    desc.Type = _type;
    desc.NumDescriptors = num_of_descriptors;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = 0;

    _handle.resize(num_of_descriptors);

    ThrowIfFailed(DEVICE().CreateDescriptorHeap(&desc, IID_PPV_ARGS(_descHeap.ReleaseAndGetAddressOf())));
}
