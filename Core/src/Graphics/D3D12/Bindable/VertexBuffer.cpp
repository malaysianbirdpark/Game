#include "pch.h"
#include "VertexBuffer.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Commander.h"
#include "..\Resource\CPUBuffer.h"
#include "Graphics/D3D12/TEMP/Vertex.h"

Engine::Graphics::VertexBuffer::VertexBuffer(GraphicsContext& gfx, x_vector<Vertex> const& vertices) {
	_count = vertices.size();
	auto buffer_size = _count * sizeof(Vertex);
	_buffer = MakeUnique<CPUBuffer>(gfx, buffer_size);

	// Copy the triangle data to the vertex buffer.
	_buffer->Upload(vertices);

	// Initialize the vertex buffer view.
	_view.BufferLocation = _buffer->GetGPUvAddr();
	_view.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_view.SizeInBytes = buffer_size; // 버퍼의 크기	
}

void Engine::Graphics::VertexBuffer::Bind(GraphicsContext& gfx) noexcept {
    // TODO: primitive topology
    CMD_LIST().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    CMD_LIST().IASetVertexBuffers(0u, 1u, &_view);
}

std::string Engine::Graphics::VertexBuffer::GetUID() const noexcept {
    return Bindable::GetUID();
}
