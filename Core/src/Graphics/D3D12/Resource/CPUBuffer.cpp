#include "pch.h"
#include "CPUBuffer.h"

#include "Graphics/GraphicsContext.h"

Engine::Graphics::CPUBuffer::CPUBuffer(GraphicsContext& gfx, uint32_t size)
    : _size{size}
{
	D3D12_HEAP_PROPERTIES const heap_property {CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)};
	D3D12_RESOURCE_DESC const desc {CD3DX12_RESOURCE_DESC::Buffer(size)};

	_resource = MakeUnique<Resource>(gfx, heap_property, desc, D3D12_RESOURCE_STATE_GENERIC_READ);
}