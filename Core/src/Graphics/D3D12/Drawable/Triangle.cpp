#include "pch.h"
#include "Triangle.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Bindable/VertexBuffer.h"
#include "Graphics/D3D12/TEMP/Vertex.h"


Engine::Graphics::Triangle::Triangle(GraphicsContext& gfx) {
    x_vector<Vertex> vertices;
	vertices.resize(3);
	vertices[0]._pos = DirectX::XMFLOAT3(0.f, 0.5f, 0.5f);
	vertices[0]._color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	vertices[1]._pos = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
	vertices[1]._color = DirectX::XMFLOAT4(0.f, 1.0f, 0.f, 1.f);
	vertices[2]._pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
	vertices[2]._color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);

	_vertexBuffer = MakeUnique<VertexBuffer>(gfx, vertices);
}

void Engine::Graphics::Triangle::Draw(GraphicsContext& gfx) {
	_vertexBuffer->Bind(gfx);
    //CMD_LIST().DrawInstanced(_vertexBuffer->GetCount(), 1u, 0u, 0u);
}
